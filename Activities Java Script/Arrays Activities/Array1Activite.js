const readline = require("readline");

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
})

let precios = new Array(3); // arreglo con 3 posiciones
let i = 0;

function pedirPrecio() {
  if (i < precios.length) {
    rl.question(`Dame el precio ${i + 1}: `, (respuesta) => {
      precios[i] = parseInt(respuesta);
      i++;
      pedirPrecio(); // 
    });
  } else {
    console.log("\nLos precios ingresados son:");
    for (let j = 0; j < precios.length; j++) {
      console.log(precios[j]);
    }
    rl.close();
  }
}

pedirPrecio();
