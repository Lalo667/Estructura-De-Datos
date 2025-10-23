#include <iostream>

    using namespace std;
    int main(){
        int TotalBilletes = 0;
        int TotalMonedas = 0;
        float Money;
        float MoneyDollar;
        int Billete20;
        int Billete50;
        int Billete100;
        int Billete200;
        int Billete500;
        int Billete1000;

        float monedas050(0);
        int monedas1;
        int monedas2;
        int monedas5;
        int monedas10;
        float MoneyEuro;

        float dllar = 18.36;
        float Euro = 21.25;

        
        cout << "Ingrese la cantidad de billetes de 20:" << '\n';
                cin >>Billete20;
                    Billete20 = Billete20 + (20 * Billete20);

        cout << "Ingrese la cantidad de billetes de 50:" << '\n';
                cin >> Billete50;
                    Billete50 = (50 * Billete50);
                    
        cout << "Ingrese la cantidad de billetes de 100:" << '\n'; 
                cin >> Billete100;
                    Billete100 = Billete100;
                Billete100 = Billete100 + (100 * Billete100);

           cout << "Ingrese la cantidad de billetes de 200:" << '\n';
                cin >> Billete200;
                Billete200 = Billete200 + (200 * Billete200);

            cout << "Ingrese la cantidad de billetes de 500:" << '\n';
                cin >> Billete500;
                Billete500 = Billete500 + (500 * Billete500);

             cout << "Ingrese la cantidad de billetes de 1000:" << '\n';
                 cin >> Billete1000;
                    Billete1000 = (1000 * Billete1000);
            TotalBilletes = Billete1000 + Billete500 + Billete200 + Billete100 + Billete50 + Billete20; 

            cout << "Ingrese la cantidad de monedas de .50:" << '\n';
              cin >> monedas050;
                    monedas050 = (0.50 * monedas050);

        cout << "Ingrese la cantidad de monedas de 1:" << '\n';
                cin >> monedas1;
                    monedas1 = (1 * monedas1);
        cout << "Ingrese la cantidad de monedas de 2:" << '\n';
                cin >> monedas2;
                    monedas2 = (2 * monedas2);

           cout << "Ingrese la cantidad de monedas 5:" << '\n';
                cin >> monedas5;
                    monedas5 = (5 * monedas5);

            cout << "Ingrese la cantidad de monedas de 10:" << '\n';
                cin >> monedas10;
                    monedas10 = (10 * monedas10);
            TotalMonedas = monedas050 + monedas1 + monedas2 + monedas5 + monedas10;
            Money = TotalMonedas + TotalBilletes;
            MoneyDollar = Money / dllar;
            MoneyEuro = Money / Euro;

             cout << "El total del dinero en billetes es: " << TotalBilletes << '\n';
              cout << "El total del dinero en monedas es: " << TotalMonedas << '\n';
               cout << "El dinero total es: " << Money << '\n';
                cout << "La cantidad en dolares es: " << MoneyDollar << '\n';
                 cout << "La cantidad en euros es: " << MoneyEuro << '\n';

        return 0 ;
    }