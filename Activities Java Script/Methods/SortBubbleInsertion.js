// Función de ordenamiento por inserción
function insertionSort(a) {
    for (let i = 1; i < a.length; i++) {
        let temp = a[i];  
        let j = i - 1;

        while (j >= 0 && temp < a[j]) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = temp; 
    }
}

function printArr(a) {
    console.log(a.join(" "));
}

let a = [70, 12, 32, 34, 65];

console.log("Antes de ordenar los elementos del arreglo:");
printArr(a);

insertionSort(a);

console.log("Después de ordenar los elementos del arreglo:");
printArr(a);
