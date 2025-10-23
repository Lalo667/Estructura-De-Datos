#include <iostream>
using namespace std;

void imprimirCuadrado(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "* ";
        }
        cout << endl;
    }
}

void imprimirRectangulo(int base, int altura) {
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < base; j++) {
            cout << "* ";
        }
        cout << endl;
    }
}

// Función para pedir dimensiones al usuario e imprimir un cuadrado
void cuadradoUsuario() {
    int n;
    cout << "\nIngresa la dimension del cuadrado: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "* ";
        }
        cout << endl;
    }
}

int main() {
    cout << "Cuadrado de 5:\n";
    imprimirCuadrado(5);

    cout << "\nRectangulo:\n";
    imprimirRectangulo(8, 5);

    cout << "\nCuadrado con dimension ingresada por el usuario:\n";
    cuadradoUsuario();

    return 0;
}
