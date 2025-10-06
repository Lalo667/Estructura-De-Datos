package ElGatitoProyecto;
    import java.io.IOException; //esto me permite manejar errores de entrada y salida como por ejemplo al leer un archivo que no existe y no se puede abrir
    import javax.swing.JOptionPane; //sta me permite mostrar ventanas emergentes
    
    public class ElGatitoProyecto {

    // Los métodos público y estático, significa que puede llamarse desde cualquier parte de la clase sin crear un objeto.

    public static char[][] matriz_cat = new char [3][3]; //Marca un arreglo de 3x3 que estara vacia al inicio
    public static String Name1 = " "; // Marca las variables que guardaran los nombres de los jugadores
    public static String Name2 = " "; // Marca las variables que guardaran los nombres de los jugadores
    public static char JugadaActual = 'X'; // Marca la variable que guardara el turno actual, inicia con X

// Evita que los jugadores pongan su ficha en una casilla que ya esté ocupada.
 public static boolean CasillaVacia(int posicion) { //Devuelve true si la posicion esta vacia y false si no lo esta
        switch(posicion){ //verifica si la posicion ingresada esta vacia
            case 1: return matriz_cat[0][0] == '-';    
            case 2: return matriz_cat[0][1] == '-';
            case 3: return matriz_cat[0][2] == '-';
            case 4: return matriz_cat[1][0] == '-';
            case 5: return matriz_cat[1][1] == '-';
            case 6: return matriz_cat[1][2] == '-';
            case 7: return matriz_cat[2][0] == '-';
            case 8: return matriz_cat[2][1] == '-';
            case 9: return matriz_cat[2][2] == '-';
            default: return false; //si no es ninguna de las opciones anteriores, devuelve false
        }
    }

    public static void ImprimirPosiciones(){
        int pos = 1; // Contador, en donde las enumeraciones de las posiciones van del 1 al 9
        if (JugadaActual == 'X'){ // Indica de quien es el turno, por defecto inicia con X, pero si ya se cambio a O, entonces es turno de O
            System.out.println(" " + Name1 + "Elija una posicion sin usar, porfas");
        } else {
            System.out.println(" " + Name2 + "Elija una posicion sin usar, porfas");
        }
        //Su funcion es recorrer la matriz e imprimir su estado actual
        for (int i = 0; i < matriz_cat.length; i++){  // i es igual a posicion 0, y mientras i que equivale a la posicion 0 sea menor que la longitud de la matriz (3), se incrementa i en 1
            for (int j = 0; j < matriz_cat.length; j++){ // j es igual a posicion 0, y mientras j que equivale a la posicion 0 sea menor que la longitud de la matriz (3), se incrementa j en 1
                if (matriz_cat[i][j] == 'X' || matriz_cat[i][j] == 'O'){
                    System.out.print(" " + matriz_cat[i][j]); // Si la posicion de la matriz ya tiene X o O, imprime X o O
                } else {
                    System.out.print(" " + pos); // Si la posicion de la matriz esta vacia, imprime el numero de la posicion y el "" es para que haya un espacio entre los numeros
                }
                pos++; // Incrementa el contador de posiciones en 1
            }

            System.out.println(); // Imprime un salto de linea para que la siguiente fila de la matriz se imprima en una nueva linea
        }
    }

    public static boolean GanarPorRenglon(char caracter){ //Aca recibe el simbolo que quiere verificar si gano
        for (int i = 0; i < matriz_cat.length; i++){ // Recorre las filas de la matriz sumandole 1 a i
            if (matriz_cat[i][0] == caracter && matriz_cat[i][1] == caracter && matriz_cat[i][2] == caracter){ // Verifica si en la fila i, las 3 posiciones son iguales al simbolo que se esta verificando
                return true; // Si las 3 posiciones son iguales al simbolo, devuelve true
            }
        }
        return false; // Si no se cumple la condicion, devuelve false
    }

    public static boolean ThereisaSpace(){ // Verifica si hay espacios vacios en la matriz, si los hay devuelve true, si no devuelve false
        for (char[] cat1 : matriz_cat) { // Recorre las filas de la matriz
            for (int j = 0; j < matriz_cat.length; j++) { // Recorre las columnas de la matriz
                if (cat1[j] == '-') { // Verifica si la posicion en la fila i y columna j es igual a '-'
                    return true; // Si la posicion es igual a '-', devuelve true

                }
            }
        }
        return false;
    }

    public static boolean GanarPorColumna(char caracter){ //Aca recibe el simbolo que quiere verificar si gano
        for (int i = 0; i < matriz_cat.length; i++){ // Recorre las columnas de la matriz sumandole 1 a i
            if (matriz_cat[0][i] == caracter && matriz_cat[1][i] == caracter && matriz_cat[2][i] == caracter){ // Verifica si en la columna i, las 3 posiciones son iguales al simbolo que se esta verificando
                return true; // Si las 3 posiciones son iguales al simbolo, devuelve true
            }
        }
        return false; // Si no se cumple la condicion, devuelve false
    }

    public static boolean GanarPorDiagonal(char caracter){ //Aca recibe el simbolo que quiere verificar si gano
        if (matriz_cat[0][0] == caracter && matriz_cat[1][1] == caracter && matriz_cat[2][2] == caracter){ // Verifica si en la diagonal de izquierda a derecha, las 3 posiciones son iguales al simbolo que se esta verificando
            return true;
        }
        if (matriz_cat[0][2] == caracter && matriz_cat[1][1] == caracter && matriz_cat[2][0] == caracter){ // Verifica si en la diagonal de derecha a izquierda, las 3 posiciones son iguales al simbolo que se esta verificando
            return true;
        }
        return false; // Si no se cumple la condicion, devuelve false
    }

    public static boolean ThereisaWinner(char caracter) { //Aca recibe el simbolo que quiere verificar si gano
        if (GanarPorRenglon(caracter)) { //
            return true;
        }
        if (GanarPorColumna(caracter)) {
            return true;
        }
        if (GanarPorDiagonal(caracter)) {
            return true;
        }

        return false;
    }

    public static void RegistrarPlay(char caracter) throws IOException{ //
        boolean salir = false; // Marca la variable que indica si el jugador ha ingresado una posicion valida
        String Entrada; // Marca la variable que guardara la entrada del jugador
        int posicion; // Marca la variable que guardara la posicion ingresada por el jugador
        do{
            ImprimirPosiciones(); // aca c llama la matriz para mostrar su estado actual
            if (JugadaActual == 'X'){
                Entrada = JOptionPane.showInputDialog("Turno de " + Name1 + " Ingrese la posicion donde desea colocar su " + caracter);
            } else {
                Entrada = JOptionPane.showInputDialog("Turno de " + Name2 + " Ingrese la posicion donde desea colocar su " + caracter);
            }

                posicion = Integer.parseInt(Entrada); // c tiene q convertir a numero porque a la hora de ingresar la posicion, se ingresa como texto en el cuadro de dialogo
                //sta llama la funcion que verifica si la posicion ingresada esta vacia
                if (CasillaVacia(posicion)){ // Verifica si la posicion ingresada esta vacia
                    switch(posicion){ // Si la posicion esta vacia, coloca el simbolo en la posicion ingresada
                    //Coloca el caracter respectivo en la posicion ingresada
                    case 1: 
                    matriz_cat[0][0] = caracter;
                    break;
                    case 2:
                    matriz_cat[0][1] = caracter;
                    break;
                    case 3:
                    matriz_cat[0][2] = caracter;
                    break;
                    case 4:
                    matriz_cat[1][0] = caracter;
                    break;
                    case 5:
                    matriz_cat[1][1] = caracter;
                    break;
                    case 6:
                    matriz_cat[1][2] = caracter;
                    break;
                    case 7:
                    matriz_cat[2][0] = caracter;
                    break;
                    case 8:
                    matriz_cat[2][1] = caracter;
                    break;
                    case 9:
                    matriz_cat[2][2] = caracter;
                    break;
                }
                salir = true; // Si se ha colocado el simbolo en la posicion ingresada, marca salir como true para salir del ciclo
            } else {
                // Si la posicion ingresada no esta vacia, muestra un mensaje de error
            JOptionPane.showMessageDialog(null, "ya sta ocupada, porfa elija otra");
         }
        } while (!salir); // hace el ciclo hasta que el jugador ingrese una posicion valida
    }
    
    public static void inicioTablero() {
        for (int i = 0; i < matriz_cat.length; i++) {
            for (int j = 0; j < matriz_cat.length; j++) {
                matriz_cat[i][j] = '-'; // Inicializa todas las posiciones de la matriz con '-' y recorre la matriz, poniendo '-' en cada posicion

            }
        }
    }
    
     public static void nombres() { // Solicita los nombres de los jugadores al inicio del juego
        Name1 = JOptionPane.showInputDialog("Ingrese el nombre del jugador 1: ");
        Name2 = JOptionPane.showInputDialog("Ingrese el nombre del jugador 2: ");
    }

    public static void main(String[] args) throws IOException { // El main es el metodo principal, donde se ejecuta el programa

    boolean endit = false; // Marca la variable que indica si el juego ha terminado
    inicioTablero(); // Llama al metodo que inicializa la matriz
    nombres(); // pide los nombres de los jugadores
    JOptionPane.showMessageDialog(null, "El jugador 1 es: " + Name1 + " y su simbolo es X" 
    + "\n El jugador 2 es: " + Name2 + " y su simbolo es O"
    + "\n Mucha Suerte!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"); //Esto muestra un mensaje con los nombres de los jugadores y sus simbolos

    do { // Hace el ciclo del juego hasta que haya un ganador o se llene la matriz
        RegistrarPlay(JugadaActual); // La jugada Actual es x o O, dependiendo de quien este jugando
        if (ThereisaWinner(JugadaActual)) { // Verifica si el jugador que acaba de jugar ha ganado
          if (JugadaActual == 'X'){ // Si el jugador que acaba de jugar es X, entonces X es el ganador
            ImprimirPosiciones(); // Imprime la matriz con el estado actual

            JOptionPane.showMessageDialog(null, "Felicidades " + Name1 + " Ganaste!!!!"); // Muestra un mensaje de felicitacion al jugador que gano
            int pos = 1; 
                for (int i = 0; i < matriz_cat.length; i++) {
                    for (int j = 0; j < matriz_cat.length; j++) {
                        if (matriz_cat[i][j] == 'X' || matriz_cat[i][j] == 'O') {
                            System.out.print(" " + matriz_cat[i][j]);
                        } else {
                            System.out.print(" " + pos);
                        }
                        pos++;
                    }
                    System.out.println();
                }
                endit = true;
                
            } else {
                
            JOptionPane.showMessageDialog(null, "Felicidades " + Name2 + " Ganaste!!!!");
            int pos = 1;
                for (int i = 0; i < matriz_cat.length; i++) { // Esto imprime el tablero final, de quien es el ganador, aunque esta 2 veces el codigo, esto con una funcion c podria arreglar
                    for (int j = 0; j < matriz_cat.length; j++) {
                        if (matriz_cat[i][j] == 'X' || matriz_cat[i][j] == 'O') {
                            System.out.print(" " + matriz_cat[i][j]);
                                 } else {
                                System.out.print(" " + pos);
                            }
                         pos++;
                        }
                        System.out.println();
                    }
                    endit = true;
                }
            } else {
                if (!ThereisaSpace()) { //Es una negacion, entonces esto entra en el momento en el que ya no hay espacios los cuales hay que llenar
                    JOptionPane.showMessageDialog(null, "Ya no hay casillas, es empate");
                    endit = true;
                } else if (JugadaActual == 'X'){ 
                        JugadaActual = 'O';
                } else {
                        JugadaActual = 'X';
                    }    
                }
            } while (!endit); // Este bloque se ejecuta, hasta que ya no exista espacios que llenar o alguien halla ganado
        }
    }