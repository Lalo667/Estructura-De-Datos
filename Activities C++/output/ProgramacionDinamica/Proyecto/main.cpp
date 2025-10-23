// Para ejecutar el archivo necesitamos g++ main.cpp paciente.cpp -o programa.exe  >> .\programa.exe
#include "paciente.h"

int main() {
    vector<Patient> patients;
    int opc;
    bool menu = true;

    while (menu) {
        showMenu();
        
        cin >> opc;

        switch (opc) {
        case 1: agregarPaciente(patients); break;
        case 2: buscarPaciente(patients); break;
        case 3: mostrarPacientes(patients); break;
        case 4: eliminarPaciente(patients); break;
        case 5: modificarPaciente(patients); break;

        case 6: menu = false; cout << "Saliendo del programa...\n"; break;
        default: cout << "Opcion no valida.\n"; break;
        }
    }

    return 0;
}
