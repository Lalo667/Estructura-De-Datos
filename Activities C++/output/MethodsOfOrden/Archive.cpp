#include <iostream>
#include <fstream>

using namespace std;

int main(){


string texto;
int op;
ofstream escribirArchivo("prueba.txt", ios::app);

if (!escribirArchivo){
    cerr<<"El Archivo no se abrio correctamente"; 
}
do{
    cout <<"Escribe los datos ";
    getline(cin, texto);
    if (texto != "salir"){
        escribirArchivo <<texto <<endl;
    }

} while (texto != "salir");
escribirArchivo.close();

ifstream leerArchivo("prueba.txt");
while (getline(leerArchivo, texto)) {
    cout <<texto << '\n';
}
leerArchivo.close();

return 0;
}