#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

struct Patient {
    string name;
    int age;
    string rfc;
    float weight;
    float height;
    string gender;
};

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

// Función para mostrar el menú principal
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
    cout << "  -  [5]  Salir del programa                   -\n";
    cout << "  ----------------------------------------------\n\n";
    cout << "  Seleccione una opcion: ";
}

// Función para agregar paciente
void addPatient(vector<Patient> & patients) {
    clearScreen();
    cout << "\-------------------------------------------\n";
    cout << "         AGREGAR NUEVO EXPEDIENTE\n";
    cout << "--------------------------------------------\n\n";
    
    Patient p;
    cin.ignore();
    
    cout << "  Nombre completo del paciente: ";
    getline(cin, p.name);
    
    cout << "  Edad: ";
    cin >> p.age;
    
    cin.ignore();
    cout << "  RFC: ";
    getline(cin, p.rfc);
    
    cout << "  Peso (kg): ";
    cin >> p.weight;
    
    cout << "  Altura (m): ";
    cin >> p.height;
    
    cin.ignore();
    cout << "  Género: ";
    getline(cin, p.gender);
    
    patients.push_back(p);
    
    cout << "\n ¡Expediente agregado exitosamente!\n";
    cout << "  Total de expedientes: " << patients.size() << endl;
    pause();
}

// Función para mostrar información de un paciente
void displayPatient(const Patient& p, int index) {
    cout << "\n-----------------------------------------------------------------------\n";
    cout << "-  Expediente #" << setw(3) << index + 1 << "                            │\n";
    cout << "--------------------------------------------------\n";
    cout << "-  Nombre:   " << left << setw(31) << p.name << "-\n";
    cout << "- RFC:      " << left << setw(31) << p.rfc << " -\n";
    cout << "-  Edad:     " << left << setw(31) << (to_string(p.age) + " años") << "-\n";
    cout << "-  Género:   " << left << setw(31) << p.gender << "                    -\n";
    cout << "-  Peso:     " << left << setw(31) << (to_string(p.weight) + " kg") <<"-\n";
    cout << "-  Altura:   " << left << setw(31) << (to_string(p.height) + " m") << "-\n";
    
    // Calcular IMC
    float imc = p.weight / (p.height * p.height);
    cout << "-  IMC:      " << left << setw(31) << (to_string(imc).substr(0, 5)) << "-\n";
    cout << "----------------------------------------------------\n";
}

// Función para buscar paciente
void searchPatient(const vector<Patient>& patients) {
    clearScreen();
    if (patients.empty()) {
        cout << "\n No hay expedientes registrados.\n";
        pause();
        return;
    }

    cout << "\n-----------------------------------------\n";
    cout << "           BUSCAR EXPEDIENTE\n";
    cout << "-----------------------------------------\n";
    cout << "\n[1] Buscar por nombre\n";
    cout << "[2] Buscar por RFC\n";
    cout << "\nSeleccione opción: ";
    
    int option;
    cin >> option;
    cin.ignore();
    
    bool found = false;
    
    if (option == 1) {
        string name;
        cout << "\nIngrese el nombre a buscar: ";
        getline(cin, name);
        
        for (size_t i = 0; i < patients.size(); i++) {
            if (patients[i].name.find(name) != string::npos) {
                displayPatient(patients[i], i);
                found = true;
            }
        }
    } else if (option == 2) {
        string rfc;
        cout << "\nIngrese el RFC a buscar: ";
        getline(cin, rfc);
        
        for (size_t i = 0; i < patients.size(); i++) {
            if (patients[i].rfc == rfc) {
                displayPatient(patients[i], i);
                found = true;
            }
        }
    }
    
    if (!found) {
        cout << "\n No se encontraron expedientes con ese criterio.\n";
    }
    pause();
}

// Función para mostrar todos los pacientes
void showAllPatients(const vector<Patient>& patients) {
    clearScreen();
    if (patients.empty()) {
        cout << "\n No hay expedientes registrados.\n";
        pause();
        return;
    }
    
    cout << "\n-----------------------------------------\n";
    cout << "        LISTA DE EXPEDIENTES (" << patients.size() << ")\n";
    cout << "-----------------------------------------\n";

    for (size_t i = 0; i < patients.size(); i++) {
        displayPatient(patients[i], i);
    }
    pause();
}

// Función para eliminar paciente
void deletePatient(vector<Patient>& patients) {
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
    
    cout << "\nIngrese el número del expediente a eliminar (0 para cancelar): ";
    int index;
    cin >> index;
    
    if (index > 0 && index <= static_cast<int>(patients.size())) {
        cout << "\n¿Está seguro de eliminar el expediente de " << patients[index-1].name << "? (S/N): ";
        char confirm;
        cin >> confirm;
        
        if (confirm == 'S' || confirm == 's') {
            patients.erase(patients.begin() + index - 1);
            cout << "\n Expediente eliminado exitosamente.\n";
        } else {
            cout << "\n Operación cancelada.\n";
        }
    } else if (index != 0) {
        cout << "\n Número de expediente inválido.\n";
    }
    pause();
}

int main() {
    vector<Patient> patients;
    int opc;
    
    while (true) {
        showMenu();
        cin >> opc;
        
        switch (opc) {
            case 1:
                addPatient(patients);
                break;
            
            case 2:
                searchPatient(patients);
                break;
            
            case 3:
                showAllPatients(patients);
                break;
            
            case 4:
                deletePatient(patients);
                break;
            
            case 5:
                clearScreen();
                cout << "\n--------------------------------------------------\n";
                cout << "-                                                  -\n";
                cout << "-     ¡Gracias por usar el sistema!                -\n";
                cout << "-                                                  -\n";
                cout << "----------------------------------------------------\n\n";
                return 0;
            
            default:
                cout << "\n- Opción inválida. Intente nuevamente.\n";
                pause();
                break;
        }
    }
    
    return 0;
}