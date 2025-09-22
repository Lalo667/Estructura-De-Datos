#include <iostream>
using namespace std;

void insertionSort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int temp = a[i];  
        int j = i - 1;

        while (j >= 0 && temp < a[j]) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp;
    }
}

void printArr(int a[], int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main() {
    int a[] = {70, 12, 32, 34, 65};
    int n = sizeof(a) / sizeof(a[0]); 

    cout << "Antes de ordenar los elementos del arreglo:" << endl;
    printArr(a, n);

    insertionSort(a, n);

    cout << "Después de ordenar los elementos del arreglo:" << endl;
    printArr(a, n);

    return 0;
}
