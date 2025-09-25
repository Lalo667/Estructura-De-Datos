import java.util.Scanner;

public class Excercise {
        public static void main(String[] args) {
            int suma = 0;
            int may, men;

            Scanner sc = new Scanner(System.in);
            int[] numero = new int[10];

            for (int i = 0; i < 10; i++) {
            System.out.print("Ingrese los numeros " + (i + 1) + ": ");
            numero[i] = sc.nextInt();
            suma += numero[i];
        }

        may = numero[0];
        men = numero[0];

        for(int i = 0; i < 10; i++)
            if (numero[i] > may ){
                may = numero[i];
            }
            if (numero[i] < men) {
                men = numero[i];
            }

            double promedio = suma / 10;

            
             System.out.print("La suma es " + suma);
             System.out.print("El promedio es  " + promedio);
             System.out.print("El mayor es " + may);
             System.out.print("El menor es " + men);
 
            sc.close();
     }
}       

