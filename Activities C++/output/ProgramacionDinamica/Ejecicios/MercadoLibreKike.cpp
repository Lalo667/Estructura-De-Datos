#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Función para convertir texto a minúsculas
string convertirMinusculas(string texto) {
    string resul = texto;
    for (int i = 0; i < resul.length(); i++) {
        resul[i] = tolower(resul[i]);
    }
    return resul;
}

// Función para verificar si contiene palabras clave de contacto externo
bool cont_contac_ext(string texto) {
    string txt_min = convertirMinusculas(texto);

    // Palabras clave de redes sociales y aplicaciones de mensajería
    string pal_clave[] = {
        "whatsapp", "whats app", "wpp", "telegram", "facebook", 
        "instagram", "twitter", "email", "correo", "gmail",
        "hotmail", "outlook", "yahoo", "skype", "discord",
        "snapchat", "tiktok", "messenger", "signal","telegram" 
        
    };

    for (int i = 0; i < 19; i++) {
        if (txt_min.find(pal_clave[i]) != string::npos) {
            return true;
        }
    }

    return false;
}

// Función para detectar números de teléfono (10 dígitos consecutivos)
bool cont_tel(string texto) {
    int conta_digitos = 0;

    for (int i = 0; i < texto.length(); i++) {
        if (isdigit(texto[i])) {
            conta_digitos++;
            if (conta_digitos >= 10) {
                return true;
            }
        } else {
            conta_digitos = 0;
        }
    }

    return false;
}

// Función para detectar emails disfrazados
bool cont_correo(string texto) {
    string txt_min = convertirMinusculas(texto);

    // Buscar patrones de email disfrazado
    if (txt_min.find("arroba") != string::npos || 
        txt_min.find("@") != string::npos) {
        return true;
    }

    if (txt_min.find("punto") != string::npos && 
        (txt_min.find("com") != string::npos || 
         txt_min.find("edu") != string::npos ||
         txt_min.find("mx") != string::npos ||
         txt_min.find("org") != string::npos)) {
        return true;
    }

    return false;
}

// Función para detectar números escritos en palabras
bool cont_num_escrit(string texto) {
    string txt_min = convertirMinusculas(texto);

    string numerosTexto[] = {
        "cero", "uno", "dos", "tres", "cuatro", "cinco",
        "seis", "siete", "ocho", "nueve", "diez",
        "once", "doce", "trece", "catorce", "quince",
        "veinte", "treinta", "cuarenta", "cincuenta",
        "sesenta", "setenta", "ochenta", "noventa"
    };

    int conta = 0;
    for (int i = 0; i < 24; i++) {
        size_t pos = 0;
        while ((pos = txt_min.find(numerosTexto[i], pos)) != string::npos) {
            conta++;
            pos += numerosTexto[i].length();
        }
    }

    // Si hay más de 4 números escritos, probablemente es un teléfono disfrazado
    if (conta >= 4) {
        return true;
    }

    return false;
}

// Función para detectar frases de contacto externo
bool contieneFraseContactoExterno(string texto) {
    string textoMin = convertirMinusculas(texto);

    string frases[] = {
        "por fuera", "fuera de mercado libre", "contactame",
        "localizarme", "llamame", "marcame", "escribeme",
        "te envio mi", "mi numero", "mi telefono",
        "hacemos el trato", "fuera de aqui", "por otro lado","mensaje",
        "ig", "face","whats","insta"
    };

    for (int i = 0; i < 17; i++) {
        if (textoMin.find(frases[i]) != string::npos) {
            return true;
        }
    }

    return false;
}


bool validarMensaje(string mensaje) {
    if (cont_contac_ext(mensaje)) {
        return false;
    }

    if (cont_tel(mensaje)) {
        return false;
    }

    if (cont_correo(mensaje)) {
        return false;
    }

    if (cont_num_escrit(mensaje)) {
        return false;
    }

    if (contieneFraseContactoExterno(mensaje)) {
        return false;
    }

    return true;
}

int main() {
    string msj;
    char conti;



    do {
        cout << ">>>>>> BLOQUEO DE SEGURIDAD <<<<<<" << endl;
        cout << "Ingrese el mensaje:" << endl;
        getline(cin, msj);

        cout << "\n---- VALIDACION ----" << endl;
        cout << "Mensaje ingresado: " << msj << endl << endl;

    if (validarMensaje(msj)) {
        cout << "EL MENSAJE ES VALIDO Y CUMPLE CON LAS NORMAS" << endl;
    } else {
        cout << "EL MENSAJE ES INVALIDO YA BAILASTE" << endl;
        cout << "NIMODOS" << endl;
    }

        cout << "Acaso desea verificar otro mensaje o nadota? (s/n): ";
        cin >> conti;
        cin.ignore();
        cout << endl;

    } while (conti == 's' || conti == 'S');

    cout << "RIFADO FELICIDADES!" << endl;

    return 0;
}