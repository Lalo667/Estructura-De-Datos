const readline = require("readline");

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
}); //Siempre va a ir esta linea de codgio

function preguntar(pregunta) {
    return new Promise(resolve => rl.question(pregunta, ans => resolve(ans)));
}

async function main() {
    let n = parseInt(await preguntar("¿De qué tamaño será la matriz?: "));

    let lista = new Array(n).fill(0);

    for (let i = 0; i < n; i++) {
        lista[i] = parseInt(await preguntar(`Ingrese el valor para la posición ${i + 1}: `));
    }

    // Pedimos el valor a buscar
    let valor = parseInt(await preguntar("Ingrese el valor que desea buscar: "));

    // Búsqueda lineal
    let encontrado = false; 
    let posicion = -1;

    for (let i = 0; i < n; i++) {
        if (lista[i] === valor) {
            encontrado = true;
            posicion = i;
            break;
        }
    }

    if (encontrado) {
        console.log(`✅ El valor ${valor} se encuentra en la posición ${posicion + 1}.`);
    } else {
        console.log(`❌ El valor ${valor} no se encuentra en la lista.`);
    }

    rl.close();
}

main();
