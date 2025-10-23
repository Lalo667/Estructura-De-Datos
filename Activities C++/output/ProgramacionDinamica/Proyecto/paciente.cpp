#include "paciente.h"
#include <limits>
#include <iomanip>
#include<algorithm>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayPatient(const Patient& p, int index) {
    cout << "| " << setw(2) << index + 1 << " "
         << "| " << left << setw(15) << p.name.substr(0, 15)
         << "| " << left << setw(10) << p.rfc
         << "| " << setw(4) << p.age
         << "| " << setw(8) << p.gender
         << "| " << setw(6) << p.weight
         << "| " << setw(6) << p.height
         << "|\n";
}

void showMenu() {
    clearScreen();
    cout << "\n--------------------------------------------------\n";
    cout << "-                                                  -\n";
    cout << "-     SISTEMA DE GESTION DE EXPEDIENTES MEDICOS    -\n";
    cout << "-                                                  -\n";
    cout << "---------------------------------------------------\n\n";
    cout << "  ----------------------------------------------\n";
    cout << "  -  [1]  Agregar un expediente                -\n";
    cout << "  -  [2]  Buscar expediente                    -\n";
    cout << "  -  [3]  Mostrar todos los expedientes        -\n";
    cout << "  -  [4]  Eliminar un expediente               -\n";
    cout << "  -  [5]  Modificar un expediente               -\n";
    cout << "  -  [6]  Salir del programa                   -\n";
    cout << "  ----------------------------------------------\n\n";
    cout << "  Seleccione una opcion: ";
}

void agregarPaciente(vector<Patient>& patients) {
    int next = 0;
    while (next == 0) {

        clearScreen();
        cout << "\n-------------------------------------------\n";
        cout << "         AGREGAR NUEVO EXPEDIENTE\n";
        cout << "--------------------------------------------\n\n";
        
        Patient p;
        cin.ignore();
        cout << "Ingrese el nombre del paciente: ";
        getline(cin, p.name);
        cout << "Ingrese la edad del paciente: ";
        cin >> p.age;
        cout << "Ingrese el RFC del paciente (solo numero): ";
        cin >> p.rfc;
        cout << "Ingrese el peso del paciente (kg): ";
        cin >> p.weight;
        cout << "Ingrese la altura del paciente (m): ";
        cin >> p.height;
        cin.ignore();
        cout << "Ingrese el genero del paciente: ";
        getline(cin, p.gender);

        patients.push_back(p);

        cout << "\nPaciente agregado correctamente.\n";
        cout << "¿Desea agregar otro paciente?\n";
        cout << "0 - Si \n";
        cout << "1 - No \n> ";
        cin >> next;
    }
}


void buscarPaciente(const vector<Patient>& patients) {
    clearScreen();
    if (patients.empty()) {
        cout << "\n No hay expedientes registrados.\n";
        pause();
        return;
    }

    int option = 0;

    while (true) {
        clearScreen();
        cout << "\n=========================================\n";
        cout << "            BUSCAR EXPEDIENTE\n";
        cout << "=========================================\n";
        cout << "1 Buscar por nombre\n";
        cout << "2 Buscar por RFC\n";
        cout << "3 Volver al menu principal\n";
        cout << "-----------------------------------------\n";
        cout << "Seleccione una opción: ";
        cin >> option;

        if (cin.fail() || option < 1 || option > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n Opción inválida. Intente de nuevo.\n";
            pause();
            continue;
        }

        if (option == 3) { 
            cout << "\nRegresando al menu principal...\n";
            pause();
            return;
        }

        cin.ignore();
        bool found = false;

        // --- Buscar por nombre ---
        if (option == 1) {
            string name;
            cout << "\nIngrese el nombre a buscar: ";
            getline(cin, name);

            for (size_t i = 0; i < patients.size(); i++) {
                // búsqueda parcial (ignora mayúsculas/minúsculas)
                string storedName = patients[i].name;
                string search = name;
                transform(storedName.begin(), storedName.end(), storedName.begin(), ::tolower);
                transform(search.begin(), search.end(), search.begin(), ::tolower);

                if (storedName.find(search) != string::npos) {
                    displayPatient(patients[i], i);
                    found = true;
                }
            }
        }

        else if (option == 2) {
            cout << "\n RFC registrados:\n";
            cout << "--------------------------\n";
            for (const auto& p : patients)
                cout << "- " << p.rfc << " (" << p.name << ")\n";

            cout << "--------------------------\n";
            cout << "Ingrese el RFC a buscar (o 0 para salir): ";

            int rfc;
            cin >> rfc;
            cin.ignore();

            if (rfc == 0) {
                cout << "\nCancelando búsqueda...\n";
                pause();
                return;
            }

            for (size_t i = 0; i < patients.size(); i++) {
                if (patients[i].rfc == rfc) {
                    clearScreen();
                    cout << "\n=========================================\n";
                    cout << " Expediente encontrado\n";
                    cout << "=========================================\n";

                    // Usa la versión de tabla que ya hicimos
                    cout << "| # | " << left << setw(15) << "Nombre"
                         << "| " << setw(10) << "RFC"
                         << "| " << setw(4)  << "Edad"
                         << "| " << setw(8)  << "Genero"
                         << "| " << setw(6)  << "Peso"
                         << "| " << setw(6)  << "Altura"
                         << "|\n";
                    cout << "---------------------------------------------------------------\n";

                    displayPatient(patients[i], i);
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "\n No se encontraron expedientes con ese criterio.\n";
        }

        pause();
        return;
    }
}


void eliminarPaciente(vector<Patient>& patients) {
   clearScreen();
    if (patients.empty()) {
        cout << "\n No hay expedientes registrados.\n";
        pause();
        return;
    }
    
    cout << "\n-----------------------------------------\n";
    cout << "          ELIMINAR EXPEDIENTE\n";
    cout << "------------------------------------------\n\n";

    // Mostrar lista de pacientes
    for (size_t i = 0; i < patients.size(); i++) {
        cout << "[" << i + 1 << "] " << patients[i].name << " - " << patients[i].rfc << endl;
    }
    
    cout << "\n Ingrese el numero del expediente a eliminar (0 para cancelar): ";
    int index;
    cin >> index;
    
    if (index > 0 && index <= static_cast<int>(patients.size())) {
        cout << "\n Esta seguro de eliminar el expediente de " << patients[index-1].name << "? (S/N): ";
        char confirm;
        cin >> confirm;
        
        if (confirm == 'S' || confirm == 's') {
            patients.erase(patients.begin() + index - 1);
            cout << "\n Expediente eliminado exitosamente.\n";
        } else {
            cout << "\n Operacion cancelada.\n";
        }
    } else if (index != 0) {
        cout << "\n Numero de expediente invalido.\n";
    }
    pause();
}

void mostrarPacientes(const vector<Patient>& patients) {
    clearScreen();

    if (patients.empty()) {
        cout << "\n No hay expedientes registrados.\n";
        pause();
        return;
    }

    cout << "\n=============================================================\n";
    cout << "                  LISTA DE PACIENTES (" << patients.size() << ")\n";
    cout << "=============================================================\n";
    

    cout << "---------------------------------------------------------------\n";
    cout << "| # | " << left << setw(15) << "Nombre"
         << "| " << setw(10) << "RFC"
         << "| " << setw(4)  << "Edad"
         << "| " << setw(8)  << "Genero"
         << "| " << setw(6)  << "Peso"
         << "| " << setw(6)  << "Altura"
         << "|\n";
    cout << "---------------------------------------------------------------\n";

    for (size_t i = 0; i < patients.size(); ++i) {
        displayPatient(patients[i], i);
    }

    cout << "---------------------------------------------------------------\n";

    pause();
}

void modificarPaciente(vector<Patient>& patients) {
    clearScreen();

    if (patients.empty()) {
        cout << "\n No hay pacientes registrados.\n";
        pause();
        return;
    }

    cout << "=============================================\n";
    cout << "           MODIFICAR EXPEDIENTE\n";
    cout << "=============================================\n\n";

    // Mostrar lista de pacientes
    cout << "Lista de pacientes:\n";
    cout << "---------------------------------------------\n";
    for (size_t i = 0; i < patients.size(); i++) {
        cout << "[" << i + 1 << "] " << left << setw(20) << patients[i].name
             << "RFC: " << patients[i].rfc << '\n';
    }
    cout << "[0] Cancelar y regresar al menú\n";
    cout << "---------------------------------------------\n";

    int opcion;
    cout << "Seleccione el numero del paciente a modificar: ";
    cin >> opcion;

    if (opcion == 0) {
        cout << "\nRegresando al menu principal...\n";
        pause();
        return;
    }

    if (opcion < 1 || opcion > (int)patients.size()) {
        cout << "\n Opcion invalida.\n";
        pause();
        return;
    }

    Patient& p = patients[opcion - 1]; 

    cin.ignore();
    cout << "\nPaciente seleccionado: " << p.name << "\n";
    cout << "Ingrese los nuevos datos (deje vacio para mantener el valor actual):\n";

    string input;

    cout << "Nuevo nombre (" << p.name << "): ";
    getline(cin, input);
    if (!input.empty()) p.name = input;

    cout << "Nueva edad (" << p.age << "): ";
    getline(cin, input);
    if (!input.empty()) p.age = stoi(input);

    cout << "Nuevo RFC (" << p.rfc << "): ";
    getline(cin, input);
    if (!input.empty()) p.rfc = stoi(input);

    cout << "Nuevo peso (" << p.weight << " kg): ";
    getline(cin, input);
    if (!input.empty()) p.weight = stof(input);

    cout << "Nueva altura (" << p.height << " m): ";
    getline(cin, input);
    if (!input.empty()) p.height = stof(input);

    cout << "Nuevo genero (" << p.gender << "): ";
    getline(cin, input);
    if (!input.empty()) p.gender = input;

    cout << "\n Los datos han sido actualizados correctamente.\n";
    pause();
}
