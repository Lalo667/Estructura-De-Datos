using System;

class Program
{
    static void InsertionSort(int[] a)
    {
        for (int i = 1; i < a.Length; i++)
        {
            int temp = a[i]; 
            int j = i - 1;

            while (j >= 0 && temp < a[j])
            {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = temp;  
        }
    }

    // Método para imprimir el arreglo
    static void PrintArr(int[] a)
    {
        for (int i = 0; i < a.Length; i++)
        {
            Console.Write(a[i] + " ");
        }
        Console.WriteLine();
    }

    static void Main()
    {
        int[] a = { 70, 12, 32, 34, 65 };
        Console.WriteLine("Antes de ordenar los elementos del arreglo:");
        PrintArr(a);
        InsertionSort(a);
        Console.WriteLine("Después de ordenar los elementos del arreglo:");
        PrintArr(a);
    }
}
