#include <iostream> // Esta libreria siempre va
using namespace std; // Este tambien siempre

int main() { // De entrada
    int precios[3]; // Declaramos un arreglo de 3 elementos

    for (int i = 0; i < 3; i++) {  //ciclo para llenar el arreglo
        cout << "Dame el precio " << i + 1 << ": "; // Pedimos el precio
        cin >> precios[i]; // El usuario ingresa el precio y se guarda en el arreglo
    }

    cout << "\nLos precios que ingresaste son:\n";
    for (int i = 0; i < 3; i++) { // Ciclo para mostrar los precios
        cout << precios[i] << endl ; // Mostramos el precio
    }

    return 0;
}