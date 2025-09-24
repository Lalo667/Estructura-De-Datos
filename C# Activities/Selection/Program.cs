using System;

class Program
{
    static void Swap(int[] arr, int i, int j)
    {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    static int Partition(int[] arr, int low, int high)
    {
        int pivot = arr[high]; // último elemento como pivote
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                Swap(arr, i, j);
            }
        }

        Swap(arr, i + 1, high); // coloca el pivote en su lugar
        return i + 1;
    }


    static void QuickSort(int[] arr, int low, int high)
    {
        if (low < high)
        {
            int pi = Partition(arr, low, high);

            QuickSort(arr, low, pi - 1);
            QuickSort(arr, pi + 1, high);
        }
    }

    static void Main()
    {
        int[] arr = { 10, 7, 8, 9, 1, 5 };

        Console.WriteLine("Arreglo antes de ordenarlo:");
        Console.WriteLine(string.Join(", ", arr));

        QuickSort(arr, 0, arr.Length - 1);

        Console.WriteLine("Arreglo después de ordenarlo:");
        Console.WriteLine(string.Join(", ", arr));
    }
}
