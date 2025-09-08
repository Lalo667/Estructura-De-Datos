using System;

class Array1Activitie1
{
   static void Main() 
    {
        int[] precios = new int[3]; // c declara el arreglo

        for (int i = 0; i < 3; i++) //el int c iniciliaza en 0 y si es menor a 3 se incrementa en 1
        {
            Console.Write($"Dame el precio {i + 1}: "); // c escribe en la consolsa
            precios[i] = Convert.ToInt32(Console.ReadLine()); //el valor lo convierte en un entero y lo guarda en el arreglo
        }

        Console.WriteLine("\nLos precios que ingresaste son:"); //c escribe en la consola
        for (int i = 0; i < 3; i++) //el int c iniciliaza en 0 y si es menor a 3 se incrementa en 1
        {
            Console.WriteLine(precios[i]); //c escribe en la consola el valor guardado en el arreglo
        }

        Console.WriteLine("\nPresiona cualquier tecla para salir...");
        Console.ReadKey(); // siempre va este codigo para que no se cierre la consola
    }
 }

