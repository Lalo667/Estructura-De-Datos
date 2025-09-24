#include <iostream>
using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int partition(int arr[], int l, int h) {
    int pvt = arr[h];  // Último elemento como pivote
    int j = l - 1;
    for (int k = l; k < h; k++) {
        if (arr[k] < pvt) {
            j++;
            swap(arr[j], arr[k]);
        }
    }
    swap(arr[j + 1], arr[h]);  // Coloca el pivote en su lugar correcto
    return j + 1;
}

void qckSort(int arr[], int l, int h) {
    if (l < h) {
        int pi = partition(arr, l, h);
        qckSort(arr, l, pi - 1);
        qckSort(arr, pi + 1, h);
    }
}

int main() {
    int arr[] = {10, 7, 8, 9, 1, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    cout << "El arreglo antes de ordenarlo: ";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;

    qckSort(arr, 0, size - 1);

    cout << "El arreglo después de ordenarlo: ";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;

    return 0;
}
