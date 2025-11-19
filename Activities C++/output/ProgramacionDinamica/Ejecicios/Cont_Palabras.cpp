#include <iostream>
#include <string>
#include <cctype> // para tolower
using namespace std;

class AnalizadorTexto {
private:
    string frase; // Frase a analizar

public:
    AnalizadorTexto(string texto) {
        frase = texto;
    }

    int contarPalabras() {
        int contador = 0;
        bool enPalabra = false;
        
        for (size_t i = 0; i < frase.length(); i++) {
            // Si encontramos un carácter que no es espacio
            if (frase[i] != ' ' && frase[i] != '\t' && frase[i] != '\n') { // Si frase no es igual a espacio y tabulador y salto de linea
                if (!enPalabra) { // Si no estamos en una palabra
                    contador++;
                    enPalabra = true;
                }
            } else {
                enPalabra = false;
            }
        }
        
        return contador;
    }

    int buscaLetra(char letra) {
        int contador = 0;
        char letraMinuscula = tolower(letra); // Convertir a minúscula para comparación insensible a mayúsculas/minúsculas
        
        for (size_t i = 0; i < frase.length(); i++) {
            if (tolower(frase[i]) == letraMinuscula) { // si la letra en minúscula es igual a la letra buscada
                contador++; // Incrementar contador
            }
        }
        
        return contador;
    }

    string getFrase() { // Obtener la frase completa, a la hora de escribirlo en la terminal
        return frase;
    }
}; // Fin de la clase AnalizadorTexto

int main() {
    string frase;
    char letra;
    char continuar;

    cout << "Practica No 2.2" << endl;
    cout << "\nIngrese una oracion: ";
    getline(cin, frase); // Leer frase completa con espacios

    AnalizadorTexto analizador(frase); // Crear objeto AnalizadorTexto

    int numPalabras = analizador.contarPalabras(); // Contar palabras en la frase
    cout << "\nLa frase \"" << frase << "\" contiene " << numPalabras << " palabra(s)." << endl;

    do {
        cout << "\nDesea buscar una letra en la frase? (s/n): ";
        cin >> continuar;

        if (continuar == 's' || continuar == 'S') {
            cout << "Ingrese la letra a buscar: ";
            cin >> letra;

            int ocurrencias = analizador.buscaLetra(letra); // Buscar la letra en la frase
            cout << "La letra '" << letra << "' aparece " << ocurrencias << " veces en la frase." << endl;
        }

    } while (continuar == 's' || continuar == 'S');

    cout << "\nthankyou por usarme!" << endl;

    return 0;
}