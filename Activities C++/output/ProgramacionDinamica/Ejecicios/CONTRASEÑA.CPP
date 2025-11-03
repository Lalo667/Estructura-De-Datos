#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool tieneNumeros(const string& pass) {
    for (char c : pass) {  //  Recorre cada carácter de la cadena pass.
        if (isdigit(c))  // Es una función de <cctype> que retorna true si c es un dígito numérico
    return true;
    }
    return false;
}
    // Hara lo mismo para minusculas, mayusculas y caracteres especiales

bool tieneMinusculas(const string& pass) {
    for (char c : pass) {
        if (islower(c)) return true;
    }
    return false;
}

bool tieneMayusculas(const string& pass) {
    for (char c : pass) {
        if (isupper(c)) return true;
    }
    return false;
}

bool tieneCaracterEspecial(const string& pass) {
    for (char c : pass) {
        if (!isalnum(c)) return true;
    }
    return false;
}

 // Verifica si la contraseña contiene solo letras o solo números
bool soloLetrasONumeros(const string& pass) {
    bool soloNumeros = true;
    bool soloLetras = true;
    //| Recorre cada carácter de la cadena pass.
    for (char c : pass) {
        if (!isdigit(c)) soloNumeros = false;
        if (!isalpha(c)) soloLetras = false;  // isalpha verifica si el carácter es una letra (mayúscula o minúscula)
    }
    
    return soloNumeros || soloLetras;
}

string evaluarSeguridad(const string& pass) {
    int longitud = pass.length();
    bool numeros = tieneNumeros(pass);
    bool minusculas = tieneMinusculas(pass);
    bool mayusculas = tieneMayusculas(pass);
    bool especial = tieneCaracterEspecial(pass);
    
    if (soloLetrasONumeros(pass)) {
        return "BAJO";
    }
    
    if (longitud >= 8 && numeros && mayusculas && minusculas && especial) {
        return "ALTO";
    }
    
    if (numeros && (mayusculas || minusculas) && especial) {
        return "MEDIO";
    }
    
    return "BAJO";
}

int main() {
    string nombre, password, passwordConfirm;
    
    cout << "=== LOGIIN ===" << endl;
    cout << endl;

    cout << "Ingrese su nombre: ";
    getline(cin, nombre);
    

    cout << "Ingrese su contraseña: ";
    getline(cin, password);
    
    // Confirmar contraseña
    cout << "Repita su contraseña: ";
    getline(cin, passwordConfirm);
    
    cout << endl;
    
    if (password != passwordConfirm) {
        cout << "ERROR: Las contraseñas no coinciden." << endl;
        return 1;
    }
    
    string nivelSeguridad = evaluarSeguridad(password);
    
    cout << "=== RESULTADO DEL REGISTRO ===" << endl;
    cout << "Usuario: " << nombre << endl;
    cout << "Nivel de seguridad de la contraseña: " << nivelSeguridad << endl;
    cout << endl;
    
    if (nivelSeguridad == "BAJO") {
        cout << "⚠️  RECOMENDACIÓN: Su contrasena es débil." << endl;
        cout << "   Para mejorarla incluya números, letras mayúsculas," << endl;
        cout << "   minúsculas y caracteres especiales (!@#$%&*)." << endl;
    } else if (nivelSeguridad == "MEDIO") {
        cout << "⚡ RECOMENDACIÓN: Su contrasena es aceptable." << endl;
        cout << "   Para hacerla más segura use al menos 8 caracteres." << endl;
    } else {
        cout << "✓ ¡Excelente! Su contrasena es muy segura." << endl;
    }
    
    return 0;
}