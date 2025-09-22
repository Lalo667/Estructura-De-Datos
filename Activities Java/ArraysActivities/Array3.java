import java.util.Scanner;

public class Array3 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Ingrese el tamaño del arreglo: ");
        int n = sc.nextInt();

        int[] matriz = new int[n];

        for (int i = 0; i < n; i++) {
            System.out.print("Dame el valor " + (i + 1) + ": ");
            matriz[i] = sc.nextInt();
        }

        System.out.println();

        System.out.print("Ingrese el valor que desea buscar: ");
        int valor = sc.nextInt();

        boolean encontrado = false;
        int posicion = -1; // Guardaremos la posición si se encuentra

        for (int j = 0; j < n; j++) {
            if (matriz[j] == valor) {
                encontrado = true;
                posicion = j; // Guardamos la posición
                break;
            }
        }

        if (encontrado) {
            System.out.println("El valor " + valor + " se encuentra en la matriz en la posición " + (posicion + 1) + ".");
        } else {
            System.out.println("El valor " + valor + " no se encuentra en la matriz.");
        }

        sc.close();
    }
}
