
ListProduct = [0] * 3

for i in range(len(ListProduct)):
    NameProduct = input("Cual es el nombre del producto?: ") 
    PriceProduct = float(input(f"Cual es el precio del producto?: "))
    ListProduct[i] = (NameProduct, PriceProduct)

for i in range(len(ListProduct)):
    print(ListProduct[i])