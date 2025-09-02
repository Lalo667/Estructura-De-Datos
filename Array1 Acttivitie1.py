precios = [0] * 3  # Se declara el arreglo y se asigna de cuantos valores seran y el 0 es porque se inicializa en 0

for i in range(len(precios)): # a i se le da valores, en rango se existen los valores 0,1,2
    precios[i] = int(input(f"Dame el precio {i+1}: "))

for i in range(len(precios)):
    print(precios[i])