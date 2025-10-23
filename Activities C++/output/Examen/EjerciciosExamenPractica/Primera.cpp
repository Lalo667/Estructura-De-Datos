#include <iostream>
#include <limits>
using namespace std;

int main() {
    int matriz[2][3][3]; 
    int bus;
    int par(0); 

    // Llenar matriz manualmente
    for (int i = 0; i < 2; i++) { // Recorre el arreglo por bloques 
        std::cout << "Bloque " << i + 1 << ":\n";
        for (int j = 0; j < 3; j++) { // Recorre las filas
            for (int k = 0; k < 3; k++) { // Recorre las columnas
                std::cout << "Ingresa valor para [" << i << "][" << j << "][" << k << "]: ";
                std::cin >> matriz[i][j][k]; // Lo guarda 
                            
                            // Funciona si o si con namespace
                            if (cin.fail()) { // si no se ingreso un entero
                        cout << "Tas wey \n";
                        cin.clear(); // limpia el estado de error
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta lo mal escrito
                    } else {
                        break; // salida del bucle si es correcto
                    }
                }
            }
        }

 // C declara la matriz previamente
 //   int arr[2][3][3] = {
 //   {  
 //       {1, 2, 3},
 //       {4, 5 ,6},
 //       {7, 8 ,9}
 //
 //   },
 //   {
 //       {10, 12, 13},
 //       {14, 15 ,16},
 //       {17, 18 ,19}
 //   }
 // };    


    // Buscar numero
    std::cout << "Ingrese el numero que desea buscar" << '\n';
    std::cin >> bus; 
    for (int i = 0; i < 2; i++) { // Recorre el arreglo por bloque
        for (int j = 0; j < 3; j++) { // Recorre el arreglo por fila
            for (int k = 0; k < 3; k++) { // Recorre eñ arreglo por columna
                if (matriz[i][j][k] == bus){ // Si el valor que recorrio el arreglo es igual a bus
                std::cout << "El numero: "<< bus << " Esta en la posicion: " << "[" << i << "][" << j << "][" << k << "]: "<<'\n'; // Escribe la pos
                break; // hace que solo salga del bucle más interno (k), no de todos.
                }
            }
        }
    }
    // Conteo de pares
        for (int i = 0; i < 2; i++) { // Recorrera toda la matriz
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (matriz[i][j][k] % 2 == 0 ){ // Esto verifica si es par o no es par, si lo es, le sumara 1
                    par += 1;
                }
            }
        }
    }
    std::cout << par << " pares" << '\n'; // Aca lo escribe en la terminal
    
    //Suma de columnas
    for (int i = 0; i < 2; i++) { 
        std::cout << "\nSuma de columnas en el bloque " << i + 1 << ":\n";
        for (int k = 0; k < 3; k++) { // k recorre las columnas (0, 1, 2) del bloque actual.
            int sumaColumna = 0;
            for (int j = 0; j < 3; j++) {  
                sumaColumna += matriz[i][j][k]; // En cada fila, toma el valor de la posición [i][j][k] y lo suma.
            }
            std::cout << "Columna " << k + 1 << ": " << sumaColumna << '\n'; // Escribe la suma de columnas
        }
    }
    
    return 0;
}