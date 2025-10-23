#include <iostream>
#include <string>
#include <vector>

struct Patient {
    std::string name;
    int age;
    float weight;
    float height;
    std::string gender;
    };

int main(){
    std::vector<Patient> Patients;
    Patient temp;
    //Estructura para los datos de los pacientes

    //Vatiables necesarias
    int menu(0);
    int opc;
    //Mostrar menu de opciones    
    while (menu == 0){
        std::cout << "Seleccione la opcion que desea realizar: " << '\n';
        std::cout << "[1] -- Agregar expediente de paciente" << '\n';
        std::cout << "[2] -- Consultar expediente de paciente" << '\n';
        std::cout << "[3] -- Eliminar expediente existente" << '\n';
        std::cout << "[4] -- Salir del programa " << '\n';
        std::cin >> opc ;

        switch (opc){
        case 1://Agregar pacientes
            for (int i = 0; i < 2; i ++){
                std::cout << "Ingrese el nombre del paciente: " << '\n';
                std::cin >> temp.name;
            }
            break;
        case 2://Mostrar pacientes
            for (int i = 0; i < 2; i ++){
                std::cout << temp.name << '\n';
            }
            break;
        case 3:
            menu = 1;
            break;
        case 4:
            menu = 1;
            break;
        }
    }






return 0;
}