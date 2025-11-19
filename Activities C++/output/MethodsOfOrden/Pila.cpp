#include <iostream> // Para operaciones de entrada/salida
#define MAX_SIZE 100 // Tamaño máximo de la pila

int stack[MAX_SIZE]; // Arreglo para almacenar los elementos de la pila
int top = -1;        // Índice del elemento superior de la pila

// Función para agregar un elemento a la pila
void push(int item) {
    if (top == MAX_SIZE - 1) { // Verifica si la pila está llena
        std::cout << "Stack Overflow" << std::endl; // Mensaje de error
        return; // Sale de la función
    }
    stack[++top] = item; // Incrementa el índice y agrega el elemento
}

// Función para eliminar y retornar el elemento superior de la pila
int pop() {
    if (top == -1) { // Verifica si la pila está vacía
        std::cout << "Stack Underflow" << std::endl; // Mensaje de error
        return -1; // Retorna -1 para indicar que la pila está vacía
    }
    return stack[top--]; // Retorna el elemento superior y decrementa el índice
}

// Función para ver el elemento superior sin eliminarlo
int peek() {
    if (top == -1) { // Verifica si la pila está vacía
        std::cout << "Pila vacía" << std::endl; // Mensaje de error
        return -1; // Retorna -1 para indicar que la pila está vacía
    }
    return stack[top]; // Retorna el elemento superior sin modificar el índice
}

// Función para verificar si la pila está vacía
bool isEmpty() {
    return top == -1; // Retorna true si está vacía, false en caso contrario
}

// Función para verificar si la pila está llena
bool isFull() {
    return top == MAX_SIZE - 1; // Retorna true si está llena
}

int main() {
    push(10); // Agrega elementos a la pila
    push(20);
    push(30);

    std::cout << "Elemento Superior: " << peek() << std::endl; // Muestra el elemento superior
    std::cout << "Extrae elemento: " << pop() << std::endl;     // Elimina y muestra el elemento superior
    std::cout << "Elemento Superior: " << peek() << std::endl;  // Muestra el nuevo elemento superior

    return 0;
}
