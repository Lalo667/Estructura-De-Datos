#ifndef PACIENTE_H // Definicion de la interfaz de paciente sirve para evitar inclusiones multiples que son errores comunes en C++
#define PACIENTE_H 

#include <iostream> 
#include <vector> // Libreria para usar el contenedor vector
#include <string> // Libreria para usar cadenas de texto
using namespace std;

struct Patient { // Struct es una estructura de datos que agrupa diferentes tipos de datos bajo un mismo nombre
    string name;
    int age;
    int rfc;
    float weight;
    float height;
    string gender;
};
//vector<Patient> patients; // Sirve para almacenar multiples pacientes
//const vector<Patient>& patients // El const 

void showMenu(); 
void agregarPaciente(vector<Patient>& patients);
void buscarPaciente(const vector<Patient>& patients);
void eliminarPaciente(vector<Patient>& patients);
void mostrarPacientes(const vector<Patient>& patients);
void modificarPaciente(vector<Patient>& patients);

#endif // PACIENTE_H
