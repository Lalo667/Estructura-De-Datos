function merge(a, l, m, r) {
  let a1 = m - l + 1;
  let a2 = r - m;

  let L = new Array(a1);
  let R = new Array(a2);

  for (let j = 0; j < a1; j++) L[j] = a[l + j];
  for (let k = 0; k < a2; k++) R[k] = a[m + 1 + k];

  let i = 0, j = 0, k = l;

  while (i < a1 && j < a2) {
    if (L[i] <= R[j]) {
      a[k] = L[i];
      i++;
    } else {
      a[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < a1) {
    a[k] = L[i];
    i++;
    k++;
  }

  while (j < a2) {
    a[k] = R[j];
    j++;
    k++;
  }
}

function mergeSort(a, l, r) {
  if (l < r) {
    let m = l + Math.floor((r - l) / 2);
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    merge(a, l, m, r);
  }
}

let a = [39, 28, 44, 11];
let s = a.length;

console.log("Antes de ordenar:", a.join(" "));
mergeSort(a, 0, s - 1);
console.log("Después de ordenar:", a.join(" "));
