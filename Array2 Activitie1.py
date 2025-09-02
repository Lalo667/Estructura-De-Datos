n = int(input(f"De que tamaño sera la matriz?: ")) # se le pregunta el usuario que tan grande sera el arreglo
valores = [0] * n #es porque se inicializa en 0
for i in range(n):
    valores[i] = int(input(f"Ingrese el valor para la posicion {i+1}: ")) #La informacion se guarda en la lista i

print(f"Los valores ingresados son: {valores}")
pos = int(input(f"\nIngrese la posicion donde desea insertar (0 a {n}): "))
valor = int(input("Ingrese el valor a insertar: "))

valores = valores + [0] #Se suma un nuevo espacio al final de la lista

for i in range(len(valores)-1, pos, -1): #Se desplazan los elementos a la derecha, el primer -1 indica el último índice, el pos indica la posicion antes que parara y el ultimo -1 indica la direccion
    valores[i] = valores[i-1]

valores[pos] = valor

print("\nLa nueva Lista es:")
print(valores)
