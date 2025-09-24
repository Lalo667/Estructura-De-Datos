public class QuickSort {

    static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    static int partition(int[] arr, int l, int h) {
        int pvt = arr[h]; 
        int j = l - 1;
        for (int k = l; k < h; k++) {
            if (arr[k] < pvt) {
                j++;
                swap(arr, j, k);
            }
        }
        swap(arr, j + 1, h); 
        return j + 1;
    }

    // QuickSort recursivo
    static void quickSort(int[] arr, int l, int h) {
        if (l < h) {
            int pi = partition(arr, l, h);
            quickSort(arr, l, pi - 1);
            quickSort(arr, pi + 1, h);
        }
    }

    // Método principal
    public static void main(String[] args) {
        int[] arr = {10, 7, 8, 9, 1, 5};
        int size = arr.length;

        System.out.println("El arreglo antes de ordenarlo:");
        for (int v : arr) {
            System.out.print(v + " ");
        }
        System.out.println();

        quickSort(arr, 0, size - 1);

        System.out.println("El arreglo después de ordenarlo:");
        for (int v : arr) {
            System.out.print(v + " ");
        }
        System.out.println();
    }
}
