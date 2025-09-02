n = int(input(f"De que tamaño sera la matriz?: ")) # se le pregunta el usuario que tan grande sera el arreglo
lista = [0] * n #es porque se inicializa en 0
for i in range(n):
    lista[i] = int(input(f"Ingrese el valor para la posicion {i+1}: "))
valor = int(input("Ingrese el valor que desea buscar: "))

if valor in lista:
    print(f"El valor {valor} se encuentra en la lista.")
else:
    print(f"El valor {valor} no se encuentra en la lista.")
