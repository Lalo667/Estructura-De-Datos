using System;

class Array2Activitie1
{
    static void Main()
    {
        Console.Write("¿De qué tamaño será la matriz?: ");
        int n = int.Parse(Console.ReadLine());

        int[] valores = new int[n];

        // Pedir los valores
        for (int i = 0; i < n; i++)
        {
            Console.Write($"Ingrese el valor para la posición {i + 1}: ");
            valores[i] = int.Parse(Console.ReadLine());
        }

        // Mostrar los valores ingresados
        Console.WriteLine("\nLos valores ingresados son:");
        foreach (int v in valores)
        {
            Console.Write(v + " ");
        }
        Console.WriteLine();

        // Pedir posición y valor a insertar
        Console.Write($"\nIngrese la posición donde desea insertar (0 a {n}): ");
 int pos = int.Parse(Console.ReadLine()!);



        Console.Write("Ingrese el valor a insertar: ");
        int valor = int.Parse(Console.ReadLine()!);

        // Crear un arreglo más grande para insertar
        int[] nuevaLista = new int[n + 1];

        // Copiar y desplazar elementos
        for (int i = 0; i < pos; i++)
        {
            nuevaLista[i] = valores[i];
        }

        nuevaLista[pos] = valor;

        for (int i = pos; i < n; i++)
        {
            nuevaLista[i + 1] = valores[i];
        }

        // Mostrar la nueva lista
        Console.WriteLine("\nLa nueva lista es:");
        foreach (int v in nuevaLista)
        {
            Console.Write(v + " ");
        }
        Console.WriteLine();
    }
}
