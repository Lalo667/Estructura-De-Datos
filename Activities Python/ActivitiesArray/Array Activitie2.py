Matriz = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

Filas = len(Matriz)
Columnas = len(Matriz[0])  # Asumiendo que todas las filas tienen la misma cantidad de columnas

print("Recorrido por los renglones:")
for i in range(Filas):
    for j in range(Columnas):
        print(Matriz[i][j], end=" ")
    print()

print("Recorrido por las columnas:")
for j in range(Columnas):
    for i in range(Filas):
        print(Matriz[i][j], end=" ")
    print()