#include <iostream>
using namespace std;

int main() {
    int matriz[3][3][3]; // 3 bloques, cada uno con una matriz de 3 filas por 3 columnas.
    int m = 0; 
    int nuevo[27];
    
    // Llenar matriz manualmente
    for (int i = 0; i < 3; i++) { // Recorre el arreglo por bloques 
        cout << "Bloque " << i + 1 << ":\n";
        for (int j = 0; j < 3; j++) { // Recorre las filas
            for (int k = 0; k < 3; k++) { // Recorre las columnas
                cout << "Ingresa valor para [" << i << "][" << j << "][" << k << "]: ";
                cin >> matriz[i][j][k]; // Lo guarda 
            }
         }
     }

     //El punto de esto, es hacer una matriz de una sola dimension, ya que el punto es volver a imprimir la matriz, pero sin los valores duplicados
     
    for (int i = 0; i < 3; i++) { // Recorre la Matriz
        for (int j = 0; j < 3; j++) { 
                for (int k = 0; k < 3; k++) {
                    int valor = matriz[i][j][k]; //  Almacena temporalmente el valor de la matriz, que sirve para revisar si ese número ya existe en nuevo.
                    bool repetido = false; //  Va a da por hecho que el primero valor es en falsos
                    
                    // m es la cantidad de elementos que ya hemos insertado en nuevo
                    for (int x = 0; x < m; x++) {
                        // c compara los valores ya guardados en la matriz
                    if (nuevo[x] == valor) { // compara cada valor que ya está en nuevo con el valor actual de la matriz.
                        repetido = true;
                        break;
                    }
                }

                if (!repetido) { // Si no es igual a repetido el valor de m se auments
                    nuevo[m++] = valor;
            }
        }
    }
}

    cout << "Sin duplicados: ";
    for (int i = 0; i < m; i++) {
        cout << nuevo[i] << " "; // Imprime la nueva matriz
    }
    cout << endl;

 //Ordenar por filas
    for (int i = 0; i < 3; i++) {      
        for (int j = 0; j < 3; j++) {  
            // Metodo de Burbuja para fila de 3 elementos
            for (int k = 0; k < 3; k++) { // controla cuántas veces se repite el proceso de burbuja para asegurarse de que todos los elementos de la fila estén en orden.
                for (int l = 0; l < 3 - k; l++) { // recorre los elementos de la fila para comparar (matriz[i][j][l] y matriz[i][j][l+1]) y asegura que cada vez se recorra un elemento menos, porque el burbuja ya “colocó al final” el mayor elemento.
                    if (matriz[i][j][l] > matriz[i][j][l + 1]) { // 
                        int temp = matriz[i][j][l]; // se usa como variable temporal para hacer el intercambio sin perder datos
                        matriz[i][j][l] = matriz[i][j][l + 1];
                        matriz[i][j][l + 1] = temp;
                    }
                }
            }
        }
    }

    // Mostrar matriz ya ordenada
    cout << "Matriz ordenada por filas:\n";
    for (int i = 0; i < 3; i++) {
        cout << "Bloque " << i + 1 << '\n';
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                cout << matriz[i][j][k] << " ";
            }
            cout << '\n';
        }
        cout << '\n';
    }

        // Promedio por bloque
        for (int i = 0; i < 3; i++) { //izy
        int suma = 0;
        int contador = 0;
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    suma += matriz[i][j][k];
                    contador++;
            }
        }
        double promedio = (double)suma / contador;
        std::cout << "El promedio del bloque " << i + 1 << " es: " << promedio << '\n';
        }

    return 0;
}
