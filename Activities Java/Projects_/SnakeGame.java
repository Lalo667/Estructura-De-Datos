import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Random;
import java.util.Collections;
import java.io.*;

public class SnakeGame extends JPanel implements ActionListener, KeyListener {
    private static final int TAMANO_CELDA = 25;
    private static final int TAMANO_TABLERO = 20;
    private static final int ANCHO = TAMANO_CELDA * TAMANO_TABLERO;
    private static final int ALTO = TAMANO_CELDA * TAMANO_TABLERO;
    private static final String ARCHIVO_RANKINGS = "snake_rankings.dat";
    private static final String ARCHIVO_SLOT = "snake_slot_";
    
    private ArrayList<Point> serpiente;
    private ArrayList<Point> trampas; // Múltiples trampas
    private Point comida;
    private int direccion;
    private int siguienteDireccion;
    private Timer timer;
    private boolean juegoActivo;
    private int nivel;
    private int puntuacion;
    private String nombreJugador;
    private int velocidadInicial = 150;
    private Random random;
    private boolean movimientoProcesado;
    
    // Estados del juego
    private enum Estado { MENU, JUGANDO, GAME_OVER, RANKINGS, CARGAR_PARTIDA }
    private Estado estadoActual;
    
    // Sistema de rankings
    private static ArrayList<Jugador> rankings = new ArrayList<>();
    
    // Clase para guardar partidas
    static class PartidaGuardada implements Serializable {
        private static final long serialVersionUID = 1L;
        String nombreJugador;
        int nivel;
        int puntuacion;
        ArrayList<Point> serpiente;
        int direccion;
        
        PartidaGuardada(String nombre, int nivel, int puntuacion, ArrayList<Point> serpiente, int direccion) {
            this.nombreJugador = nombre;
            this.nivel = nivel;
            this.puntuacion = puntuacion;
            this.serpiente = new ArrayList<>(serpiente);
            this.direccion = direccion;
        }
    }
    
    // Clase interna para almacenar datos de jugadores
    static class Jugador implements Comparable<Jugador>, Serializable {
        private static final long serialVersionUID = 1L;
        String nombre;
        int puntuacion;
        int nivel;
        
        Jugador(String nombre, int puntuacion, int nivel) {
            this.nombre = nombre;
            this.puntuacion = puntuacion;
            this.nivel = nivel;
        }
        
        @Override
        public int compareTo(Jugador otro) {
            return Integer.compare(otro.puntuacion, this.puntuacion);
        }
    }
    
    public SnakeGame() {
        this.serpiente = new ArrayList<>();
        this.trampas = new ArrayList<>();
        this.random = new Random();
        this.nivel = 1;
        this.puntuacion = 0;
        this.estadoActual = Estado.MENU;
        this.movimientoProcesado = true;
        
        setPreferredSize(new Dimension(ANCHO, ALTO + 50));
        setBackground(Color.BLACK);
        setFocusable(true);
        addKeyListener(this);
        
        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                manejarClick(e.getX(), e.getY());
            }
        });
        
        cargarRankings();
    }
    
    private void cargarRankings() {
        try {
            File archivo = new File(ARCHIVO_RANKINGS);
            if (archivo.exists()) {
                FileInputStream fis = new FileInputStream(archivo);
                ObjectInputStream ois = new ObjectInputStream(fis);
                rankings = (ArrayList<Jugador>) ois.readObject();
                ois.close();
                fis.close();
            }
        } catch (Exception e) {
            System.out.println("No se pudieron cargar los rankings: " + e.getMessage());
            rankings = new ArrayList<>();
        }
    }
    
    private void guardarRankings() {
        try {
            FileOutputStream fos = new FileOutputStream(ARCHIVO_RANKINGS);
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(rankings);
            oos.close();
            fos.close();
        } catch (Exception e) {
            System.out.println("Error al guardar rankings: " + e.getMessage());
        }
    }
    
    private void guardarPartida(int slot) {
        try {
            PartidaGuardada partida = new PartidaGuardada(nombreJugador, nivel, puntuacion, serpiente, direccion);
            FileOutputStream fos = new FileOutputStream(ARCHIVO_SLOT + slot + ".dat");
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(partida);
            oos.close();
            fos.close();
            JOptionPane.showMessageDialog(this, "Partida guardada en Slot " + slot, "Guardado exitoso", JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Error al guardar: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    private void cargarPartida(int slot) {
        try {
            File archivo = new File(ARCHIVO_SLOT + slot + ".dat");
            if (!archivo.exists()) {
                JOptionPane.showMessageDialog(this, "No hay partida guardada en el Slot " + slot, "Slot vacío", JOptionPane.WARNING_MESSAGE);
                return;
            }
            
            FileInputStream fis = new FileInputStream(archivo);
            ObjectInputStream ois = new ObjectInputStream(fis);
            PartidaGuardada partida = (PartidaGuardada) ois.readObject();
            ois.close();
            fis.close();
            
            nombreJugador = partida.nombreJugador;
            nivel = partida.nivel;
            puntuacion = partida.puntuacion;
            serpiente = new ArrayList<>(partida.serpiente);
            direccion = partida.direccion;
            siguienteDireccion = partida.direccion;
            
            estadoActual = Estado.JUGANDO;
            juegoActivo = true;
            movimientoProcesado = true;
            
            generarComidaYTrampas();
            
            if (timer != null) {
                timer.stop();
            }
            
            int velocidad = Math.max(50, velocidadInicial - (nivel - 1) * 10);
            timer = new Timer(velocidad, this);
            timer.start();
            
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Error al cargar: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    private String obtenerInfoSlot(int slot) {
        try {
            File archivo = new File(ARCHIVO_SLOT + slot + ".dat");
            if (!archivo.exists()) {
                return "VACÍO";
            }
            
            FileInputStream fis = new FileInputStream(archivo);
            ObjectInputStream ois = new ObjectInputStream(fis);
            PartidaGuardada partida = (PartidaGuardada) ois.readObject();
            ois.close();
            fis.close();
            
            return partida.nombreJugador + " - Nv." + partida.nivel + " - " + partida.puntuacion + "pts";
        } catch (Exception e) {
            return "ERROR";
        }
    }
    
    private void manejarClick(int x, int y) {
        if (estadoActual == Estado.MENU) {
            // Botón Iniciar Partida
            if (x >= ANCHO/2 - 100 && x <= ANCHO/2 + 100 && y >= 200 && y <= 250) {
                String nombre = JOptionPane.showInputDialog(null, 
                    "Ingresa tu nombre:", 
                    "Snake Game", 
                    JOptionPane.QUESTION_MESSAGE);
                
                if (nombre != null && !nombre.trim().isEmpty()) {
                    nombreJugador = nombre.trim();
                    estadoActual = Estado.JUGANDO;
                    inicializarJuego();
                }
            }
            // Botón Cargar Partida
            else if (x >= ANCHO/2 - 100 && x <= ANCHO/2 + 100 && y >= 270 && y <= 320) {
                estadoActual = Estado.CARGAR_PARTIDA;
                repaint();
            }
            // Botón Rankings
            else if (x >= ANCHO/2 - 100 && x <= ANCHO/2 + 100 && y >= 340 && y <= 390) {
                estadoActual = Estado.RANKINGS;
                repaint();
            }
        } else if (estadoActual == Estado.CARGAR_PARTIDA) {
            // Slots de carga
            for (int i = 1; i <= 3; i++) {
                if (x >= ANCHO/2 - 150 && x <= ANCHO/2 + 150 && 
                    y >= 150 + (i-1) * 80 && y <= 210 + (i-1) * 80) {
                    cargarPartida(i);
                    return;
                }
            }
            // Botón Volver
            if (x >= ANCHO/2 - 100 && x <= ANCHO/2 + 100 && y >= ALTO - 80 && y <= ALTO - 40) {
                estadoActual = Estado.MENU;
                repaint();
            }
        } else if (estadoActual == Estado.GAME_OVER) {
            // Botón Volver a Jugar
            if (x >= ANCHO/2 - 150 && x <= ANCHO/2 - 10 && y >= ALTO/2 + 90 && y <= ALTO/2 + 130) {
                nivel = 1;
                puntuacion = 0;
                estadoActual = Estado.JUGANDO;
                inicializarJuego();
            }
            // Botón Menú Principal
            else if (x >= ANCHO/2 + 10 && x <= ANCHO/2 + 150 && y >= ALTO/2 + 90 && y <= ALTO/2 + 130) {
                estadoActual = Estado.MENU;
                repaint();
            }
        } else if (estadoActual == Estado.RANKINGS) {
            // Botón Volver al Menú
            if (x >= ANCHO/2 - 100 && x <= ANCHO/2 + 100 && y >= ALTO - 80 && y <= ALTO - 40) {
                estadoActual = Estado.MENU;
                repaint();
            }
        }
    }
    
    private void inicializarJuego() {
        serpiente.clear();
        trampas.clear();
        int centroX = TAMANO_TABLERO / 2;
        int centroY = TAMANO_TABLERO / 2;
        
        for (int i = 0; i < 5; i++) {
            serpiente.add(new Point(centroX - i, centroY));
        }
        
        direccion = 1;
        siguienteDireccion = 1;
        juegoActivo = true;
        movimientoProcesado = true;
        
        generarComidaYTrampas();
        
        if (timer != null) {
            timer.stop();
        }
        
        int velocidad = Math.max(50, velocidadInicial - (nivel - 1) * 10);
        timer = new Timer(velocidad, this);
        timer.start();
    }
    
    private void generarComidaYTrampas() {
        // Generar comida
        do {
            comida = new Point(random.nextInt(TAMANO_TABLERO), random.nextInt(TAMANO_TABLERO));
        } while (serpiente.contains(comida));
        
        // Generar trampas (aumentan según el nivel)
        trampas.clear();
        int numTrampas = 1 + (nivel - 1) / 2; // 1 trampa al inicio, aumenta cada 2 niveles
        
        for (int i = 0; i < numTrampas; i++) {
            Point trampa;
            do {
                trampa = new Point(random.nextInt(TAMANO_TABLERO), random.nextInt(TAMANO_TABLERO));
            } while (serpiente.contains(trampa) || trampa.equals(comida) || trampas.contains(trampa));
            
            trampas.add(trampa);
        }
    }
    
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        
        if (estadoActual == Estado.MENU) {
            dibujarMenu(g);
        } else if (estadoActual == Estado.JUGANDO) {
            dibujarJuego(g);
        } else if (estadoActual == Estado.GAME_OVER) {
            dibujarGameOver(g);
        } else if (estadoActual == Estado.RANKINGS) {
            dibujarRankings(g);
        } else if (estadoActual == Estado.CARGAR_PARTIDA) {
            dibujarCargarPartida(g);
        }
    }
    
    private void dibujarMenu(Graphics g) {
        g.setColor(Color.WHITE);
        g.setFont(new Font("Arial", Font.BOLD, 50));
        String titulo = "SNAKE GAME";
        FontMetrics fm = g.getFontMetrics();
        g.drawString(titulo, (ANCHO - fm.stringWidth(titulo)) / 2, 120);
        
        // Botón Iniciar Partida
        g.setColor(new Color(0, 150, 0));
        g.fillRect(ANCHO/2 - 100, 200, 200, 50);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 - 100, 200, 200, 50);
        g.setFont(new Font("Arial", Font.BOLD, 20));
        g.drawString("INICIAR PARTIDA", ANCHO/2 - 90, 232);
        
        // Botón Cargar Partida
        g.setColor(new Color(150, 100, 0));
        g.fillRect(ANCHO/2 - 100, 270, 200, 50);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 - 100, 270, 200, 50);
        g.drawString("CARGAR PARTIDA", ANCHO/2 - 85, 302);
        
        // Botón Rankings
        g.setColor(new Color(0, 100, 200));
        g.fillRect(ANCHO/2 - 100, 340, 200, 50);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 - 100, 340, 200, 50);
        g.drawString("RANKINGS", ANCHO/2 - 55, 372);
    }
    
    private void dibujarCargarPartida(Graphics g) {
        g.setColor(Color.WHITE);
        g.setFont(new Font("Arial", Font.BOLD, 40));
        String titulo = "CARGAR PARTIDA";
        FontMetrics fm = g.getFontMetrics();
        g.drawString(titulo, (ANCHO - fm.stringWidth(titulo)) / 2, 80);
        
        g.setFont(new Font("Arial", Font.PLAIN, 14));
        g.drawString("Selecciona un slot para cargar", ANCHO/2 - 100, 120);
        
        // Dibujar los 3 slots
        for (int i = 1; i <= 3; i++) {
            String infoSlot = obtenerInfoSlot(i);
            
            g.setColor(new Color(50, 50, 100));
            g.fillRect(ANCHO/2 - 150, 150 + (i-1) * 80, 300, 60);
            g.setColor(Color.WHITE);
            g.drawRect(ANCHO/2 - 150, 150 + (i-1) * 80, 300, 60);
            
            g.setFont(new Font("Arial", Font.BOLD, 18));
            g.drawString("SLOT " + i, ANCHO/2 - 130, 175 + (i-1) * 80);
            
            g.setFont(new Font("Arial", Font.PLAIN, 14));
            g.drawString(infoSlot, ANCHO/2 - 130, 195 + (i-1) * 80);
        }
        
        // Botón Volver
        g.setColor(new Color(200, 0, 0));
        g.fillRect(ANCHO/2 - 100, ALTO - 80, 200, 40);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 - 100, ALTO - 80, 200, 40);
        g.setFont(new Font("Arial", Font.BOLD, 18));
        g.drawString("VOLVER AL MENÚ", ANCHO/2 - 80, ALTO - 53);
    }
    
    private void dibujarJuego(Graphics g) {
        if (juegoActivo) {
            // Dibujar serpiente
            for (int i = 0; i < serpiente.size(); i++) {
                Point p = serpiente.get(i);
                if (i == 0) {
                    g.setColor(Color.GREEN);
                } else {
                    g.setColor(new Color(0, 200, 0));
                }
                g.fillRect(p.x * TAMANO_CELDA, p.y * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA);
                g.setColor(Color.BLACK);
                g.drawRect(p.x * TAMANO_CELDA, p.y * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA);
            }
            
            // Dibujar comida
            g.setColor(Color.BLUE);
            g.fillOval(comida.x * TAMANO_CELDA, comida.y * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA);
            
            // Dibujar trampas
            g.setColor(Color.RED);
            for (Point trampa : trampas) {
                g.fillOval(trampa.x * TAMANO_CELDA, trampa.y * TAMANO_CELDA, TAMANO_CELDA, TAMANO_CELDA);
            }
            
            // Información del juego
            g.setColor(Color.WHITE);
            g.setFont(new Font("Arial", Font.BOLD, 14));
            g.drawString("Jugador: " + nombreJugador + " | Nivel: " + nivel + " | Puntuación: " + puntuacion + 
                        " | Tamaño: " + serpiente.size() + " | Trampas: " + trampas.size(), 10, ALTO + 20);
            
            // Leyenda
            g.setFont(new Font("Arial", Font.PLAIN, 11));
            g.setColor(Color.BLUE);
            g.fillOval(10, ALTO + 25, 15, 15);
            g.setColor(Color.WHITE);
            g.drawString("= Comida (+100 pts, +tamaño)", 30, ALTO + 37);
            
            g.setColor(Color.RED);
            g.fillOval(220, ALTO + 25, 15, 15);
            g.setColor(Color.WHITE);
            g.drawString("= Trampa (-200 pts, -tamaño)", 240, ALTO + 37);
            
            g.drawString("Presiona 'F' para guardar", 400, ALTO + 37);
        }
    }
    
    private void dibujarGameOver(Graphics g) {
        g.setColor(Color.WHITE);
        g.setFont(new Font("Arial", Font.BOLD, 40));
        String msg = "GAME OVER";
        FontMetrics metrics = g.getFontMetrics();
        g.drawString(msg, (ANCHO - metrics.stringWidth(msg)) / 2, ALTO / 2 - 50);
        
        g.setFont(new Font("Arial", Font.PLAIN, 24));
        String scoreMsg = "Puntuación Final: " + puntuacion;
        g.drawString(scoreMsg, (ANCHO - g.getFontMetrics().stringWidth(scoreMsg)) / 2, ALTO / 2);
        
        String nivelMsg = "Nivel Alcanzado: " + nivel;
        g.drawString(nivelMsg, (ANCHO - g.getFontMetrics().stringWidth(nivelMsg)) / 2, ALTO / 2 + 40);
        
        // Botón Volver a Jugar
        g.setColor(new Color(0, 150, 0));
        g.fillRect(ANCHO/2 - 150, ALTO/2 + 90, 140, 40);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 - 150, ALTO/2 + 90, 140, 40);
        g.setFont(new Font("Arial", Font.BOLD, 16));
        g.drawString("VOLVER A JUGAR", ANCHO/2 - 140, ALTO/2 + 117);
        
        // Botón Menú
        g.setColor(new Color(200, 0, 0));
        g.fillRect(ANCHO/2 + 10, ALTO/2 + 90, 140, 40);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 + 10, ALTO/2 + 90, 140, 40);
        g.drawString("MENÚ PRINCIPAL", ANCHO/2 + 17, ALTO/2 + 117);
    }
    
    private void dibujarRankings(Graphics g) {
        g.setColor(Color.WHITE);
        g.setFont(new Font("Arial", Font.BOLD, 40));
        String titulo = "RANKINGS";
        FontMetrics fm = g.getFontMetrics();
        g.drawString(titulo, (ANCHO - fm.stringWidth(titulo)) / 2, 80);
        
        g.setFont(new Font("Arial", Font.PLAIN, 18));
        
        if (rankings.isEmpty()) {
            g.drawString("No hay registros aún", ANCHO/2 - 80, ALTO/2);
        } else {
            g.setFont(new Font("Arial", Font.BOLD, 16));
            g.drawString("POS    JUGADOR              PUNTOS    NIVEL", 80, 150);
            
            g.setFont(new Font("Arial", Font.PLAIN, 16));
            for (int i = 0; i < Math.min(10, rankings.size()); i++) {
                Jugador j = rankings.get(i);
                String linea = String.format("%2d.    %-20s %6d     %3d", 
                    i + 1, j.nombre, j.puntuacion, j.nivel);
                g.drawString(linea, 80, 180 + i * 30);
            }
        }
        
        // Botón Volver
        g.setColor(new Color(0, 100, 200));
        g.fillRect(ANCHO/2 - 100, ALTO - 80, 200, 40);
        g.setColor(Color.WHITE);
        g.drawRect(ANCHO/2 - 100, ALTO - 80, 200, 40);
        g.setFont(new Font("Arial", Font.BOLD, 18));
        g.drawString("VOLVER AL MENÚ", ANCHO/2 - 80, ALTO - 53);
    }
    
    @Override
    public void actionPerformed(ActionEvent e) {
        if (juegoActivo && estadoActual == Estado.JUGANDO) {
            mover();
            movimientoProcesado = true;
            repaint();
        }
    }
    
    private void mover() {
        direccion = siguienteDireccion;
        
        Point cabeza = serpiente.get(0);
        Point nuevaCabeza = new Point(cabeza);
        
        switch (direccion) {
            case 0: nuevaCabeza.y--; break;
            case 1: nuevaCabeza.x++; break;
            case 2: nuevaCabeza.y++; break;
            case 3: nuevaCabeza.x--; break;
        }
        
        if (nuevaCabeza.x < 0) nuevaCabeza.x = TAMANO_TABLERO - 1;
        if (nuevaCabeza.x >= TAMANO_TABLERO) nuevaCabeza.x = 0;
        if (nuevaCabeza.y < 0) nuevaCabeza.y = TAMANO_TABLERO - 1;
        if (nuevaCabeza.y >= TAMANO_TABLERO) nuevaCabeza.y = 0;
        
        if (serpiente.contains(nuevaCabeza)) {
            gameOver();
            return;
        }
        
        serpiente.add(0, nuevaCabeza);
        
        if (nuevaCabeza.equals(comida)) {
            puntuacion += 100;
            generarComidaYTrampas();
            
            if (serpiente.size() >= 15) {
                subirNivel();
            }
        } else if (trampas.contains(nuevaCabeza)) {
            puntuacion -= 200;
            if (puntuacion < 0) puntuacion = 0;
            
            if (serpiente.size() > 1) {
                serpiente.remove(serpiente.size() - 1);
            }
            
            if (serpiente.size() <= 1) {
                serpiente.remove(serpiente.size() - 1);
                gameOver();
                return;
            } else {
                serpiente.remove(serpiente.size() - 1);
            }
            
            generarComidaYTrampas();
        } else {
            serpiente.remove(serpiente.size() - 1);
        }
    }
    
    private void subirNivel() {
        nivel++;
        puntuacion += 1000;
        
        Point cabeza = serpiente.get(0);
        serpiente.clear();
        
        for (int i = 0; i < 5; i++) {
            Point p = new Point(cabeza.x - i, cabeza.y);
            if (p.x < 0) p.x += TAMANO_TABLERO;
            serpiente.add(p);
        }
        
        timer.stop();
        int velocidad = Math.max(50, velocidadInicial - (nivel - 1) * 10);
        timer = new Timer(velocidad, this);
        timer.start();
        
        generarComidaYTrampas();
    }
    
    private void gameOver() {
        juegoActivo = false;
        timer.stop();
        
        rankings.add(new Jugador(nombreJugador, puntuacion, nivel));
        Collections.sort(rankings);
        guardarRankings();
        
        estadoActual = Estado.GAME_OVER;
    }
    
    @Override
    public void keyPressed(KeyEvent e) {
        int tecla = e.getKeyCode();
        
        // Guardar partida con F
        if (tecla == KeyEvent.VK_F && estadoActual == Estado.JUGANDO && juegoActivo) {
            String[] opciones = {"Slot 1", "Slot 2", "Slot 3", "Cancelar"};
            int seleccion = JOptionPane.showOptionDialog(this,
                "Selecciona un slot para guardar:",
                "Guardar Partida",
                JOptionPane.DEFAULT_OPTION,
                JOptionPane.QUESTION_MESSAGE,
                null,
                opciones,
                opciones[0]);
            
            if (seleccion >= 0 && seleccion < 3) {
                guardarPartida(seleccion + 1);
            }
            return;
        }
        
        if (estadoActual == Estado.JUGANDO && juegoActivo && movimientoProcesado) {
            int nuevaDireccion = siguienteDireccion;
            
            if (tecla == KeyEvent.VK_W || tecla == KeyEvent.VK_UP) {
                nuevaDireccion = 0;
            } else if (tecla == KeyEvent.VK_D || tecla == KeyEvent.VK_RIGHT) {
                nuevaDireccion = 1;
            } else if (tecla == KeyEvent.VK_S || tecla == KeyEvent.VK_DOWN) {
                nuevaDireccion = 2;
            } else if (tecla == KeyEvent.VK_A || tecla == KeyEvent.VK_LEFT) {
                nuevaDireccion = 3;
            }
            
            if ((nuevaDireccion == 0 && direccion != 2) ||
                (nuevaDireccion == 1 && direccion != 3) ||
                (nuevaDireccion == 2 && direccion != 0) ||
                (nuevaDireccion == 3 && direccion != 1)) {
                siguienteDireccion = nuevaDireccion;
                movimientoProcesado = false;
            }
        }
    }
    
    @Override
    public void keyTyped(KeyEvent e) {}
    
    @Override
    public void keyReleased(KeyEvent e) {}
    
    public static void main(String[] args) {
        JFrame frame = new JFrame("Snake Game - Infinito");
        SnakeGame juego = new SnakeGame();
        frame.add(juego);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}