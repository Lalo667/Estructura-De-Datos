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

// ==================== OBTENER DESCRIPCIÓN DEL ESCENARIO ====================
string obtenerDescripcionEscenario(string tipo) {
    if (tipo == "Ordenado") {
        return "Arreglo ya ordenado ascendentemente [1, 2, 3, ..., n]";
    } else if (tipo == "Medianamente Ordenado") {
        return "Arreglo parcialmente ordenado (70% ordenado, 30% aleatorio)";
    } else {
        return "Arreglo ordenado inversamente [n, n-1, ..., 2, 1]";
    }
}

// ==================== FUNCIÓN DE ANÁLISIS ====================
void analizarAlgoritmo(string nombre, void (*sortFunc)(vector<int>&)) {
    vector<int> tamanios = {100, 1000, 10000, 100000};
    vector<string> tipos = {"Ordenado", "Medianamente Ordenado", "Inverso"};
    
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                       ANÁLISIS: " << left << setw(40) << nombre << "║\n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";

    for (int n : tamanios) {
        cout << "┌───────────────────────────────────────────────────────────────────────────┐\n";
        cout << "│  TAMAÑO DEL ARREGLO: " << right << setw(8) << n << " elementos" << setw(37) << "│\n";
        cout << "└───────────────────────────────────────────────────────────────────────────┘\n\n";
        
        double tiempoMejor = -1;
        string escenarioMejor;
        
        for (int i = 0; i < tipos.size(); i++) {
            cout << "  ┌─────────────────────────────────────────────────────────────────────┐\n";
            cout << "  │ ESCENARIO " << (i+1) << ": " << left << setw(55) << tipos[i] << "│\n";
            cout << "  │ " << left << setw(70) << obtenerDescripcionEscenario(tipos[i]) << "│\n";
            cout << "  └─────────────────────────────────────────────────────────────────────┘\n";
            
            vector<int> arr;
            if (i == 0) arr = generarOrdenado(n);
            else if (i == 1) arr = generarMedianamenteOrdenado(n);
            else arr = generarInverso(n);
            
            double tiempo = medirTiempo(arr, sortFunc);
            
            cout << "    ⏱️  Tiempo de ejecución: " << right << setw(12) << fixed << setprecision(4) 
                 << tiempo << " ms\n\n";
            
            if (tiempoMejor == -1 || tiempo < tiempoMejor) {
                tiempoMejor = tiempo;
                escenarioMejor = tipos[i];
            }
        }
        
        cout << "  ╔═══════════════════════════════════════════════════════════════════════╗\n";
        cout << "  ║ 🏆 MEJOR ESCENARIO para " << left << setw(8) << n << " elementos: " 
             << left << setw(27) << escenarioMejor << "║\n";
        cout << "  ║    Tiempo: " << right << setw(12) << fixed << setprecision(4) 
             << tiempoMejor << " ms" << setw(43) << "║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════════════════╝\n\n";
    }
}

// ==================== COMPARAR TODOS LOS ALGORITMOS ====================
void compararTodosLosAlgoritmos() {
    struct Algoritmo {
        string nombre;
        void (*funcion)(vector<int>&);
    };
    
    vector<Algoritmo> algoritmos = {
        {"Burbuja", bubbleSort},
        {"Por Cubos", bucketSort},
        {"Comb Sort", combSort},
        {"Conteo", countingSort},
        {"Heap Sort", heapSort},
        {"Insercion", insertionSort},
        {"Fusion", mergeSort},
        {"Rapido", quickSort},
        {"Radix Sort", radixSort},
        {"Seleccion", selectionSort},
        {"Shell Sort", shellSort}
    };
    
    vector<int> tamanios = {100, 1000, 10000, 100000};
    vector<string> tiposOrden = {"Ordenado", "Medianamente Ordenado", "Inverso"};
    
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║            COMPARACIÓN COMPLETA DE TODOS LOS ALGORITMOS                  ║\n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n";
    
    for (int n : tamanios) {
        cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║  TAMAÑO DEL ARREGLO: " << right << setw(8) << n << " elementos" << setw(36) << "║\n";
        cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n";
        
        for (int tipoIdx = 0; tipoIdx < 3; tipoIdx++) {
            cout << "\n┌───────────────────────────────────────────────────────────────────────────┐\n";
            cout << "│  ESCENARIO " << (tipoIdx+1) << ": " << left << setw(60) << tiposOrden[tipoIdx] << "│\n";
            cout << "│  " << left << setw(74) << obtenerDescripcionEscenario(tiposOrden[tipoIdx]) << "│\n";
            cout << "└───────────────────────────────────────────────────────────────────────────┘\n\n";
            
            vector<pair<string, double>> resultados;
            
            for (const auto& algo : algoritmos) {
                vector<int> arr;
                if (tipoIdx == 0) arr = generarOrdenado(n);
                else if (tipoIdx == 1) arr = generarMedianamenteOrdenado(n);
                else arr = generarInverso(n);
                
                double tiempo = medirTiempo(arr, algo.funcion);
                resultados.push_back({algo.nombre, tiempo});
                
                cout << "  " << left << setw(22) << algo.nombre << ": " 
                     << right << setw(14) << fixed << setprecision(4) << tiempo << " ms\n";
            }
            
            auto mejor = min_element(resultados.begin(), resultados.end(),
                [](const pair<string, double>& a, const pair<string, double>& b) {
                    return a.second < b.second;
                });
            
            auto peor = max_element(resultados.begin(), resultados.end(),
                [](const pair<string, double>& a, const pair<string, double>& b) {
                    return a.second < b.second;
                });
            
            cout << "\n  ╔═════════════════════════════════════════════════════════════════════╗\n";
            cout << "  ║  🏆 MEJOR ALGORITMO: " << left << setw(22) << mejor->first 
                 << right << setw(14) << fixed << setprecision(4) << mejor->second << " ms       ║\n";
            cout << "  ║  ❌ PEOR ALGORITMO:  " << left << setw(22) << peor->first 
                 << right << setw(14) << fixed << setprecision(4) << peor->second << " ms       ║\n";
            cout << "  ║  📊 DIFERENCIA:      " << right << setw(36) 
                 << fixed << setprecision(4) << (peor->second - mejor->second) << " ms       ║\n";
            cout << "  ║  ⚡ FACTOR DE MEJORA: " << right << setw(35) 
                 << fixed << setprecision(2) << (peor->second / mejor->second) << "x        ║\n";
            cout << "  ╚═════════════════════════════════════════════════════════════════════╝\n";
        }
    }
    
    cout << "\n\n╔═══════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                    RESUMEN Y RECOMENDACIONES                              ║\n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";
    
    cout << "┌───────────────────────────────────────────────────────────────────────────┐\n";
    cout << "│  📋 ESCENARIO 1: DATOS YA ORDENADOS                                       │\n";
    cout << "│     Descripción: [1, 2, 3, ..., n]                                        │\n";
    cout << "└───────────────────────────────────────────────────────────────────────────┘\n";
    cout << "   🏆 Mejor opción: Counting Sort o Radix Sort\n";
    cout << "   ⚡ Complejidad: O(n) - Tiempo lineal\n";
    cout << "   💡 Alternativa: Insertion Sort (O(n) en mejor caso)\n\n";
    
    cout << "┌───────────────────────────────────────────────────────────────────────────┐\n";
    cout << "│  📋 ESCENARIO 2: DATOS MEDIANAMENTE ORDENADOS                             │\n";
    cout << "│     Descripción: 70% ordenado, 30% elementos aleatorios                   │\n";
    cout << "└───────────────────────────────────────────────────────────────────────────┘\n";
    cout << "   🏆 Mejor opción: Quick Sort o Merge Sort\n";
    cout << "   ⚡ Complejidad: O(n log n) promedio\n";
    cout << "   💡 Ventaja: Buen balance entre velocidad y estabilidad\n\n";
    
    cout << "┌───────────────────────────────────────────────────────────────────────────┐\n";
    cout << "│  📋 ESCENARIO 3: DATOS ORDENADOS INVERSAMENTE                             │\n";
    cout << "│     Descripción: [n, n-1, ..., 2, 1] - Peor caso para muchos algoritmos  │\n";
    cout << "└───────────────────────────────────────────────────────────────────────────┘\n";
    cout << "   🏆 Mejor opción: Merge Sort o Heap Sort\n";
    cout << "   ⚡ Complejidad: O(n log n) garantizado\n";
    cout << "   ⚠️  EVITAR: Quick Sort (degrada a O(n²) en peor caso)\n\n";
    
    cout << "╔═══════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                    RECOMENDACIÓN GENERAL POR CASO                         ║\n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";
    cout << "  ✅ Uso general (datos aleatorios):     Quick Sort o Merge Sort\n";
    cout << "  ✅ Datos pequeños con enteros:         Counting Sort\n";
    cout << "  ✅ Estabilidad garantizada:            Merge Sort\n";
    cout << "  ✅ Memoria limitada:                   Heap Sort o Quick Sort (in-place)\n";
    cout << "  ✅ Datos casi ordenados:               Insertion Sort o Shell Sort\n";
    cout << "  ✅ Enteros con rango limitado:         Radix Sort o Counting Sort\n\n";
}

int main() {
    int opcion;
    
    do {
        cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║         SISTEMA DE ANÁLISIS DE ALGORITMOS DE ORDENAMIENTO                ║\n";
        cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n";
        cout << "┌───────────────────────────────────────────────────────────────────────────┐\n";
        cout << "│  1.  Burbuja (Bubble Sort)                                                │\n";
        cout << "│  2.  Por Cubos (Bucket Sort)                                              │\n";
        cout << "│  3.  Comb Sort                                                            │\n";
        cout << "│  4.  Conteo (Counting Sort)                                               │\n";
        cout << "│  5.  Heap Sort                                                            │\n";
        cout << "│  6.  Inserción (Insertion Sort)                                           │\n";
        cout << "│  7.  Fusión (Merge Sort)                                                  │\n";
        cout << "│  8.  Rápido (Quick Sort)                                                  │\n";
        cout << "│  9.  Radix Sort                                                           │\n";
        cout << "│  10. Por Selección (Selection Sort)                                       │\n";
        cout << "│  11. Shell Sort                                                           │\n";
        cout << "├───────────────────────────────────────────────────────────────────────────┤\n";
        cout << "│  12. ⭐ COMPARAR TODOS LOS ALGORITMOS                                     │\n";
        cout << "├───────────────────────────────────────────────────────────────────────────┤\n";
        cout << "│  0.  Salir                                                                │\n";
        cout << "└───────────────────────────────────────────────────────────────────────────┘\n";
        cout << "\n>>> Seleccione una opción: ";
        cin >> opcion;
        
        cout << fixed << setprecision(4);
        
        switch(opcion) {
            case 1:
                analizarAlgoritmo("MÉTODO BURBUJA (BUBBLE SORT)", bubbleSort);
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
                analizarAlgoritmo("INSERCIÓN (INSERTION SORT)", insertionSort);
                break;
            case 7:
                analizarAlgoritmo("FUSIÓN (MERGE SORT)", mergeSort);
                break;
            case 8:
                analizarAlgoritmo("RÁPIDO (QUICK SORT)", quickSort);
                break;
            case 9:
                analizarAlgoritmo("RADIX SORT", radixSort);
                break;
            case 10:
                analizarAlgoritmo("POR SELECCIÓN (SELECTION SORT)", selectionSort);
                break;
            case 11:
                analizarAlgoritmo("SHELL SORT", shellSort);
                break;
            case 12:
                compararTodosLosAlgoritmos();
                break;
            case 0:
                cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗\n";
                cout << "║           ¡Gracias por usar el Sistema de Análisis!                      ║\n";
                cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";
                break;
            default:
                cout << "\n❌ Opción no válida. Por favor, intente de nuevo.\n";
        }
        
        if (opcion != 0) {
            cout << "\n>>> Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while(opcion != 0);
    
    return 0;
}