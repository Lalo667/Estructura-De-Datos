#include <iostream>

int main() {
    int matriz[2][3][3];
    int bus;
    int par(0);
    // Llenar matriz manualmente
    for (int i = 0; i < 2; i++) {
        std::cout << "Bloque " << i + 1 << ":\n";
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                std::cout << "Ingresa valor para [" << i << "][" << j << "][" << k << "]: ";
                std::cin >> matriz[i][j][k];
            }
        }
    }
}