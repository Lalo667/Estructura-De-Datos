#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;     
    cout << "Ingrese el tamaño de la matriz: ";
    cin >> n;

    vector<int> matriz(n); // Usamos vector para manejar tamaño dinámico y la matriz pueda crecer

    for (int i = 0; i < n; i++) {
        cout << "Dame el valor " << i + 1 << ": ";
        cin >> matriz[i];
    }

    cout << "\nLos valores que ingresaste son:\n";
    for (int i = 0; i < n; i++) {
        cout << matriz[i] << " ";
    }
    cout << endl;

    int pos, valor;
    cout << "\nDame la posicion donde insertar (1-" << n << "): ";
    cin >> pos;
    cout << "Dame el nuevo valor: ";
    cin >> valor;


    if (pos >= 1 && pos <= n) {
 
        for (int i = n - 1; i > pos - 1; i--) {
            matriz[i] = matriz[i - 1];
        }

        matriz[pos - 1] = valor;

        cout << "\nValor insertado correctamente.\n";
    } else {
        cout << "Error: Posición inválida.\n";
    }

 
    cout << "\nLos valores actualizados son:\n";
    for (int i = 0; i < n; i++) {
        cout << matriz[i] << " ";
    }
    cout << endl;

    return 0;
}
