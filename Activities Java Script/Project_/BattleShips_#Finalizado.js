const readline = require('readline');

const board_size = 10;
const wata = '~';
const hitChar = '💥';
const missChar = '•';

const ships = [
  { id: 1, name: 'Acorazado', size: 4, char: 'A' },
  { id: 2, name: 'Submarino', size: 3, char: 'S' },
  { id: 3, name: 'Destructor', size: 3, char: 'D' },
  { id: 4, name: 'Lancha', size: 2, char: 'L' }
];

    // Entonces la funcion create board es para crear un un arreglo de 10 elementos en donde las filas estaran vacias, despues
    // a cada columna le colocaremos agua y devolveremos la fila ya completa de wata

  function createBoard() {
  const board = []; // aquí guardaremos todas las filas
  for (let i = 0; i < board_size; i++) {
    const row = []; // creamos una nueva fila vacía
    for (let j = 0; j < board_size; j++) {
      row.push(wata); // agregamos agua en cada columna
    }
    board.push(row); // añadimos la fila completa al tablero
  }

  return board; // devolvemos la matriz bidimensionl
}

function printBoard(board, reveal = false) {
  const cellWidth = 2; // 2 Representa el numero de caracteres
  let header = '    ';  // Son 4 espacios
  for (let c = 1; c <= board_size; c++) // De 1 hasta q recorra todo el arreglo
  header += String(c).padStart(cellWidth, ' ') + ' ';  // En header c almacenara, los numeros del 1 al 10 que los convertira en tipo string y sera padstar, osea q sera menor de dos caracteres
  console.log(header); // Esto lo imprimira en la consola

  for (let r = 0; r < board_size; r++) { // 
    let rowStr = String(r + 1).padStart(2, ' ') + '  ';
    for (let c = 0; c < board_size; c++) {
      let val = board[r][c];
      if (!reveal && ships.some(s => s.char === val)) val = wata;

      // Colores:
      if (val === hitChar) val = '\x1b[31m' + hitChar + '\x1b[0m'; // para cmbiar el color a rojo
      else if (val === missChar) val = '\x1b[34m' + missChar + '\x1b[0m'; // a azul

      rowStr += val.padStart(cellWidth, ' ') + ' ';
    }
    console.log(rowStr);
  }
}

function canPlace(board, row, col, orientation, size) {
  if (orientation === 'H') {
    if (col + size > board_size) 
        return false; // Suma la columna con el tamaño del barco y si es mayor al tamaño del tablero regresara un falso
    for (let i = 0; i < size; i++)
      if (board[row][col + i] !== wata) 
        return false; //Ahora comprobara las poscisiones del tamaño del barcon con las pos del tablero, si no es igual a agua regresara un falso
  } else {
    if (row + size > board_size) return false; // Lo mismo pero ahora con las filas
    for (let i = 0; i < size; i++)
      if (board[row + i][col] !== wata) return false;
  }
  return true;
}

function placeAt(board, row, col, orientation, ship) {
  for (let i = 0; i < ship.size; i++) {
    if (orientation === 'H') board[row][col + i] = ship.char;
    else board[row + i][col] = ship.char;
  }
}

function randomOrientation() { // Genera una probabilidad aleatoria en 0.1 y o.99, el cual si es menor a 0.5 dara h o v
  return Math.random() < 0.5 ? 'H' : 'V';
}

function randomInt(max) { // Genera un numero entero aleatorio, en donde lo redondeara hacia abajo
  return Math.floor(Math.random() * max);
}

function placeEnemyShips(board) {
  for (const ship of ships) {
    let placed = false;
    while (!placed) {
      const orientation = randomOrientation();
      const row = randomInt(board_size);
      const col = randomInt(board_size);
      if (canPlace(board, row, col, orientation, ship.size)) {
        placeAt(board, row, col, orientation, ship);
        placed = true;
      }
    }
  }
}

function isAllSunk(board) {
  return !board.flat().some(cell =>
    ships.some(s => cell === s.char)
  );
}

function attack(board, row, col) {
  const cell = board[row][col];
  if (ships.some(s => s.char === cell)) {
    board[row][col] = hitChar;
    return 'hit';
  } else if (cell === wata) {
    board[row][col] = missChar;
    return 'miss';
  } else {
    return 'repeat';
  }
}

const rl = readline.createInterface({ input: process.stdin, output: process.stdout }); // Sta funcion es necesariam ya q con ella nos da la capacidad de poder imprimir e ingresar cosas en la terminal
const ask = q => new Promise(r => rl.question(q, a => r(a)));

async function promptCoordinates() {
  while (true) {
    const r = await ask('Fila (1-10): ');
    const c = await ask('Columna (1-10): ');
    const row = parseInt(r, 10) - 1;
    const col = parseInt(c, 10) - 1;
    if (row >= 0 && row < board_size && col >= 0 && col < board_size)
      return { row, col };
    console.log('Coordenadas inválidas.');
  }
}

(async () => { // aca comienza el programa principal
  console.clear();
  console.log('===     BATTLESHIP    ===');

  const playerBoard = createBoard();
  const enemyBoard = createBoard();
  const placed = new Set();

  while (placed.size < ships.length) {
    printBoard(playerBoard, true);
    console.log('\nSelecciona un barco para colocar:');
    ships.forEach(s => console.log(`${s.id}) ${s.name} (${s.size}) ${placed.has(s.id) ? '(colocado)' : ''}`));
    const ans = parseInt(await ask('Número del barco (0 para salir): '), 10);
    if (ans === 0) break;
    const ship = ships.find(s => s.id === ans);
    if (!ship || placed.has(ship.id)) {
      console.log('Barco inválido o ya colocado.');
      continue;
    }
    const o = (await ask('Orientación (H/V): ')).trim().toUpperCase();
    const { row, col } = await promptCoordinates();
    if (canPlace(playerBoard, row, col, o, ship.size)) {
      placeAt(playerBoard, row, col, o, ship);
      placed.add(ship.id);
    } else console.log('No se puede colocar ahí.');
  }

  placeEnemyShips(enemyBoard);
  console.log('\nTodos los barcos colocados. ¡Comienza la batalla!\n');

  while (true) {
    console.log('\nTu tablero:');
    printBoard(playerBoard, true);
    console.log('\nTablero enemigo (oculto):');
    printBoard(enemyBoard, false);

    console.log('\nTu turno:');
    const { row, col } = await promptCoordinates();
    const result = attack(enemyBoard, row, col);
    if (result === 'hit') console.log(' Le diste!');
    else if (result === 'miss') console.log('awa...');
    else console.log('Ya habías disparado ahí.');

    if (isAllSunk(enemyBoard)) {
      console.log('\n Felicidades!!!');
      break;
    }

    let enemyRow, enemyCol, enemyResult;
    do { 
      enemyRow = randomInt(board_size);
      enemyCol = randomInt(board_size);
      enemyResult = attack(playerBoard, enemyRow, enemyCol);
    } while (enemyResult === 'repeat');

    console.log(`\nTe atacaron [${enemyRow + 1}, ${enemyCol + 1}]`);
    if (enemyResult === 'hit') console.log('\x1b[31mEl enemigo acertó tu barco!\x1b[0m');
    else console.log('El enemigo falló.');

    if (isAllSunk(playerBoard)) {
      console.log('\n buuuu. Has perdido.');
      break;
    }
  }

  rl.close();
})();
