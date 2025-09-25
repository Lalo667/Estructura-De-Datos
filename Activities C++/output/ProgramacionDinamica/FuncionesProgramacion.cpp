# include <iostream> 
using namespace std;

int main()
{
    int opc, num1, num2, num3;
    float residuo;
    char vocal;

    do{
    cout << "******************* MENU DE OPCIONES ********************* " << endl;
    cout << "¨[1]. Par o impar" << endl;
    cout << "¨[2]. Dia de la Semana" << endl;
    cout << "¨[3]. Nombre del mes" << endl;
    cout << "¨[4]. Positivo o negativo" << endl;
    cout << "¨[5]. Mayor que 100" << endl;
    cout << "¨[6]. Vocal o no" << endl;
    cout << "¨[7]. C = A + B" << endl;
    cout << "¨[8]. C = A * B" << endl;
    cout << "¨[9]. SALIR" << endl;
    cout <<"Ingrese un numero:"<< endl;
    cin >> opc;
    
    switch(opc)  {
        
            case 1:
                cout << "Introduzca un numero para conocer s23di es par o impar: ";
                cin >> num1;
                residuo = num1 % 2;
                if (residuo == 0)
                    cout << "Es un numero par" << endl;
                else
                    cout << "Es un numero impar" << endl;
                break; 

                case 2:
                cout <<"Introduzca un numero del 1-7 para conocer el dia de la semana" <<endl;
                cin >> num1;
                switch(num1){
                    case 1: cout << "Lunes"; break;
                    case 2: cout << "Martes"; break;
                    case 3: cout << "Miercoles"; break;
                    case 4: cout << "Jueves"; break;
                    case 5: cout << "Viernes"; break;
                    case 6: cout << "Sabado"; break;
                    case 7: cout << "Domingo"; break;
                    default: cout <<"Introduzca un dia existente";
                }
                break;    
      
                case 3:
                cout << "Introduce un num del 1 al 12: ";
                cin >> num1;
                switch(num1) {
                    case 1: cout << "Enero" << endl; break;
                    case 2: cout << "Febrero" << endl; break;
                    case 3: cout << "Marzo" << endl; break;
                    case 4: cout << "Abril" << endl; break;
                    case 5: cout << "Mayo" << endl; break;
                    case 6: cout << "Junio" << endl; break;
                    case 7: cout << "Julio" << endl; break;
                    case 8: cout << "Agosto" << endl; break;
                    case 9: cout << "Septiembre" << endl; break;
                    case 10: cout << "Octubre" << endl; break;
                    case 11: cout << "Noviembre" << endl; break;
                    case 12: cout << "Diciembre" << endl; break;
                    default: cout << "Numero invalido" << endl; break;
                }
                break;
                
                case 4:
                cout << "Introduzca un numero" << endl;
                cin >> num1;
                if (num1 > 0)
                    cout <<"Es un numero positivo" << endl;
                else if (num1 <0)
                    cout <<"Es un numero negativo" << endl;
                else
                    cout <<"El numero es 0" << endl;
                break;
                
                case 5:
                cout << "Introduce un numero: ";
                cin >> num1;
                if (num1 > 100) cout << "Es mayor que 100" << endl;
                else cout << "No es mayor que 100" << endl;
                break;
                
                case 6: 
                cout << "Introduce una letra: ";
                cin >> vocal;
                if (vocal=='a'||vocal=='e'||vocal=='i'||vocal=='o'||vocal=='u')
                    cout << "Es una vocal" << endl;
                else
                    cout << "No es una vocal" << endl;
                break;
                
                case 7: 
                cout << "Introduce 1er numero: " <<endl;
                cin >> num1;
                cout << "Introduce 2do numero: " <<endl;
                cin >> num2;
                cout << "Introduce 3er numero: " <<endl ;
                cin >> num3;
                if(num1 + num2 == num3)
                    cout <<"La suma del primer y segundo numero, es igual al numero 3" << endl;
                else{
                    cout <<"No es igual al numero 3" << endl;
                }
                break;
                
                case 8: 
                cout << "Introduce 1er numero: " <<endl;
                cin >> num1;
                cout << "Introduce 2do numero: " <<endl;
                cin >> num2;
                cout << "Introduce 3er numero: " <<endl ;
                cin >> num3;
                if(num1 * num2 == num3)
                    cout <<"La multiplicacion del primer y segundo numero, es igual al numero 3" << endl;
                else{
                    cout <<"La multiplicacion No es igual al numero 3" << endl;
                }
                break;
            
                case 9:
                cout << "Saliendo..." << endl;
                break;

                default:
                cout << "Opcion invalida" << endl;
    }
                
                if (opc != 8) {
                cout << "\nPresione ENTER para volver al menu...";
                cin.ignore();
                cin.get();
                }

    }while(opc!=8);

    return 0;
}