#include <iostream>
#include <limits>
using namespace std;

int main(){
    int i = 10;
    int bus;
    int matriz1[10];
    int matriz2[3][3]  = {
        {1, 4, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int matriz3[3][3][3] = {
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        },
        {
            {10, 12, 13},
            {14, 15, 16},
            {17, 18, 19}
        },
        {
            {10, 12, 13},
            {14, 15, 16},
            {17, 18, 19}
        }
    };

    for (int i = 0; i < 10; i++) {
        cout << "Ingresa valor para " << i + 1 << ": ";
        cin >> matriz1[i];
        if (cin.fail()) {
            cout << "Esta mal\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            i--;
        }
    }

    int par = 0;
    for (int k = 0; k < 10; k++) {
        if (matriz1[k] % 2 == 0) {
            par += 1;
        }
    }
    cout << par << " pares" << '\n';

    for (int i = 0; i < 3; i++) { 
        cout << "\nSuma de columnas en el bloque " << i + 1 << ":\n";
        for (int j = 0; j < 3; j++) {
            int sumaColumna = 0;   
            sumaColumna += matriz2[0][j] + matriz2[1][j] + matriz2[2][j];
            cout << "Columna " << j + 1 << ": " << sumaColumna << '\n';
        }
    }

    cout << "Ingrese el numero que desea buscar" << '\n';
    cin >> bus;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (matriz3[i][j][k] == bus) {
                    cout << "El numero: " << bus << " Esta en la posicion: [" << i << "][" << j << "][" << k << "]\n";
                }
            }
        }
    }
    
    return 0;
}
