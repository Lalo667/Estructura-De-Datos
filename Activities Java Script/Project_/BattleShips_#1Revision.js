// # 1ra Revision. 26/09/2025
const readline = require('readline'); // podemos llenar o escribir en la terminal

const board_size = 10;
const wata = '~'
const ships = [
    { id: 1, name: 'Acorazado', size: 4, char: 'A' },
    { id: 2, name: 'Submarino', size: 3, char: 'S' },
    { id: 3, name: 'Destructor', size: 3, char: 'D' },
    { id: 4, name: 'Lancha', size: 2, char: 'L' }
];

function createBoard() {
    return Array.from({ length:board_size }, () => Array(board_size).fill(wata)); // Regresa un arreglo bidimensional , el cual lo llena de agua de tamaño 10
}

function printBoard(board, reveal = true) {
  const cellWidth = 2; 
  let header = '    ';

  // Números de columna
  for (let c = 1; c <= board_size; c++) {
    header += String(c).padStart(cellWidth, ' ') + ' ';
  }
  console.log(header);

  // Filas
  for (let r = 0; r < board_size; r++) {
    let rowStr = String(r + 1).padStart(2, ' ') + '  ';
    for (let c = 0; c < board_size; c++) {
      const cell = board[r][c];
      let valor;
      if (reveal) {
        valor = cell;
      } else {
        valor = (cell === wata) ? wata : '?';
      }
      rowStr += valor.padStart(cellWidth, ' ') + ' ';
    }
    console.log(rowStr); // 
  }
} 


function canplace(board, row, col, orientation, size){
    if(orientation === 'H'){
        if (col + size > board_size) return false; //Esto evita que el barco sobrepase el límite derecho del tablero.
            for(let i = 0; i < size; i++) 
                if (board[row][col + i] !== wata) 
                return false;

    } else {
        if (row + size > board_size ) //Recorre la celda en donde ira el barco
            return false;
        for (let i =0; i < size; i++) 
                if (board[row + i][col] !== wata) // si alguno es diferente a wata, entonces regresara un falso
            return false;
    }
    return true;
}
function placeAt(board, row, col, orientation, ship){
    for (let i = 0; i < ship.size; i++){
        if(orientation === 'H') board[row][col + i] = ship.char;
        else board[row + i] [col] = ship.char;
    }
}

const rl = readline.createInterface({ input: process.stdin, output: process.stdout});
function ask(q) { return new Promise(resolve => rl.question(q,a => resolve(a))); }

async function chooseShip(placed){
    console.log('\nSelecciona un barco para colocar' );
    ships.forEach( s => {
    console.log(`${s.id}) ${s.name} (tamaño ${s.size}) ${placed.has(s.id) ? '(colocado)' : ''}`);
    });
  console.log('0) Salir');

    while (true){
        const ans = (await ask('Numero del barco: ')).trim();
        const n = parseInt(ans,10);
        switch (n){
            case 0:
                return null;
            case 1:
                if (!placed.has(1)) return ships[0];
                break;
            case 2:
                if (!placed.has(2)) return ships[1];
                break;
            case 3:
                if (!placed.has(3)) return ships[2];
                break;
            case 4:
            if (!placed.has(4)) return ships[3];
            break;
            default:
                break;
        }
        console.log('Seleccion invalida o barco ya colocado. Intente otra vez');
    }
}

async function promptOrientation(){
    while(true){
    const o = (await ask('La Orientación (H = horizontal, V = vertical): ')).trim().toUpperCase();
    if (o === 'H' || o === 'V') return o;
    console.log('Entrada inválida. Escribe H o V.');
  }
}

async function promptCoordinates(){
    while(true){
        const r = await ask('Fila 1 - 10: ');
        const c = await ask('Columnas 1 - 10: ');
        const row = parseInt(r,10) - 1;
        const col = parseInt(c,10) - 1;
        if (Number.isInteger(row) && Number.isInteger(col) && row >= 0 && row < board_size && col >= 0 && col < board_size) {
        return { row, col };   
         }
        console.log('Coordenadas stan mal');
    }
}

async function PromptPlaceShip(board, ship){
    while (true) {
        console.log(`\nColocando: ${ship.name} (size ${ship.size})`);
        const orientation = await promptOrientation();
        const {row,col } = await promptCoordinates();
            if (canplace(board, row, col, orientation, ship.size)) {
            placeAt(board, row, col, orientation, ship);
        console.log (`${ship.name} colocado en fila ${row + 1}, columna ${col + 1}, ${orientation === 'H' ? 'horizontal' : 'vertical'}.`);
      return true;
            }  else {
                console.log('No cabe ahi o hay otro barco');
                const retry = (await ask('Intenta otra vez? S/n: ')).trim().toLowerCase();
                if (retry === 'n') return false;
            }   
    }
}

(async () => {
    console.clear();
    console.log('battleshipss');

    const board = createBoard();
    const placed = new Set()

    while(placed.size < ships.length){
        printBoard(board);
        const ship = await chooseShip(placed);
        if (!ship) break;

        const placedOK = await PromptPlaceShip(board, ship);
        if (placedOK) placed.add(ship.id);
    }
    console.log('\nTodos los barcos colocados: ');
    printBoard(board, true);
    rl.close();
    
}) ();
