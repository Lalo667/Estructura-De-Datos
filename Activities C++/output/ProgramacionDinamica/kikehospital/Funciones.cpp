#include "funciones.h"
#include <iostream>
#include <limits> // para limpiar buffer

void agregarPaciente(std::vector<Patient>& patients) {
    Patient temp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar buffer
    std::cout << "Nombre: ";
    std::getline(std::cin, temp.name);
    std::cout << "Edad: ";
    std::cin >> temp.age;
    std::cout << "Peso (kg): ";
    std::cin >> temp.weight;
    std::cout << "Altura (m): ";
    std::cin >> temp.height;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar buffer
    std::cout << "RFC: ";
    std::getline(std::cin, temp.RFC);
    std::cout << "Diagnostico: ";
    std::getline(std::cin, temp.diagnostico);

    patients.push_back(temp);
    std::cout << "Paciente agregado correctamente.\n";
}

void mostrarPacientes(const std::vector<Patient>& patients) {
    if (patients.empty()) {
        std::cout << "No hay pacientes registrados.\n";
        return;
    }
    std::cout << "\n--- Lista de Pacientes ---\n";
    for (size_t i = 0; i < patients.size(); ++i) {
        std::cout << i + 1 << ". " << patients[i].name
                  << " | Edad: " << patients[i].age
                  << " | Peso: " << patients[i].weight
                  << " | Altura: " << patients[i].height
                  << " | RFC: " << patients[i].RFC
                  << " | Diagnostico: " << patients[i].diagnostico << '\n';
    }
}

void eliminarPaciente(std::vector<Patient>& patients) {
    if (patients.empty()) {
        std::cout << "No hay pacientes para eliminar.\n";
        return;
    }
    int pos;
    std::cout << "Ingrese el numero del paciente a eliminar: ";
    std::cin >> pos;
    if (pos > 0 && pos <= static_cast<int>(patients.size())) {
        patients.erase(patients.begin() + (pos - 1));
        std::cout << "Paciente eliminado correctamente.\n";
    } else {
        std::cout << "Indice invalido.\n";
    }
}

void modificarPaciente(std::vector<Patient>& patients) {
    if (patients.empty()) {
        std::cout << "No hay pacientes para modificar.\n";
        return;
    }
    int pos;
    std::cout << "Ingrese el numero del paciente a modificar: ";
    std::cin >> pos;
    if (pos <= 0 || pos > static_cast<int>(patients.size())) {
        std::cout << "Indice invalido.\n";
        return;
    }

    Patient& temp = patients[pos - 1];
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar buffer
    std::cout << "Nuevo nombre (actual: " << temp.name << "): ";
    std::getline(std::cin, temp.name);
    std::cout << "Nueva edad (actual: " << temp.age << "): ";
    std::cin >> temp.age;
    std::cout << "Nuevo peso (kg) (actual: " << temp.weight << "): ";
    std::cin >> temp.weight;
    std::cout << "Nueva altura (m) (actual: " << temp.height << "): ";
    std::cin >> temp.height;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar buffer
    std::cout << "Nuevo RFC (actual: " << temp.RFC << "): ";
    std::getline(std::cin, temp.RFC);
    std::cout << "Nuevo diagnostico (actual: " << temp.diagnostico << "): ";
    std::getline(std::cin, temp.diagnostico);

    std::cout << "Paciente modificado correctamente.\n";
}