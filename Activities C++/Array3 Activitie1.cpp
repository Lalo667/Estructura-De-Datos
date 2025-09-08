#include <iostream>
#include <array>
 using namespace std;
 int main(){
    int n; // es la variable en donde el usuario escribira el tamaño de la matriz
    cout << "De que tamaño sera la matriz?: ";
    cin >> n;
        array<int, 100> matriz; // Aca c declara q el arreglo tendra un maximo de 100 elementos
        for (int i = 0; i < n; i++){ // Ciclo para llenar el arreglo de elementos
        cout << "Ingrese el valor para la posicion " << i + 1 << ": ";
        cin >> matriz[i]; // Aca c almacena los elementos puestos por el usuario
    }
        int valor; // c declara
        cout << "Ingrese el valor que desea buscar: ";
        cin >> valor; // c almacena el valor que el usuario desea buscar

        bool encontrado = false; // c inicializa encontrado como false
        for (int j = 0; j < n; j++) { // Ciclo  para buscar la matriz, la j se iniciliza en 0, mientras que j sea menor q el tamaño de la matriz ,  c sumara 1
            if (matriz[j] == valor) { // Si el valor en la posicion j es igual al valor buscado
                encontrado = true; // c cambia encontrado a true
                break; // aca c sale del ciclo
            }
        }
        if (encontrado) {
            cout << "El valor " << valor << " se encuentra en la matriz." << endl;
        } else {
            cout << "El valor " << valor << " no se encuentra en la matriz." << endl;
        }
        return 0;
    }


