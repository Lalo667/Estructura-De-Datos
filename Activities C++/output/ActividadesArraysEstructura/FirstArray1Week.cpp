#include <iostream>
using namespace std;

struct Product {
    string nombre; //lo declaramos cmo string
    float precio; // c declara cmo punto decimal
};

int main() {
    Product product[3]; // arreglo de 3 productos

    for (int i = 0; i < 3; i++) { // for para llenar el arreglo
        cout << "Producto " << i + 1 << endl; // pedimos el producto
        cout << "Nombre: "; // ahora el nombre
        cin >> product[i].nombre; // se guarda en el arreglo
        cout << "Precio: "; // Le pedimos el precio
        cin >> product[i].precio; //lo guardamos en la matriz
        cout << endl; 
    }

    // Mostrar datos
    cout << "\nLista de productos:\n";
    for (int i = 0; i < 3; i++) {
        cout << product[i].nombre << " - $" << product[i].precio << endl;
    }

    return 0;
}
