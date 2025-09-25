    #include <iostream>
    using namespace std;  
        
        int n1(0);
        int n2(1);
        int resultado(0);
        int num;

    int main() {

        cout << "Ingrese el numero que desee: ";
        cin >> num;

        for(int i = 1; i < num; i++ ){
            resultado = n1 + n2;
            n1 = n2;
            n2 = resultado;
            cout << resultado<< "\n" ;
        }
        
        return 0;
    }