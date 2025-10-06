#include <bits/stdc++.h> // es la libreria que contiene la mayoria de librerias en c++ como por ejemplos, vectores, algoritmos,etcs
#include <chrono>
#include <thread>
#include <iostream> // va siempre
using namespace std; // tambien este es clave


const int Rows = 6; // const es que c mantiene fija la matriz, y no cambiara
const int Cols = 7;

class Board { // c define la clase q puede tene^r atributos y metodos como funciones
    private: // c refiere que son inmodificables, por datos externos
        char board[Rows][Cols]; // declaramos la matriz  de tipo bidimensional
        
            void ShowBoard() { // Declaracion de funcion que mostrara en pantalla el tablero, mas adelante
                    cout << endl << "\t\t---------------" << endl; // esto hace que c centre el tablero
                        for(int i= 0; i <Rows; i++) { // Recorrera cada fila del tablero
                            cout<<"\t\t|";  // Es lo q imprimira para delimitar el tablero en la terminal
                            for (int j = 0; j < Cols; j++) { // aca recorrera cada columa del tablero
                             // Si cada celda tiene R o B, la celsa c llenara de ese color
                            if (board[i][j] == 'R') {
                                 cout<< "\x1B[31m" << board[i][j] <<"\033[0m"; // codigo que hace cambiar de color a rojo, y luego lo restablece a normal
                            } else if (board[i][j] == 'B') {
                                 cout<<"\x1B[34m"<<board[i][j]<<"\033[0m"; //  Codigo q tiene la misma funcion, pero en azul
                            } else { // si no tiene ninguna de ambas la celda se pintara en color blanco
                                cout << " "; 
                                }
                                cout << "|";
                            }
                                cout <<endl;
                                cout<<"\t\t---------------"<<endl;
                        }
                                cout<<"\t\t|1|2|3|4|5|6|7|"<<endl;
                                cout<<"\t\t---------------"<<endl;
                                cout<<endl;
                                }
        
                            bool AddElement(int col, char player) { // aca c añadira la ficha al tablero
                                    for(int i = Rows - 1; i >= 0; i--){ // c inicia desde la ultima fila hacia arriba
                                        if (board[i][col] == ' ') { // si, en el tablero encuentra la celda vacia, colocara la ficha
                                        board[i][col] = player; // c hizo la jugada
                                        return true; 
                                        }
                                    }     
                                    return false; // dice q jugada no valida
                                } 

                            //Lee los caracteres y determina si es victoria , devolviendo true , el -4 esta porque si tenemos 7 columnas restara
                            //4 haciendo que sean 3, y es hasta ahi donde leera, ya que si no pondriamos esto y leyera 4 por ejemplo, tendria
                            //que leer 4, cuando la matriz es solo de 7 por lo que se saldria de ella y marcaria error, ahora, cuando llegue a
                            //la posicion 3, y se le sumamos 1, ahora seria 4, y asi sucesivamente, para determinar la victoria, asi funciona
                            //en cada una de las formas para tener victoria

                            bool HorizontalCheck(char player) {   // c declara y recibimos el simbolo del jugador
                               for(int i = 0; i < Rows; i++) {
                                    for(int j = 0; j <= Cols - 4; j++) {   // el -4 es porque mira mas alla de las casillas
                                    if(board[i][j] == player && //en esta valida las primeras 4 posiciones demanera horizontal
                                    board[i][j + 1] == player && 
                                    board[i][j + 2] == player && 
                                    board[i][j + 3] == player) {
                                            return true;
                                        } 
                                    } 
                                } 
                                return false;
                                }

                                bool VerticalCheck(char player)   // Check vertical
                                 { for(int i = 0; i <= Rows - 4; i++)
                                     
                                {
                                    for(int j = 0; j < Cols; j++) 
                                {
                                if(board[i][j] == player && 
                                    board[i + 1][j] == player && 
                                    board[i + 2][j] == player && 
                                    board[i + 3][j] == player) 
                                {
                                        return true;
                                    }
                                    }
                                }
                                return false;
                                }

                                bool LeftDiagonalCheck(char player) 
                                {
                                for(int i = 0; i <= Rows - 4; i++) 
                                {
                                for(int j = 0; j <= Cols - 4; j++) 
                                    {
                                    if(board[i][j] == player && 
                                        board[i + 1][j + 1] == player && 
                                        board[i + 2][j + 2] == player && 
                                        board[i + 3][j + 3] == player) 
                                    {
                                        return true;
                                    }
                                    }
                                }
                                return false;
                                }
                            
                                bool RightDiagonalCheck(char player)
                                {
                                for(int i = 0; i <= Rows - 4; i++) 
                                {
                                    for(int j = 3; j < Cols; j++) 
                                    {
                                    if(board[i][j] == player && 
                                        board[i + 1][j - 1] == player && 
                                        board[i + 2][j - 2] == player && 
                                        board[i + 3][j - 3] == player) 
                                    {
                                        return true;
                                    }
                                    }
                                }
                                return false;
                                }
                                bool checkWin(char player) 
                                {
                                return HorizontalCheck(player) || VerticalCheck(player) ||     
                                LeftDiagonalCheck(player) || RightDiagonalCheck(player);
                                }
                            
                                bool TableroLleno() //Verifica si el tablero sta lleno, devolviendo un falso, y un verdadero si lo esta
                                {
                                    for (int i = 0; i < Rows; i++) {
                                
                                        for(int j= 0; j < Cols; j++){
                                            if(board[i][j] == ' ')
                                            return false;
                                    }
                                }        
                                return true;
                            }

                            void Jugar(){
                                system("clear");
                                    int flag = 1; //Controla de quien es el turno
                                    char player1 ='R';
                                    char player2 ='B';
                                    char columnInput[10];

                            while(!TableroLleno())
                            { // Repitira en bucle mientras que el tablero no este lleno
                                ShowBoard(); //Muestra el tablero
                                if (flag == 1) {
                                cout<<"\x1B[31mEs el turno del Jugador 1 \033[0m"<<endl; //esto cambia el color a rojo en la terminal
                                cout<<"Porfa coloque un numero del 1 al 7 "<< endl;
                                cin >> columnInput;
                                
                                if(strlen(columnInput) != 1 || columnInput[0] < '1' || columnInput[0] > '7') { // c asegura que el caracter que introduzca sea entre el 1 y al 7 solamente
                                    cout<<"tas mal"<<endl;
                                    continue;
                                }
                                int col = columnInput[0] - '1';
                                    if(board[0][col] != ' ') { // Comprueba si la columna esta llena
                                    cout << "Columna llena, elige otra" << endl;
                                continue;
                            }
                                AddElement(col, player1); //llama a la funcion para colocar la ficha del player en el lugar escogido
                                    if(checkWin(player1)) {
                                        system("Clear");
                                        cout<<"\t+-----------------------+"<<endl; //Muestra el mensaje de quien gano
                                        cout<<"\t|     \x1B[31mEl JUgador 1 es el ganador !!!!!!!!!!!!!!!!!!!!!\033[0m      |"<<endl;
                                        cout<<"\t+-----------------------+"<<endl;
                                        ShowBoard();
                                        return;
                                    }
                                    flag = 2;
                                       } else {
                                            cout << "\x1B[34mEs el turno del Jugador 2 \033[0m" << endl;
                                            cout << "Porfa coloque un numero del 1 al 7 " << endl;
                                            cin >> columnInput;
                            
                                            if (strlen(columnInput) != 1 || columnInput[0] < '1' || columnInput[0] > '7') {
                                                cout << "tas mal" << endl;
                                                continue;
                                            }
                                            int col = columnInput[0] - '1';
                                            if (board[0][col] != ' ') {
                                                cout << "Columna llena, elige otra" << endl;
                                                continue;
                                            }
                                            AddElement(col, player2);
                                            if (checkWin(player2)) {
                                                system("clear");
                                                cout << "\t+-----------------------+" << endl;
                                                cout << "\t|  \x1B[34mEl Jugador 2 GANOaldsbañas!!!\033[0m  |" << endl;
                                                cout << "\t+-----------------------+" << endl;
                                                ShowBoard();
                                                return;
                                            }
                                            flag = 1;
                                        }
                                    }
                                    system("clear");
                                    cout<<"+-----------------------+"<<endl;
                                    cout<<"|      empateeeeee       |"<<endl;
                                    cout<<"+-----------------------+"<<endl;
                                    ShowBoard();
                                }
                                
                                public:
                                    Board(){ // inicianiliza el tablero, todo en vacioooooooooooo
                                        for(int i = 0; i < Rows; i++)
                                    {
                                        for(int j =0; j < Cols; j++)
                                    {
                                board[i][j]= ' ';
                            }  
                        }  
                    }
                                     
                void startGame(){
                    Jugar();
                }
            };
                                    
                void GameMenu() {
                Board board;
                char menuchoice[10];

                cout<<"*  1 para iniciar el juego" <<endl;
                cout<<"*  2 para salir del juego" <<endl;
                cout<<" Enter your choice : ";
                cin>>menuchoice;

                if(menuchoice[0] == '1') {
                board.startGame();
                                        
                } else if(menuchoice[0] == '2') {
                    exit(0);
                } else {
                    cout<<" porfa lea, bn"<<endl;
                    GameMenu();
            }
         }

    int main() {
      GameMenu();
      return 0;
    }
