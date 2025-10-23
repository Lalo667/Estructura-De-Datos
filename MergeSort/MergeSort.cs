using System;

class MergeSort
{
    static void Merge(int[] a, int l, int m, int r)
    {
        int a1 = m - l + 1;
        int a2 = r - m;

        int[] L = new int[a1];
        int[] R = new int[a2];

        for (int j = 0; j < a1; j++)
            L[j] = a[l + j];
        for (int k = 0; k < a2; k++)
            R[k] = a[m + 1 + k];

        int i = 0, j2 = 0, k2 = l;

        while (i < a1 && j2 < a2)
        {
            if (L[i] <= R[j2])
            {
                a[k2] = L[i];
                i++;
            }
            else
            {
                a[k2] = R[j2];
                j2++;
            }
            k2++;
        }

        while (i < a1)
        {
            a[k2] = L[i];
            i++;
            k2++;
        }

        while (j2 < a2)
        {
            a[k2] = R[j2];
            j2++;
            k2++;
        }
    }

    static void MergeSortRecursive(int[] a, int l, int r)
    {
        if (l < r)
        {
            int m = l + (r - l) / 2;
            MergeSortRecursive(a, l, m);
            MergeSortRecursive(a, m + 1, r);
            Merge(a, l, m, r);
        }
    }

    static void Main()
    {
        int[] a = { 39, 28, 44, 11 };
        int s = a.Length;

        Console.Write("Antes de ordenar: ");
        foreach (int x in a) Console.Write(x + " ");
        Console.WriteLine();

        MergeSortRecursive(a, 0, s - 1);

        Console.Write("Después de ordenar: ");
        foreach (int x in a) Console.Write(x + " ");
        Console.WriteLine();
    }
}