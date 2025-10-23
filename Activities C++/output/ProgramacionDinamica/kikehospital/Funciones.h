#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <vector>

// Estructura de los pacientes
struct Patient {
    std::string name;
    int age;
    float weight;
    float height;
    std::string RFC;
    std::string diagnostico; // puede tener espacios
};

// Declaraciones de funciones
void agregarPaciente(std::vector<Patient>& patients);
void mostrarPacientes(const std::vector<Patient>& patients);
void eliminarPaciente(std::vector<Patient>& patients);
void modificarPaciente(std::vector<Patient>& patients);

#endif