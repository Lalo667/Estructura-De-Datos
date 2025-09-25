import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int[] precios = new int[3];

        for (int i = 0; i < 3; i++) {
            System.out.print("Dame el precio " + (i + 1) + ": ");
            precios[i] = sc.nextInt();
        }

        System.out.println("\nLos precios que ingresaste son:");
        for (int i = 0; i < 3; i++) {
            Sytem.out.println(precios[i]);
        }

       
        System.out.println("\nPresiona Enter para salir...");
        try {
            System.in.read();
        } catch (Exception e) {
        }
    }
}


           System.out.print("La suma de los numeros es: " );