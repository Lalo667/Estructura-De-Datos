const readline = require("readline");

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
}); //Siempre va a ir esta linea de codgio

let n;            // Variable para el tamaño del arreglo y el let permite cambiar su valor
let matriz = [];  // Arreglo para almacenar los valores y el let permite cambiar su valor

function main() {
    rl.question("Ingrese el tamaño del arreglo: ", (respuesta) => { // Escribimos la pregunta y respuesta es la variable que guarda lo que el usuario ingresa
        n = parseInt(respuesta); //La respuesta se convierte a entero y despues se guarda en n que es el tamaño del arreglo

        if (isNaN(n) || n <= 0) {  //is NaN significa "is Not a Number" o es menor o igual a 0 no creara el arreglo
            console.log("Error: Ingresa un número válido."); //a cambio creara esta csa
            return main(); //y volvera a llamar a la funcion main que es es donde se pide el tamaño del arreglo
        }

        matriz = new Array(n).fill(0); // Matriz es igual al nuevo arreglo de tamaño n y se llena con ceros

        pedirValores(0); // Iniciamos la función para pedir los valores, comenzando en el índice 0
    });
}

function pedirValores(i) { // Funcion para pedir los valores y recibe el indice i
    if (i < n) { // entonces si i es menor a n entonces
        rl.question(`Dame el valor ${i + 1}: `, (valor) => { //pregunta el valor i+1 y lo guarda en la variable valor
            let num = parseInt(valor); // Convierte la respuesta a entero y lo guarda en num

            if (isNaN(num)) { // Si num no es un numero
                console.log("Error: Ingresa un número válido.");
                return pedirValores(i); // c regresa a la funcion de PedirValores con el mismo indice i
            }

            matriz[i] = num; // Si es un numero lo guarda en la posicion i del arreglo matriz entonces es igual a num que es el numero ingresado
            pedirValores(i + 1); // Pasamos al siguiente índice
        });
    } else {
        console.log("\nLos valores que ingresaste son:");
        console.log(matriz.join(" ")); // Muestra los valores ingresados separados por espacio

        pedirPosicion(); // Llamamos a la función para pedir la posición donde insertar
    }
}

function pedirPosicion() {
    rl.question(`\nDame la posición donde insertar (1-${n}): `, (respuesta) => { // Pregunta la posición donde insertar y la guarda en respuesta
        let pos = parseInt(respuesta); // Convierte la respuesta a entero y la guarda en pos

        if (isNaN(pos) || pos < 1 || pos > n) { // Si pos no es un numero o es menor a 1 o mayor a n
            console.log("⚠️ Error: Posición inválida.");
            return pedirPosicion();
        }
        pedirNuevoValor(pos);
    });
}

function pedirNuevoValor(pos) { // Funcion para pedir el nuevo valor y recibe la posicion pos
    rl.question("Dame el nuevo valor: ", (respuesta) => { // Pregunta el nuevo valor y lo guarda en respuesta   
        let valor = parseInt(respuesta); // Convierte la respuesta a entero y la guarda en valor    

        if (isNaN(valor)) { // Valida si valor es un numero
            console.log("Ingresa un número válido.");
            return pedirNuevoValor(pos);
        }

        for (let i = n - 1; i > pos - 1; i--) { // Para i que es igual a n-1 (ultimo indice) hasta que i sea mayor a pos-1 (indice donde se insertara) orientado del ultimo al primero
            // Desplazamos los elementos hacia la derecha
            matriz[i] = matriz[i - 1];
        }

        matriz[pos - 1] = valor;

        console.log("\n Valor insertado correctamente.");

        console.log("\nLos valores actualizados son:");
        console.log(matriz.join(" "));

        rl.close(); // Cerramos las entradas de readline
    });
}

// Iniciamos el programa
main();
