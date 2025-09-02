n = int(input(f"De que tamaño sera la matriz?: ")) # se le pregunta el usuario que tan grande sera el arreglo
lista = [0] * n #es porque se inicializa en 0
for i in range(n):
    lista[i] = int(input(f"Ingrese el valor para la posicion {i+1}: "))
valor = int(input("Ingrese el valor que desea buscar: "))

encontrado = False #Se inicializa el valor encontrado como falso
for i in range(n): #Se repetira el valor las veces el cual el usuario pidio en n
    if lista[i] == valor: #i es donde se guardaron los valores, entonces se hara una comparacion con el valor que escribio en la terminal
        encontrado = True #Entonces si lo encuentra el encontrado sera verdadero
        posicion = i #Se guarda la posicion donde se encontro el valor
        break

if encontrado:
    print(f"El valor {valor} se encuentra en la posición {posicion+1}.")
else:
    print(f"El valor {valor} no se encuentra en la lista.")