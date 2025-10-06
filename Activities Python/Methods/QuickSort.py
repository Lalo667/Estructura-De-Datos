def partition(a, l, h):
    pvt = a[h]   # Último elemento como pivote
    j = l - 1
    for k in range(l, h):
        if a[k] < pvt:
            j += 1
            swap(a, j, k)
    swap(a, j + 1, h)   # Mueve el pivote a su posición correcta
    return j + 1

def swap(a, j, k):
    a[j], a[k] = a[k], a[j]

def qckSort(a, l, h):
    if l < h:
        pi = partition(a, l, h)
        qckSort(a, l, pi - 1)
        qckSort(a, pi + 1, h)

if __name__ == "__main__":
    a = [10, 7, 8, 9, 1, 5]
    size = len(a)
    print("El arreglo antes de ordenarlo:")
    print(a)

    qckSort(a, 0, size - 1)

    print("El arreglo después de ordenarlo:")
    print(a)