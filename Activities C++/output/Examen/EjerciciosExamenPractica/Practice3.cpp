#include <iostream>
using namespace std;

int main() {
    int x=5, y=5, z=5;
    int matriz[x][y][z];  // 5 bloques, cada uno con una matriz de 5 filas por 5 columnas.

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                matriz[i][j][k] = rand() % 125 + 1; // le asignamos nuemeros random a tda la matriz
            }
        }
    } 

    for (int i = 0; i < x; i++) {
        cout << "Bloque " << i+1 << ":\n";
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                cout << matriz[i][j][k] << "\t"; // Enseñamos la matriz cmo queda
            }
            cout << endl;
        }
        cout << "-----------------------\n";
    }
    //Metodo Burbuja, para acomodarlo pero por filas
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z - 1; k++) {
                for (int l = 0; l < z - 1 - k; l++) {
                    if (matriz[i][j][l] > matriz[i][j][l + 1]) {
                        int temp = matriz[i][j][l];
                        matriz[i][j][l] = matriz[i][j][l + 1];
                        matriz[i][j][l + 1] = temp;
                    }
                }
            }
        }
    }

    // Mostrar matriz ordenada por filas
    cout << "\n=== MATRIZ ORDENADA POR FILAS ===\n";
    for (int i = 0; i < x; i++) {
        cout << "Bloque " << i + 1 << ":\n";
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                cout << matriz[i][j][k] << "\t";
            }
            cout << endl;
        }
        cout << "-----------------------\n";
    }

    // Buscar un valor ingresado por el usuario
    int valor;
    bool encontrado = false;
    cout << "\nIngrese un numero a buscar: ";
    cin >> valor;

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                if (matriz[i][j][k] == valor) {
                    cout << " Valor encontrado en Bloque " << i+1
                         << ", Fila " << j+1
                         << ", Columna " << k+1 << endl;
                    encontrado = true;
                }
            }
        }
    }

    if (!encontrado) {
        cout << "El valor " << valor << " NO se encuentra en la matriz.\n";
    }

        // Encontrar valor maximo 
    int maximo = matriz[0][0][0]; // inicializar con el primer elemento
    int posI = 0, posJ = 0, posK = 0; // para guardar la posicion el cual el valor es mas grande

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (matriz[i][j][k] > maximo) {         // Si por ejemplo
                    maximo = matriz[i][j][k];           // 3, 7, 2, 9, 1
                                                        // maximo = 3 (primer valor).    
                    posI = i;                           //  Revisa 7  como 7 > 3, ahora maximo = 7 y posición (i, j, k) se guarda
                    posJ = j;                           // Revisa 2 → no es mayor, no cambia nada
                    posK = k;                           // Revisa 9 → 9 > 7, entonces maximo = 9 y se actualiza la posición
                }
            }
        }
    }

        for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            bool ascendente = (j % 2 == 0); // filas pares -> ascendente, impares -> descendente
            for (int k = 0; k < z - 1; k++) {
                for (int l = 0; l < z - 1 - k; l++) {
                    if ((ascendente && matriz[i][j][l] > matriz[i][j][l + 1]) ||
                        (!ascendente && matriz[i][j][l] < matriz[i][j][l + 1])) {
                        int temp = matriz[i][j][l];
                        matriz[i][j][l] = matriz[i][j][l + 1];
                        matriz[i][j][l + 1] = temp;
                    }
                }
            }
        }
    }

    // Mostrar matriz ordenada alternadamente
    cout << "\n=== MATRIZ ORDENADA ALTERNADA ===\n";
    for (int i = 0; i < x; i++) {
        cout << "Bloque " << i + 1 << ":\n";
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++)
                cout << matriz[i][j][k] << "\t";
            cout << endl;
        }
        cout << "-----------------------\n";
    }

    return 0;
}

