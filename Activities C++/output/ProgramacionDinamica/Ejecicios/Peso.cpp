    #include <iostream>

    using namespace std;
        char nombre;
        int edad;
        int peso;
        int altura;
        float masota;
        int opc = 1;

    int main() {
        while(opc == 1){
        cout << "Ingrese su nombre: " << endl;
        cin >> nombre;
        cout << "Ingrese su peso: " << endl;
        cin >> peso;
        cout << "Ingrese su Altura: " << endl;
        cin >> altura;
        masota = peso / (altura * altura);

            cout << "Hola " << nombre << ", tu indice de masa corporal es " << masota << endl;
            if (masota <= 18.4) {
                cout << "tas flaco" <<endl;
            } else if (masota <= 24.4) {
                cout <<"Tas en bien peso" << endl;
            } else if (masota <= 29.9) {
                cout <<("Sobrepeso") << endl;
            } else {
                cout <<("tas obeso") << endl;
            }
            cout << "Desea continuar ? " << endl;
            cin >> opc;
        }
        return 0;
    }
