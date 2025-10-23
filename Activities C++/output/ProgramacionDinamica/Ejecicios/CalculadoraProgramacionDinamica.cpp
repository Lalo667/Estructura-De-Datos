using namespace std;
#include <iostream>

int main() {
    
    int num1, num2, opc;
    
    cout << "Ingrese el primer numero: ";
    cin >> num1;
    cout << "Ingrese el segundo numero: ";
    cin >> num2;

    cout << "Seleccione una opcion: " << endl;
    cout << "1. Sumar" << endl;
    cout << "2. Restar" << endl;
    cout << "3. Multiplicar" << endl;
    cout << "4. Dividir" << endl;
    cin >> opc;

    switch (opc) {
        case 1:
            cout << "La suma es: " << num1 + num2 << endl;
            break;
        case 2:
            cout << "La resta es: " << num1 - num2 << endl;
            break;
        case 3:
            cout << "La multiplicacion es: " << num1 * num2 << endl;
            break;
        case 4:
            if (num2 != 0) {
                cout << "La division es: " << num1 / num2 << endl;
            } else {
                cout << "Error: Division por cero." << endl;
            }
            break;
        default:
            cout << "Opcion invalida." << endl;
            break;
    }

    return 0;
}