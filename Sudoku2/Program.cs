using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SudokuGame
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            SudokuGame juego = new SudokuGame();
            juego.MenuPrincipal();
        }
    }

    class PartidaGuardada
    {
        public string NombreJugador { get; set; }
        public int NivelActual { get; set; }
        public int PuntuacionTotal { get; set; }
        public List<string> DificultadesDesbloqueadas { get; set; }
        public DateTime FechaGuardado { get; set; }

        public PartidaGuardada()
        {
            NombreJugador = "";
            NivelActual = 1;
            PuntuacionTotal = 0;
            DificultadesDesbloqueadas = new List<string>();
            FechaGuardado = DateTime.Now;
        }
    }

    class SudokuGame
    {
        private int vidas;
        private int nivelActual;
        private int maxNiveles;
        private List<string> dificultadesDesbloqueadas;
        private string dificultadActual;
        private int[,] tablero;
        private int[,] solucion;
        private int[,] tableroOriginal;
        private int posX;
        private int posY;
        private int puntuacion;
        private int puntuacionTotal;
        private HashSet<int> filasCompletadas;
        private HashSet<int> columnasCompletadas;
        private HashSet<int> cuadrosCompletados;
        private Dictionary<string, int> celdasEliminar;
        private string nombreJugador;
        private int slotActual;

        private const int PUNTOS_FILA = 50;
        private const int PUNTOS_COLUMNA = 50;
        private const int PUNTOS_CUADRO = 50;
        private const int PUNTOS_ERROR = -10;
        private const int MAX_SLOTS = 3;

        public SudokuGame()
        {
            vidas = 3;
            nivelActual = 1;
            maxNiveles = 20;
            dificultadesDesbloqueadas = new List<string>();
            dificultadesDesbloqueadas.Add("Muy Fácil");
            dificultadActual = "";
            tablero = new int[9, 9];
            solucion = new int[9, 9];
            tableroOriginal = new int[9, 9];
            posX = 0;
            posY = 0;
            puntuacion = 0;
            puntuacionTotal = 0;
            filasCompletadas = new HashSet<int>();
            columnasCompletadas = new HashSet<int>();
            cuadrosCompletados = new HashSet<int>();
            nombreJugador = "";
            slotActual = -1;
            
            celdasEliminar = new Dictionary<string, int>();
            celdasEliminar.Add("Muy Fácil", 25);
            celdasEliminar.Add("Fácil", 35);
            celdasEliminar.Add("Normal", 45);
            celdasEliminar.Add("Difícil", 50);
            celdasEliminar.Add("Maestro", 55);
        }

        private void GenerarTableroBase()
        {
            int[,] basePattern = new int[9, 9];
            int[] nums = new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            Random rand = new Random();
            nums = nums.OrderBy(x => rand.Next()).ToArray();

            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    basePattern[i, j] = nums[(3 * (i % 3) + i / 3 + j) % 9];
                }
            }

            solucion = MezclarTablero(basePattern, rand);
        }

        private int[,] MezclarTablero(int[,] tableroEntrada, Random rand)
        {
            int[,] resultado = (int[,])tableroEntrada.Clone();

            for (int banda = 0; banda < 3; banda++)
            {
                int[] orden = new int[] { 0, 1, 2 };
                orden = orden.OrderBy(x => rand.Next()).ToArray();
                int[,] temp = (int[,])resultado.Clone();
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 9; j++)
                    {
                        resultado[banda * 3 + i, j] = temp[banda * 3 + orden[i], j];
                    }
                }
            }

            for (int banda = 0; banda < 3; banda++)
            {
                int[] orden = new int[] { 0, 1, 2 };
                orden = orden.OrderBy(x => rand.Next()).ToArray();
                int[,] temp = (int[,])resultado.Clone();
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        resultado[i, banda * 3 + j] = temp[i, banda * 3 + orden[j]];
                    }
                }
            }

            return resultado;
        }

        private void CrearPuzzle(string dificultad)
        {
            GenerarTableroBase();
            Array.Copy(solucion, tablero, solucion.Length);

            int celdasAEliminar = celdasEliminar[dificultad];
            int celdasEliminadas = 0;
            Random rand = new Random();

            while (celdasEliminadas < celdasAEliminar)
            {
                int fila = rand.Next(9);
                int col = rand.Next(9);

                if (tablero[fila, col] != 0)
                {
                    tablero[fila, col] = 0;
                    celdasEliminadas++;
                }
            }

            Array.Copy(tablero, tableroOriginal, tablero.Length);
            puntuacion = 0;
            filasCompletadas.Clear();
            columnasCompletadas.Clear();
            cuadrosCompletados.Clear();
        }

        private void ImprimirTablero()
        {
            Console.Clear();
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("  SUDOKU - " + nombreJugador);
            Console.WriteLine("  Nivel " + nivelActual + "/" + maxNiveles + " | Dificultad: " + dificultadActual);
            Console.WriteLine("  Vidas: " + new string('♥', vidas));
            Console.WriteLine("  Puntuación: " + puntuacion + " pts | Total: " + puntuacionTotal + " pts");
            Console.WriteLine(new string('=', 50) + "\n");

            Console.WriteLine("     0   1   2   3   4   5   6   7   8");
            Console.WriteLine("   ╔═══════════╦═══════════╦═══════════╗");

            for (int i = 0; i < 9; i++)
            {
                if (i == 3 || i == 6)
                {
                    Console.WriteLine("   ╠═══════════╬═══════════╬═══════════╣");
                }

                Console.Write(" " + i + " ║");

                for (int j = 0; j < 9; j++)
                {
                    if (j == 3 || j == 6)
                    {
                        Console.Write("║");
                    }

                    if (i == posY && j == posX)
                    {
                        if (tablero[i, j] == 0)
                        {
                            Console.Write(" ▢ ");
                        }
                        else
                        {
                            Console.ForegroundColor = ConsoleColor.Yellow;
                            Console.Write("[" + tablero[i, j] + "]");
                            Console.ResetColor();
                        }
                    }
                    else
                    {
                        if (tablero[i, j] == 0)
                        {
                            Console.Write(" · ");
                        }
                        else if (tableroOriginal[i, j] != 0)
                        {
                            Console.ForegroundColor = ConsoleColor.Cyan;
                            Console.Write(" " + tablero[i, j] + " ");
                            Console.ResetColor();
                        }
                        else
                        {
                            Console.ForegroundColor = ConsoleColor.Green;
                            Console.Write(" " + tablero[i, j] + " ");
                            Console.ResetColor();
                        }
                    }
                }
                Console.WriteLine("║");
            }

            Console.WriteLine("   ╚═══════════╩═══════════╩═══════════╝");
            Console.WriteLine("\n Filas completadas: " + filasCompletadas.Count + "/9");
            Console.WriteLine(" Columnas completadas: " + columnasCompletadas.Count + "/9");
            Console.WriteLine(" Cuadros completados: " + cuadrosCompletados.Count + "/9");
            Console.WriteLine("\n Controles:");
            Console.WriteLine(" Flechas (W/S/A/D): Mover | 1-9: Numero");
            Console.WriteLine(" K: Skip | R: Reiniciar | ESC: Menu Principal");
        }

        private bool EsValido(int fila, int col, int num)
        {
            for (int j = 0; j < 9; j++)
            {
                if (tablero[fila, j] == num && j != col)
                {
                    return false;
                }
            }

            for (int i = 0; i < 9; i++)
            {
                if (tablero[i, col] == num && i != fila)
                {
                    return false;
                }
            }

            int inicioFila = (fila / 3) * 3;
            int inicioCol = (col / 3) * 3;

            for (int i = inicioFila; i < inicioFila + 3; i++)
            {
                for (int j = inicioCol; j < inicioCol + 3; j++)
                {
                    if (tablero[i, j] == num && (i != fila || j != col))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        private void VerificarCompletados()
        {
            for (int i = 0; i < 9; i++)
            {
                if (!filasCompletadas.Contains(i) && FilaCompleta(i))
                {
                    filasCompletadas.Add(i);
                    puntuacion = puntuacion + PUNTOS_FILA;
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("\n¡Fila " + i + " completada! +" + PUNTOS_FILA + " puntos");
                    Console.ResetColor();
                    System.Threading.Thread.Sleep(800);
                }
            }

            for (int j = 0; j < 9; j++)
            {
                if (!columnasCompletadas.Contains(j) && ColumnaCompleta(j))
                {
                    columnasCompletadas.Add(j);
                    puntuacion = puntuacion + PUNTOS_COLUMNA;
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("\n¡Columna " + j + " completada! +" + PUNTOS_COLUMNA + " puntos");
                    Console.ResetColor();
                    System.Threading.Thread.Sleep(800);
                }
            }

            for (int cuadro = 0; cuadro < 9; cuadro++)
            {
                if (!cuadrosCompletados.Contains(cuadro) && CuadroCompleto(cuadro))
                {
                    cuadrosCompletados.Add(cuadro);
                    puntuacion = puntuacion + PUNTOS_CUADRO;
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("\n¡Cuadro " + (cuadro + 1) + " completado! +" + PUNTOS_CUADRO + " puntos");
                    Console.ResetColor();
                    System.Threading.Thread.Sleep(800);
                }
            }
        }

        private bool FilaCompleta(int fila)
        {
            for (int j = 0; j < 9; j++)
            {
                if (tablero[fila, j] == 0 || tablero[fila, j] != solucion[fila, j])
                {
                    return false;
                }
            }
            return true;
        }

        private bool ColumnaCompleta(int col)
        {
            for (int i = 0; i < 9; i++)
            {
                if (tablero[i, col] == 0 || tablero[i, col] != solucion[i, col])
                {
                    return false;
                }
            }
            return true;
        }

        private bool CuadroCompleto(int cuadro)
        {
            int inicioFila = (cuadro / 3) * 3;
            int inicioCol = (cuadro % 3) * 3;

            for (int i = inicioFila; i < inicioFila + 3; i++)
            {
                for (int j = inicioCol; j < inicioCol + 3; j++)
                {
                    if (tablero[i, j] == 0 || tablero[i, j] != solucion[i, j])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        private bool TableroCompleto()
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (tablero[i, j] == 0 || tablero[i, j] != solucion[i, j])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        private void DesbloquearDificultad()
        {
            string[] dificultades = new string[] { "Muy Fácil", "Fácil", "Normal", "Difícil", "Maestro" };
            int nivelDesbloqueado = (nivelActual - 1) / 5;

            if (nivelDesbloqueado < dificultades.Length)
            {
                string dif = dificultades[nivelDesbloqueado];
                if (!dificultadesDesbloqueadas.Contains(dif))
                {
                    dificultadesDesbloqueadas.Add(dif);
                    Console.ForegroundColor = ConsoleColor.Magenta;
                    Console.WriteLine("\n¡Nueva dificultad desbloqueada: " + dif + "!");
                    Console.ResetColor();
                    Console.WriteLine("Presiona Enter para continuar...");
                    Console.ReadLine();
                }
            }
        }

        private string ObtenerDificultadNivel()
        {
            string[] dificultades = new string[] { "Muy Fácil", "Fácil", "Normal", "Difícil", "Maestro" };
            int indice = Math.Min((nivelActual - 1) / 5, dificultades.Length - 1);
            return dificultades[indice];
        }

        private string JugarNivel()
        {
            dificultadActual = ObtenerDificultadNivel();
            CrearPuzzle(dificultadActual);
            posX = 0;
            posY = 0;

            while (true)
            {
                ImprimirTablero();

                if (TableroCompleto())
                {
                    puntuacionTotal = puntuacionTotal + puntuacion;
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.WriteLine("\n¡Nivel completado!");
                    Console.WriteLine("Puntos obtenidos: " + puntuacion);
                    Console.WriteLine("Puntuación total: " + puntuacionTotal);
                    Console.ResetColor();
                    nivelActual++;
                    DesbloquearDificultad();

                    if (nivelActual > maxNiveles)
                    {
                        Console.ForegroundColor = ConsoleColor.Green;
                        Console.WriteLine("\n¡FELICIDADES! ¡Has completado todos los niveles!");
                        Console.WriteLine("Puntuación final: " + puntuacionTotal + " puntos");
                        Console.ResetColor();
                        GuardarProgreso();
                        Console.WriteLine("Presiona Enter para volver al menú...");
                        Console.ReadLine();
                        return "menu";
                    }

                    GuardarProgreso();
                    Console.WriteLine("Presiona Enter para continuar...");
                    Console.ReadLine();
                    return "continuar";
                }

                Console.Write("\nAcción: ");
                ConsoleKeyInfo tecla = Console.ReadKey(true);

                if (tecla.Key == ConsoleKey.UpArrow || tecla.Key == ConsoleKey.W)
                {
                    posY = (posY - 1 + 9) % 9;
                }
                else if (tecla.Key == ConsoleKey.DownArrow || tecla.Key == ConsoleKey.S)
                {
                    posY = (posY + 1) % 9;
                }
                else if (tecla.Key == ConsoleKey.LeftArrow || tecla.Key == ConsoleKey.A)
                {
                    posX = (posX - 1 + 9) % 9;
                }
                else if (tecla.Key == ConsoleKey.RightArrow || tecla.Key == ConsoleKey.D)
                {
                    posX = (posX + 1) % 9;
                }
                else if (tecla.Key == ConsoleKey.Escape)
                {
                    Console.WriteLine("\n¿Guardar progreso antes de salir? (S/N)");
                    ConsoleKeyInfo guardar = Console.ReadKey(true);
                    if (guardar.Key == ConsoleKey.S)
                    {
                        SeleccionarSlotGuardado();
                    }
                    return "menu";
                }
                else if (tecla.Key == ConsoleKey.R)
                {
                    Array.Copy(tableroOriginal, tablero, tableroOriginal.Length);
                    posX = 0;
                    posY = 0;
                    puntuacion = 0;
                    filasCompletadas.Clear();
                    columnasCompletadas.Clear();
                    cuadrosCompletados.Clear();
                }
                else if (tecla.Key == ConsoleKey.K)
                {
                    string resultado = SkipNivel();
                    if (resultado == "continuar")
                    {
                        return resultado;
                    }
                }
                else if (tecla.KeyChar >= '1' && tecla.KeyChar <= '9')
                {
                    ColocarNumero(tecla.KeyChar - '0');
                }
            }
        }

        private void ColocarNumero(int num)
        {
            if (tableroOriginal[posY, posX] == 0)
            {
                if (EsValido(posY, posX, num) && num == solucion[posY, posX])
                {
                    tablero[posY, posX] = num;
                    VerificarCompletados();
                }
                else
                {
                    vidas--;
                    puntuacion = puntuacion + PUNTOS_ERROR;
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("\n❌ Movimiento inválido! " + PUNTOS_ERROR + " puntos");
                    Console.WriteLine("Vidas restantes: " + vidas);
                    Console.ResetColor();
                    Console.WriteLine("Presiona Enter para continuar...");
                    Console.ReadLine();

                    if (vidas <= 0)
                    {
                        MostrarMenuGameOver();
                    }
                }
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("\n⚠️ No puedes modificar los números originales!");
                Console.ResetColor();
                Console.WriteLine("Presiona Enter para continuar...");
                Console.ReadLine();
            }
        }

        private void MostrarMenuGameOver()
        {
            Console.Clear();
            Console.ForegroundColor = ConsoleColor.DarkRed;
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("                  ¡GAME OVER!");
            Console.WriteLine(new string('=', 50));
            Console.ResetColor();
            Console.WriteLine("\n  Has perdido todas tus vidas.");
            Console.WriteLine("  Nivel actual: " + nivelActual);
            Console.WriteLine("  Puntuación total: " + puntuacionTotal);
            Console.WriteLine("\n  ¿Qué deseas hacer?");
            Console.WriteLine("\n  1. Continuar con la puntuación actual (sin vidas)");
            Console.WriteLine("  2. Resetear puntuación y recuperar vidas");
            Console.WriteLine("  3. Volver al menú principal");
            Console.WriteLine("\n" + new string('=', 50));
            Console.Write("\nElige una opción: ");

            while (true)
            {
                ConsoleKeyInfo tecla = Console.ReadKey(true);
                
                if (tecla.KeyChar == '1')
                {
                    vidas = 3;
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.WriteLine("\n\n✓ Vidas restauradas. ¡Continúa con tu puntuación!");
                    Console.ResetColor();
                    Console.WriteLine("Presiona Enter para continuar...");
                    Console.ReadLine();
                    break;
                }
                else if (tecla.KeyChar == '2')
                {
                    vidas = 3;
                    puntuacionTotal = 0;
                    puntuacion = 0;
                    Console.ForegroundColor = ConsoleColor.Cyan;
                    Console.WriteLine("\n\n✓ Puntuación reseteada y vidas restauradas.");
                    Console.ResetColor();
                    Console.WriteLine("Presiona Enter para continuar...");
                    Console.ReadLine();
                    break;
                }
                else if (tecla.KeyChar == '3')
                {
                    throw new Exception("VolvermMenu");
                }
            }
        }

        private string SkipNivel()
        {
            Console.WriteLine("\n¿Seguro que quieres skipear este nivel? (S/N)");
            ConsoleKeyInfo respuesta = Console.ReadKey(true);
            if (respuesta.Key == ConsoleKey.S)
            {
                nivelActual++;
                DesbloquearDificultad();
                GuardarProgreso();
                return "continuar";
            }
            return "jugar";
        }

        private void SeleccionarSlotGuardado()
        {
            Console.Clear();
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("           📁 SELECCIONAR SLOT DE GUARDADO");
            Console.WriteLine(new string('=', 50));

            PartidaGuardada[] slots = CargarTodosLosSlots();

            for (int i = 0; i < MAX_SLOTS; i++)
            {
                Console.WriteLine("\n  Slot " + (i + 1) + ":");
                if (slots[i] != null && slots[i].NombreJugador != "")
                {
                    Console.WriteLine("    Jugador: " + slots[i].NombreJugador);
                    Console.WriteLine("    Nivel: " + slots[i].NivelActual + "/" + maxNiveles);
                    Console.WriteLine("    Puntuación: " + slots[i].PuntuacionTotal);
                    Console.WriteLine("    Fecha: " + slots[i].FechaGuardado.ToString("dd/MM/yyyy HH:mm"));
                }
                else
                {
                    Console.WriteLine("    [Vacío]");
                }
            }

            Console.WriteLine("\n" + new string('=', 50));
            Console.Write("\nSelecciona slot (1-3) o ESC para cancelar: ");

            while (true)
            {
                ConsoleKeyInfo tecla = Console.ReadKey(true);
                
                if (tecla.Key == ConsoleKey.Escape)
                {
                    return;
                }
                else if (tecla.KeyChar >= '1' && tecla.KeyChar <= '3')
                {
                    int slot = tecla.KeyChar - '1';
                    slotActual = slot;
                    GuardarProgreso();
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("\n\n✓ Partida guardada en Slot " + (slot + 1));
                    Console.ResetColor();
                    Console.WriteLine("Presiona Enter para continuar...");
                    Console.ReadLine();
                    return;
                }
            }
        }

        private void GuardarProgreso()
        {
            if (slotActual < 0 || slotActual >= MAX_SLOTS)
            {
                return;
            }

            try
            {
                PartidaGuardada partida = new PartidaGuardada();
                partida.NombreJugador = nombreJugador;
                partida.NivelActual = nivelActual;
                partida.PuntuacionTotal = puntuacionTotal;
                partida.DificultadesDesbloqueadas = new List<string>(dificultadesDesbloqueadas);
                partida.FechaGuardado = DateTime.Now;

                string nombreArchivo = "sudoku_save_" + slotActual + ".txt";
                StreamWriter sw = new StreamWriter(nombreArchivo);
                sw.WriteLine(partida.NombreJugador);
                sw.WriteLine(partida.NivelActual);
                sw.WriteLine(partida.PuntuacionTotal);
                sw.WriteLine(string.Join(",", partida.DificultadesDesbloqueadas));
                sw.WriteLine(partida.FechaGuardado.ToString("o"));
                sw.Close();
            }
            catch
            {
            }
        }

        private PartidaGuardada[] CargarTodosLosSlots()
        {
            PartidaGuardada[] slots = new PartidaGuardada[MAX_SLOTS];
            
            for (int i = 0; i < MAX_SLOTS; i++)
            {
                try
                {
                    string nombreArchivo = "sudoku_save_" + i + ".txt";
                    if (File.Exists(nombreArchivo))
                    {
                        StreamReader sr = new StreamReader(nombreArchivo);
                        PartidaGuardada partida = new PartidaGuardada();
                        
                        partida.NombreJugador = sr.ReadLine();
                        partida.NivelActual = int.Parse(sr.ReadLine());
                        partida.PuntuacionTotal = int.Parse(sr.ReadLine());
                        partida.DificultadesDesbloqueadas = sr.ReadLine().Split(',').ToList();
                        string fecha = sr.ReadLine();
                        if (fecha != null)
                        {
                            partida.FechaGuardado = DateTime.Parse(fecha);
                        }
                        
                        sr.Close();
                        slots[i] = partida;
                    }
                    else
                    {
                        slots[i] = null;
                    }
                }
                catch
                {
                    slots[i] = null;
                }
            }
            
            return slots;
        }

        private bool CargarProgreso(int slot)
        {
            try
            {
                string nombreArchivo = "sudoku_save_" + slot + ".txt";
                if (!File.Exists(nombreArchivo))
                {
                    return false;
                }

                StreamReader sr = new StreamReader(nombreArchivo);
                nombreJugador = sr.ReadLine();
                nivelActual = int.Parse(sr.ReadLine());
                puntuacionTotal = int.Parse(sr.ReadLine());
                dificultadesDesbloqueadas = sr.ReadLine().Split(',').ToList();
                sr.Close();
                
                vidas = 3;
                slotActual = slot;
                return true;
            }
            catch
            {
                return false;
            }
        }

        private string PedirNombreJugador()
        {
            Console.Clear();
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("              🎮 NUEVA PARTIDA 🎮");
            Console.WriteLine(new string('=', 50));
            Console.Write("\n  Ingresa tu nombre: ");
            string nombre = Console.ReadLine();
            
            if (string.IsNullOrWhiteSpace(nombre))
            {
                nombre = "Jugador";
            }
            
            return nombre.Trim();
        }

        private void MostrarRanking()
        {
            Console.Clear();
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("           🏆 RANKING Y ESTADÍSTICAS 🏆");
            Console.WriteLine(new string('=', 50));
            
            PartidaGuardada[] slots = CargarTodosLosSlots();
            List<PartidaGuardada> partidasValidas = new List<PartidaGuardada>();
            
            for (int i = 0; i < MAX_SLOTS; i++)
            {
                if (slots[i] != null && slots[i].NombreJugador != "")
                {
                    partidasValidas.Add(slots[i]);
                }
            }
            
            partidasValidas = partidasValidas.OrderByDescending(p => p.PuntuacionTotal).ToList();
            
            Console.WriteLine("\n  📊 TOP JUGADORES:");
            if (partidasValidas.Count > 0)
            {
                for (int i = 0; i < partidasValidas.Count; i++)
                {
                    PartidaGuardada p = partidasValidas[i];
                    string medalla = i == 0 ? "🥇" : (i == 1 ? "🥈" : "🥉");
                    Console.WriteLine("\n  " + medalla + " " + (i + 1) + ". " + p.NombreJugador);
                    Console.WriteLine("     Puntuación: " + p.PuntuacionTotal + " pts");
                    Console.WriteLine("     Nivel: " + p.NivelActual + "/" + maxNiveles);
                    Console.WriteLine("     Dificultades: " + p.DificultadesDesbloqueadas.Count + "/5");
                }
            }
            else
            {
                Console.WriteLine("\n  No hay partidas guardadas aún.");
            }
            
            Console.WriteLine("\n\n  Sistema de puntuación:");
            Console.WriteLine("    • Fila completada: +" + PUNTOS_FILA + " puntos");
            Console.WriteLine("    • Columna completada: +" + PUNTOS_COLUMNA + " puntos");
            Console.WriteLine("    • Cuadro 3x3 completado: +" + PUNTOS_CUADRO + " puntos");
            Console.WriteLine("    • Error: " + PUNTOS_ERROR + " puntos");
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("\nPresiona Enter para volver...");
            Console.ReadLine();
        }

        public void MenuPrincipal()
        {
            while (true)
            {
                Console.Clear();
                Console.WriteLine("\n" + new string('=', 50));
                Console.WriteLine("              🎮 SUDOKU GAME 🎮");
                Console.WriteLine(new string('=', 50));
                Console.WriteLine("\n  1. Nueva Partida");
                Console.WriteLine("  2. Cargar Partida");
                Console.WriteLine("  3. Ranking");
                Console.WriteLine("  4. Salir");
                Console.WriteLine("\n" + new string('=', 50));
                Console.Write("\nElige una opción: ");

                ConsoleKeyInfo tecla = Console.ReadKey(true);

                if (tecla.KeyChar == '1')
                {
                    nombreJugador = PedirNombreJugador();
                    nivelActual = 1;
                    vidas = 3;
                    puntuacionTotal = 0;
                    dificultadesDesbloqueadas = new List<string>();
                    dificultadesDesbloqueadas.Add("Muy Fácil");
                    slotActual = -1;
                    
                    try
                    {
                        while (nivelActual <= maxNiveles)
                        {
                            string resultado = JugarNivel();
                            if (resultado == "menu")
                            {
                                break;
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        if (ex.Message == "VolvermMenu")
                        {
                            continue;
                        }
                    }
                }
                else if (tecla.KeyChar == '2')
                {
                    Console.Clear();
                    Console.WriteLine("\n" + new string('=', 50));
                    Console.WriteLine("           📁 CARGAR PARTIDA");
                    Console.WriteLine(new string('=', 50));

                    PartidaGuardada[] slots = CargarTodosLosSlots();
                    bool hayPartidas = false;

                    for (int i = 0; i < MAX_SLOTS; i++)
                    {
                        Console.WriteLine("\n  Slot " + (i + 1) + ":");
                        if (slots[i] != null && slots[i].NombreJugador != "")
                        {
                            hayPartidas = true;
                            Console.WriteLine("    Jugador: " + slots[i].NombreJugador);
                            Console.WriteLine("    Nivel: " + slots[i].NivelActual + "/" + maxNiveles);
                            Console.WriteLine("    Puntuación: " + slots[i].PuntuacionTotal);
                            Console.WriteLine("    Fecha: " + slots[i].FechaGuardado.ToString("dd/MM/yyyy HH:mm"));
                        }
                        else
                        {
                            Console.WriteLine("    [Vacío]");
                        }
                    }

                    if (!hayPartidas)
                    {
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine("\n❌ No hay partidas guardadas.");
                        Console.ResetColor();
                        Console.WriteLine("Presiona Enter para continuar...");
                        Console.ReadLine();
                        continue;
                    }

                    Console.WriteLine("\n" + new string('=', 50));
                    Console.Write("\nSelecciona slot (1-3) o ESC para cancelar: ");

                    bool cargado = false;
                    while (true)
                    {
                        ConsoleKeyInfo teclaSlot = Console.ReadKey(true);
                        
                        if (teclaSlot.Key == ConsoleKey.Escape)
                        {
                            break;
                        }
                        else if (teclaSlot.KeyChar >= '1' && teclaSlot.KeyChar <= '3')
                        {
                            int slot = teclaSlot.KeyChar - '1';
                            if (CargarProgreso(slot))
                            {
                                Console.ForegroundColor = ConsoleColor.Green;
                                Console.WriteLine("\n\n✓ Partida de " + nombreJugador + " cargada exitosamente!");
                                Console.ResetColor();
                                Console.WriteLine("Presiona Enter para continuar...");
                                Console.ReadLine();
                                cargado = true;
                                break;
                            }
                            else
                            {
                                Console.ForegroundColor = ConsoleColor.Red;
                                Console.WriteLine("\n\n❌ No hay partida en este slot.");
                                Console.ResetColor();
                                Console.WriteLine("Presiona Enter para continuar...");
                                Console.ReadLine();
                                break;
                            }
                        }
                    }

                    if (cargado)
                    {
                        try
                        {
                            while (nivelActual <= maxNiveles)
                            {
                                string resultado = JugarNivel();
                                if (resultado == "menu")
                                {
                                    break;
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            if (ex.Message == "VolvermMenu")
                            {
                                continue;
                            }
                        }
                    }
                }
                else if (tecla.KeyChar == '3')
                {
                    MostrarRanking();
                }
                else if (tecla.KeyChar == '4')
                {
                    Console.WriteLine("\n¡Gracias por jugar! 👋");
                    return;
                }
            }
        }
    }
}