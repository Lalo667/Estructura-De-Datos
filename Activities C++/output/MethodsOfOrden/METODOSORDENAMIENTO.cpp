#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;

vector<int> generarOrdenado(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

vector<int> generarMedianamenteOrdenado(int n) {
    vector<int> arr = generarOrdenado(n);
    random_device rd;
    mt19937 gen(rd());
    int desordenar = n * 0.3;
    for (int i = 0; i < desordenar; i++) {
        uniform_int_distribution<> dis(0, n - 1);
        int pos1 = dis(gen);
        int pos2 = dis(gen);
        swap(arr[pos1], arr[pos2]);
    }
    return arr;
}

vector<int> generarInverso(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
    return arr;
}


// 1. BURBUJA
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 2. POR CUBOS (Bucket Sort)
void bucketSort(vector<int>& arr) {
    if (arr.empty()) return;
    int n = arr.size();
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end());
    int bucketCount = n;
    int range = maxVal - minVal + 1;
    
    vector<vector<int>> buckets(bucketCount);
    
    for (int i = 0; i < n; i++) {
        int bucketIndex = (buckets.size() * (arr[i] - minVal)) / range;
        if (bucketIndex >= bucketCount) bucketIndex = bucketCount - 1;
        buckets[bucketIndex].push_back(arr[i]);
    }
    
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
    }
    
    int index = 0;
    for (const auto& bucket : buckets) {
        for (int num : bucket) {
            arr[index++] = num;
        }
    }
}

// 3. COMB SORT
void combSort(vector<int>& arr) {
    int n = arr.size();
    int gap = n;
    bool swapped = true;
    
    while (gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;
        
        swapped = false;
        for (int i = 0; i + gap < n; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}

// 4. CONTEO (Counting Sort)
void countingSort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;
    
    vector<int> count(range, 0);
    vector<int> output(arr.size());
    
    for (int i = 0; i < arr.size(); i++)
        count[arr[i] - minVal]++;
    
    for (int i = 1; i < range; i++)
        count[i] += count[i - 1];
    
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }
    
    arr = output;
}

// 5. HEAP SORT
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 6. INSERCIÓN
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 7. FUSIÓN (Merge Sort)
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortHelper(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(vector<int>& arr) {
    mergeSortHelper(arr, 0, arr.size() - 1);
}

// 8. RÁPIDO (Quick Sort)
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

// 9. RADIX SORT
int getMax(vector<int>& arr) {
    return *max_element(arr.begin(), arr.end());
}

void countingSortForRadix(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);
    
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
    
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    
    arr = output;
}

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxVal = getMax(arr);
    
    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        countingSortForRadix(arr, exp);
}

// 10. SELECCIÓN
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        swap(arr[i], arr[minIdx]);
    }
}

// 11. SHELL SORT
void shellSort(vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// ==================== FUNCIÓN DE MEDICIÓN ====================
double medirTiempo(vector<int> arr, void (*sortFunc)(vector<int>&)) {
    auto inicio = high_resolution_clock::now();
    sortFunc(arr);
    auto fin = high_resolution_clock::now();
    duration<double, milli> duracion = fin - inicio;
    return duracion.count();
}

// ==================== FUNCIÓN DE ANÁLISIS ====================
void analizarAlgoritmo(string nombre, void (*sortFunc)(vector<int>&)) {
    vector<int> tamanios = {100, 1000, 10000, 100000};
    vector<string> tipos = {"Ordenado", "Medianamente Ordenado", "Inverso"};
    
    cout << "\n---------------------------------------------------------------\n";
    cout << "        ANÁLISIS: " << nombre << "\n";
    cout << "---------------------------------------------------------------\n\n";

    for (int n : tamanios) {
        cout << "TAMAÑO DEL ARREGLO: " << n << " elementos\n";
        cout << "-----------------------------------------------------------------\n";
        
        // Ordenado
        vector<int> arr = generarOrdenado(n);
        double tiempo = medirTiempo(arr, sortFunc);
        cout << "  " << left << setw(25) << tipos[0] << ": " << setw(10) << tiempo << " ms\n";
        
        // Medianamente ordenado
        arr = generarMedianamenteOrdenado(n);
        tiempo = medirTiempo(arr, sortFunc);
        cout << "  " << left << setw(25) << tipos[1] << ": " << setw(10) << tiempo << " ms\n";
        
        // Inverso
        arr = generarInverso(n);
        tiempo = medirTiempo(arr, sortFunc);
        cout << "  " << left << setw(25) << tipos[2] << ": " << setw(10) << tiempo << " ms\n";
        
        cout << "\n";
    }
}

int main() {
    int opcion;
    
    do {
        cout << "\n--------------------------------------------------------------\n";
        cout << "-     SISTEMA DE ANALISIS DE ALGORITMOS DE ORDENAMIENTO      -\n";
        cout << "--------------------------------------------------------------\n";
        cout << "-  1.  Burbuja                                               -\n";
        cout << "-  2.  Por Cubos (Bucket Sort)                               -\n";
        cout << "-  3.  Comb Sort                                             -\n";
        cout << "-  4.  Conteo (Counting Sort)                                -\n";
        cout << "-  5.  Heap Sort                                             -\n";
        cout << "-  6.  Insercion                                             -\n";
        cout << "-  7.  Fusion (Merge Sort)                                   -\n";
        cout << "-  8.  Rapido (Quick Sort)                                   -\n";
        cout << "-  9.  Radix Sort                                            -\n";
        cout << "-  10. Por Selección                                         -\n";
        cout << "-  11. Shell Sort                                            -\n";
        cout << "-  0.  Salir                                                 -\n";
        cout << "--------------------------------------------------------------\n";
        cout << "\nSeleccione un algoritmo: ";
        cin >> opcion;
        
        cout << fixed << setprecision(2);
        
        switch(opcion) {
            case 1:
                analizarAlgoritmo("MÉTODO BURBUJA", bubbleSort);
                break;
            case 2:
                analizarAlgoritmo("POR CUBOS (BUCKET SORT)", bucketSort);
                break;
            case 3:
                analizarAlgoritmo("COMB SORT", combSort);
                break;
            case 4:
                analizarAlgoritmo("CONTEO (COUNTING SORT)", countingSort);
                break;
            case 5:
                analizarAlgoritmo("HEAP SORT", heapSort);
                break;
            case 6:
                analizarAlgoritmo("INSERCION", insertionSort);
                break;
            case 7:
                analizarAlgoritmo("FUSION (MERGE SORT)", mergeSort);
                break;
            case 8:
                analizarAlgoritmo("RAPIDO (QUICK SORT)", quickSort);
                break;
            case 9:
                analizarAlgoritmo("RADIX SORT", radixSort);
                break;
            case 10:
                analizarAlgoritmo("POR SELECCION", selectionSort);
                break;
            case 11:
                analizarAlgoritmo("SHELL SORT", shellSort);
                break;
            case 0:
                cout << "\nGracias por usar el sistema de analisis!\n";
                break;
            default:
                cout << "\nOpción no valida. Intente de nuevo.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while(opcion != 0);
    
    return 0;
}