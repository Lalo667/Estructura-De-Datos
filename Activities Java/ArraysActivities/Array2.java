import java.util.Scanner; // Importa una clase y siemrpre va al inicio del archivo

public class Array2 { // El nombre de la clase debe de ser igual al nombre del archivo (MUY IMPORTANTE PARA COMPILAR)
    public static void main(String[] args) { // Metodo principal, siempre debe de estar
        Scanner sc = new Scanner(System.in); // Ayuda a leer los datos enteros desde consola

        System.out.print("Ingrese el tamaño del arreglo: "); //Escribe en la consola "Ingrese el tamaño del arreglo: "
        int n = sc.nextInt(); // Lee un entero desde la consola y lo guarda en la variable n

        int[] matriz = new int[n];  // Aca crea el arreglo del tamaño n

        for (int i = 0; i < n; i++) { // Ciclo for que se repite n veces osea el tamaño del arreglo y va sumandose
            System.out.print("Dame el valor " + (i + 1) + ": "); //Escribe en la consola "Dame el valor i+1: "
            matriz[i] = sc.nextInt(); // Lee un arreglo desde la consola y lo guarda en la variable matriz en la posicion i q va conforme el ciclo
        }

        //Este bloque de codigo muestra el arreglo en la terminal
        System.out.println("\nLos valores que ingresaste son:"); //Escribe en la consola "Los valores que ingresaste son:"
        for (int i = 0; i < n; i++) { // Ciclo for que se repite n veces osea el tamaño del arreglo y va sumandose
            System.out.print(matriz[i] + " "); //Escribe en la consola el valor del arreglo en la posicion i
        }

        System.out.println(); // Salto de linea

        System.out.print("\nDame la posicion donde insertar (1-" + n + "): "); //Escribe en la consola "Dame la posicion donde insertar (1-n): "
        int pos = sc.nextInt(); // Ahora c le pide un entero desde la consola y lo guarda en la variable pos
        System.out.print("Dame el nuevo valor: "); // Escribe en la consola "Dame el nuevo valor: "
        int valor = sc.nextInt(); // Lee un entero desde la consola y lo guarda en la variable valor

        if (pos >= 1 && pos <= n) { // si la posciscion es mayor o igual que uno y pos es menor o igual que n entonces
            // Este for mueve los elementos a la derecha desde la posicion dada
            for (int i = n - 1; i > pos - 1; i--) {  // ciclo for que empieza en n-1 por ejemplo si n es 5 empieza en 4  y se va restando hasta que i sea mayor que pos-1
                matriz[i] = matriz[i - 1]; // mueve el valor de la posicion i-1 a la posicion i
            }

            matriz[pos - 1] = valor; // sucede porq el arreglo inicia desde el 0
            System.out.println("\nValor insertado correctamente."); //Escribe en la consola "Valor insertado correctamente."
        } else { // si no
            System.out.println("Error: Posición inválida.");    //Escribe en la consola "Error: Posición inválida."
        }

        System.out.println("\nLos valores actualizados son:");
        for (int i = 0; i < n; i++) { // Ciclo for que se repite n veces osea el tamaño del arreglo y va sumandose
            System.out.print(matriz[i] + " "); //Escribe en la consola el valor del arreglo en la posicion i
        }
        System.out.println(); // Salto de linea

        sc.close(); // Cierra el scanner siempre al final
    }
}
