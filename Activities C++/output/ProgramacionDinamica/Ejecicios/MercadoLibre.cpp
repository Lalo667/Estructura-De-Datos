#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <cctype>

using namespace std;

class ValidadorMensajeML {
private:
    string palabrasSospechosas[15] = {
        "whatsapp", "whats", "wsp", "email", "correo", "telefono",
        "celular", "contactame", "contactar", "llamame", "llamar",
        "fuera de mercado", "por fuera", "directo", "aparte",
    };
    
    string numerosTexto[10] = {
        "cero", "uno", "dos", "tres", "cuatro",
        "cinco", "seis", "siete", "ocho", "nueve"
    };
    
    // Convierte string a minúsculas
    string aMinusculas(string texto) {
        transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
        return texto;
    }
    
    // Extrae solo los dígitos de un string
    string extraerDigitos(const string& texto) {
        string digitos = "";
        for (char c : texto) {
            if (isdigit(c)) {
                digitos += c;
            }
        }
        return digitos;
    }
    
    // Detecta teléfono (mejorado para detectar más patrones)
    bool detectarTelefono(const string& mensaje) {
        // Extrae todos los dígitos del mensaje
        string soloDigitos = extraerDigitos(mensaje);
        
        // Si hay 10 o más dígitos consecutivos (considerando el mensaje completo),
        // es muy probable que sea un teléfono
        if (soloDigitos.length() >= 10) {
            return true;
        }
        
        // Patrón flexible: detecta números separados por espacios, guiones, puntos
        // Ejemplos: "66 88 67 75 34", "668-867-6787", "66.88.67.75.34"
        regex patronFlexible("\\d{2,}[\\s\\-.]?\\d{2,}[\\s\\-.]?\\d{2,}");
        if (regex_search(mensaje, patronFlexible)) {
            // Verifica que la coincidencia tenga suficientes dígitos
            smatch match;
            if (regex_search(mensaje, match, patronFlexible)) {
                string coincidencia = match.str();
                string digitosCoincidencia = extraerDigitos(coincidencia);
                if (digitosCoincidencia.length() >= 8) {
                    return true;
                }
            }
        }
        
        // Patrón tradicional para 10 dígitos con separadores
        regex patronTradicional("\\d{3}[\\s\\-.]?\\d{3}[\\s\\-.]?\\d{4}");
        if (regex_search(mensaje, patronTradicional)) {
            return true;
        }
        
        // Detecta secuencias de números separados por espacios simples
        // Ejemplo: "66 88 67 75 34" o "668 67 67 87"
        regex patronEspaciado("(?:\\d{2,}\\s+){2,}\\d{2,}");
        if (regex_search(mensaje, patronEspaciado)) {
            smatch match;
            if (regex_search(mensaje, match, patronEspaciado)) {
                string coincidencia = match.str();
                string digitosCoincidencia = extraerDigitos(coincidencia);
                // Si tiene 8 o más dígitos, es sospechoso
                if (digitosCoincidencia.length() >= 8) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    // Detecta emails explícitos o disfrazados
    bool detectarEmail(const string& mensaje) {
        string msgMin = aMinusculas(mensaje);
        
        // Email normal: ejemplo@dominio.com
        regex patronEmail("\\b[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}\\b");
        if (regex_search(mensaje, patronEmail)) {
            return true;
        }
        
        // Email disfrazado: "usuario arroba dominio punto com"
        if (msgMin.find("arroba") != string::npos || 
            msgMin.find(" at ") != string::npos) {
            if (msgMin.find("punto") != string::npos || 
                msgMin.find(" dot ") != string::npos) {
                return true;
            }
        }
        
        return false;
    }
    
    // Detecta números disfrazados en texto
    bool detectarNumerosDisfrazados(const string& mensaje) {
        string msgMin = aMinusculas(mensaje);
        int contadorNumeros = 0;
        
        for (int i = 0; i < 10; i++) {
            size_t pos = 0;
            while ((pos = msgMin.find(numerosTexto[i], pos)) != string::npos) {
                contadorNumeros++;
                pos += numerosTexto[i].length();
            }
        }
        
        return contadorNumeros >= 6;
    }
    
    // Detecta patrones de tiempo sospechosos que ocultan números
    bool detectarTiempoSospechoso(const string& mensaje) {
        string msgMin = aMinusculas(mensaje);
        
        regex patronTiempo("\\d{2,3}\\s*(horas?|minutos?|segundos?)");
        
        auto palabras_begin = sregex_iterator(mensaje.begin(), mensaje.end(), patronTiempo);
        auto palabras_end = sregex_iterator();
        int conteo = distance(palabras_begin, palabras_end);
        
        return conteo >= 3;
    }
    
    // Detecta palabras clave sospechosas
    bool detectarPalabrasClave(const string& mensaje) {
        string msgMin = aMinusculas(mensaje);
        
        for (int i = 0; i < 15; i++) {
            if (msgMin.find(palabrasSospechosas[i]) != string::npos) {
                return true;
            }
        }
        
        return false;
    }
    
public:
    // Valida el mensaje completo
    bool validarMensaje(const string& mensaje, string& razonRechazo) {
        // Detectar teléfono
        if (detectarTelefono(mensaje)) {
            razonRechazo = "Contiene un numero de telefono";
            return false;
        }
        
        // Detectar email
        if (detectarEmail(mensaje)) {
            razonRechazo = "Contiene un email (explicito o disfrazado)";
            return false;
        }
        
        // Detectar números disfrazados en texto
        if (detectarNumerosDisfrazados(mensaje)) {
            razonRechazo = "Contiene numeros escritos en texto (posible telefono disfrazado)";
            return false;
        }
        
        // Detectar tiempo sospechoso
        if (detectarTiempoSospechoso(mensaje)) {
            razonRechazo = "Contiene patron de tiempo sospechoso (numeros disfrazados)";
            return false;
        }
        
        // Detectar palabras clave sospechosas
        if (detectarPalabrasClave(mensaje)) {
            razonRechazo = "Contiene palabras clave que sugieren comunicacion fuera de la plataforma";
            return false;
        }
        
        return true;
    }
};

int main() {
    ValidadorMensajeML validador;
    string mensaje;
    string razon;
    
    cout << "VALIDADOR DE MENSAJES DE MERCADO LIBRE" << endl;
    cout << "Este programa detecta intentos de comunicacion fuera de la plataforma\n" << endl;
    
    while (true) {
        cout << "\nIngrese el mensaje (o 'salir' para terminar):" << endl;
        cout << "> ";
        getline(cin, mensaje);
        
        if (mensaje == "salir" || mensaje == "SALIR") {
            cout << "\nGracias por usar el validador. Hasta pronto!" << endl;
            break;
        }
        
        if (mensaje.empty()) {
            cout << "Por favor ingrese un mensaje valido." << endl;
            continue;
        }
        
        cout << "\n--- RESULTADO DE LA VALIDACION ---" << endl;
        
        if (validador.validarMensaje(mensaje, razon)) {
            cout << "✓ MENSAJE VALIDO" << endl;
            cout << "El mensaje cumple con las politicas de comunicacion de Mercado Libre." << endl;
        } else {
            cout << "✗ MENSAJE INVALIDO" << endl;
            cout << "Razon: " << razon << endl;
            cout << "Este mensaje viola las politicas de comunicacion de Mercado Libre." << endl;
        }
        cout << "-----------------------------------" << endl;
    }
    
    return 0;
}