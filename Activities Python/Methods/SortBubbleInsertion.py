def insertionSort(a): #Los elementos del arreglo definida en la funcion
        for i in range(1, len(a)): #Para i, recorrera desde la pos 0 hasta la longitud del arreglo
            temp = a[i] #temp es igual a la pos actual 

            j = i -1 #j es igual a una pos antes de i
            while j >= 0 and temp <a[j] : #Mientras j sea igual o mayor a 0 y temp sea menor
                a[j + 1] = a[j] #Movera a una pos a la derecha
                j = j-1 #Se mueve un paso hacia atras para continuar comparando
            a[j + 1] = temp #Cuando encuentra la pos correcta lo inserta en el temp
def printArr(a): #Lee el arreglo y lo ilustra en la terminal
     for i in range(len(a)):
          print(a[i], end = " ")

a = [70, 12, 32, 34, 65] #Define el arreglo
print("Antes de ordenar los elementos del arreglo:")
printArr(a)
insertionSort(a)
print("\nDespues de arreglos los elemntos del arreglo:")
printArr(a)