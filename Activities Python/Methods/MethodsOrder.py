import time
import random
import matplotlib.pyplot as plt
from typing import List, Callable

# ==================== GENERADORES DE DATOS ====================
def generar_ordenado(n: int) -> List[int]:
    return list(range(1, n + 1))

def generar_medianamente_ordenado(n: int) -> List[int]:
    arr = generar_ordenado(n)
    desordenar = int(n * 0.3)
    for _ in range(desordenar):
        pos1 = random.randint(0, n - 1)
        pos2 = random.randint(0, n - 1)
        arr[pos1], arr[pos2] = arr[pos2], arr[pos1]
    return arr

def generar_inverso(n: int) -> List[int]:
    return list(range(n, 0, -1))

# ==================== ALGORITMOS DE ORDENAMIENTO ====================

# 1. BURBUJA
def bubble_sort(arr: List[int]) -> None:
    n = len(arr)
    for i in range(n - 1):
        for j in range(n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]

# 2. POR CUBOS (Bucket Sort)
def bucket_sort(arr: List[int]) -> None:
    if not arr:
        return
    
    n = len(arr)
    max_val = max(arr)
    min_val = min(arr)
    bucket_count = n
    range_val = max_val - min_val + 1
    
    buckets = [[] for _ in range(bucket_count)]
    
    for num in arr:
        bucket_index = (bucket_count * (num - min_val)) // range_val
        if bucket_index >= bucket_count:
            bucket_index = bucket_count - 1
        buckets[bucket_index].append(num)
    
    for bucket in buckets:
        bucket.sort()
    
    arr.clear()
    for bucket in buckets:
        arr.extend(bucket)

# 3. COMB SORT
def comb_sort(arr: List[int]) -> None:
    n = len(arr)
    gap = n
    swapped = True
    
    while gap > 1 or swapped:
        gap = (gap * 10) // 13
        if gap < 1:
            gap = 1
        
        swapped = False
        for i in range(n - gap):
            if arr[i] > arr[i + gap]:
                arr[i], arr[i + gap] = arr[i + gap], arr[i]
                swapped = True

# 4. CONTEO (Counting Sort)
def counting_sort(arr: List[int]) -> None:
    if not arr:
        return
    
    max_val = max(arr)
    min_val = min(arr)
    range_val = max_val - min_val + 1
    
    count = [0] * range_val
    output = [0] * len(arr)
    
    for num in arr:
        count[num - min_val] += 1
    
    for i in range(1, range_val):
        count[i] += count[i - 1]
    
    for i in range(len(arr) - 1, -1, -1):
        output[count[arr[i] - min_val] - 1] = arr[i]
        count[arr[i] - min_val] -= 1
    
    arr[:] = output

# 5. HEAP SORT
def heapify(arr: List[int], n: int, i: int) -> None:
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2
    
    if left < n and arr[left] > arr[largest]:
        largest = left
    
    if right < n and arr[right] > arr[largest]:
        largest = right
    
    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        heapify(arr, n, largest)

def heap_sort(arr: List[int]) -> None:
    n = len(arr)
    
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i)
    
    for i in range(n - 1, 0, -1):
        arr[0], arr[i] = arr[i], arr[0]
        heapify(arr, i, 0)

# 6. INSERCIÓN
def insertion_sort(arr: List[int]) -> None:
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key

# 7. FUSIÓN (Merge Sort)
def merge(arr: List[int], left: int, mid: int, right: int) -> None:
    L = arr[left:mid + 1]
    R = arr[mid + 1:right + 1]
    
    i = j = 0
    k = left
    
    while i < len(L) and j < len(R):
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1
    
    while i < len(L):
        arr[k] = L[i]
        i += 1
        k += 1
    
    while j < len(R):
        arr[k] = R[j]
        j += 1
        k += 1

def merge_sort_helper(arr: List[int], left: int, right: int) -> None:
    if left < right:
        mid = (left + right) // 2
        merge_sort_helper(arr, left, mid)
        merge_sort_helper(arr, mid + 1, right)
        merge(arr, left, mid, right)

def merge_sort(arr: List[int]) -> None:
    if len(arr) > 1:
        merge_sort_helper(arr, 0, len(arr) - 1)

# 8. RÁPIDO (Quick Sort) - VERSIÓN ITERATIVA COMPLETA
def quick_sort(arr: List[int]) -> None:
    if len(arr) <= 1:
        return
    
    # Stack manual para evitar recursión
    stack = [(0, len(arr) - 1)]
    
    while stack:
        low, high = stack.pop()
        
        if low >= high:
            continue
        
        # Pivote aleatorio para evitar peor caso
        pivot_idx = random.randint(low, high)
        arr[pivot_idx], arr[high] = arr[high], arr[pivot_idx]
        
        # Partición in-place
        pivot = arr[high]
        i = low - 1
        
        for j in range(low, high):
            if arr[j] < pivot:
                i += 1
                arr[i], arr[j] = arr[j], arr[i]
        
        arr[i + 1], arr[high] = arr[high], arr[i + 1]
        pi = i + 1
        
        # Añadir subparticiones (más pequeña primero)
        if pi - low < high - pi:
            stack.append((pi + 1, high))
            stack.append((low, pi - 1))
        else:
            stack.append((low, pi - 1))
            stack.append((pi + 1, high))

# 9. RADIX SORT
def counting_sort_for_radix(arr: List[int], exp: int) -> None:
    n = len(arr)
    output = [0] * n
    count = [0] * 10
    
    for num in arr:
        index = (num // exp) % 10
        count[index] += 1
    
    for i in range(1, 10):
        count[i] += count[i - 1]
    
    for i in range(n - 1, -1, -1):
        index = (arr[i] // exp) % 10
        output[count[index] - 1] = arr[i]
        count[index] -= 1
    
    arr[:] = output

def radix_sort(arr: List[int]) -> None:
    if not arr:
        return
    
    max_val = max(arr)
    exp = 1
    
    while max_val // exp > 0:
        counting_sort_for_radix(arr, exp)
        exp *= 10

# 10. SELECCIÓN
def selection_sort(arr: List[int]) -> None:
    n = len(arr)
    for i in range(n - 1):
        min_idx = i
        for j in range(i + 1, n):
            if arr[j] < arr[min_idx]:
                min_idx = j
        arr[i], arr[min_idx] = arr[min_idx], arr[i]

# 11. SHELL SORT
def shell_sort(arr: List[int]) -> None:
    
    n = len(arr)
    gap = n // 2
    
    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            while j >= gap and arr[j - gap] > temp:
                arr[j] = arr[j - gap]
                j -= gap
            arr[j] = temp
        gap //= 2
    return arr

# ==================== FUNCIÓN DE MEDICIÓN ====================
def medir_tiempo(arr: List[int], sort_func: Callable) -> float:
    arr_copia = arr.copy()
    inicio = time.time()
    sort_func(arr_copia)
    fin = time.time()
    return (fin - inicio) * 1000  # Convertir a milisegundos

# ==================== OBTENER DESCRIPCIÓN DEL ESCENARIO ====================
def obtener_descripcion_escenario(tipo: str) -> str:
    if tipo == "Ordenado":
        return "Arreglo ya ordenado ascendentemente [1, 2, 3, ..., n]"
    elif tipo == "Medianamente Ordenado":
        return "Arreglo parcialmente ordenado (70% ordenado, 30% aleatorio)"
    else:
        return "Arreglo ordenado inversamente [n, n-1, ..., 2, 1]"

# ==================== FUNCIÓN DE ANÁLISIS ====================
def analizar_algoritmo(nombre: str, sort_func: Callable) -> None:
    tamanios = [100, 1000, 10000, 100000]
    tipos = ["Ordenado", "Medianamente Ordenado", "Inverso"]
    
    print("\n")
    print("-" * 79)
    print(f"                   ANÁLISIS: {nombre}")
    print("-" * 79)
    print()

    for n in tamanios:
        print("-" * 79)
        print(f"  TAMAÑO DEL ARREGLO: {n:8} elementos")
        print("-" * 79)
        print()
        
        tiempo_mejor = -1
        escenario_mejor = ""
        
        for i, tipo in enumerate(tipos):
            print("  " + "-" * 71)
            print(f"  | ESCENARIO {i+1}: {tipo:<55}|")
            print(f"  | {obtener_descripcion_escenario(tipo):<70}|")
            print("  " + "-" * 71)
            
            if i == 0:
                arr = generar_ordenado(n)
            elif i == 1:
                arr = generar_medianamente_ordenado(n)
            else:
                arr = generar_inverso(n)
            
            tiempo = medir_tiempo(arr, sort_func)
            
            print(f"    ⏱️  Tiempo de ejecución: {tiempo:12.4f} ms\n")
            
            if tiempo_mejor == -1 or tiempo < tiempo_mejor:
                tiempo_mejor = tiempo
                escenario_mejor = tipo
        
        print("  " + "=" * 71)
        print(f"  | 🏆 MEJOR ESCENARIO para {n:8} elementos: {escenario_mejor:<27}|")
        print(f"  |    Tiempo: {tiempo_mejor:12.4f} ms{' ' * 43}|")
        print("  " + "=" * 71)
        print()

# ==================== COMPARAR TODOS LOS ALGORITMOS ====================
def comparar_todos_los_algoritmos() -> None:
    algoritmos = [
        ("Burbuja", bubble_sort),
        ("Por Cubos", bucket_sort),
        ("Comb Sort", comb_sort),
        ("Conteo", counting_sort),
        ("Heap Sort", heap_sort),
        ("Insercion", insertion_sort),
        ("Fusion", merge_sort),
        ("Rapido", quick_sort),
        ("Radix Sort", radix_sort),
        ("Seleccion", selection_sort),
        ("Shell Sort", shell_sort)
    ]
    
    tamanios = [100, 1000, 10000, 100000]
    tipos_orden = ["Ordenado", "Medianamente Ordenado", "Inverso"]
    
    print("\n")
    print("-" * 79)
    print("            COMPARACIÓN COMPLETA DE TODOS LOS ALGORITMOS")
    print("-" * 79)
    
    # Almacenar datos para gráficas
    datos_graficas = {tipo: {tam: [] for tam in tamanios} for tipo in tipos_orden}
    
    for n in tamanios:
        print(f"\n{'=' * 79}")
        print(f"  TAMAÑO DEL ARREGLO: {n:8} elementos")
        print("=" * 79)
        
        for tipo_idx, tipo in enumerate(tipos_orden):
            print(f"\n{'-' * 79}")
            print(f"  ESCENARIO {tipo_idx+1}: {tipo:<60}")
            print(f"  {obtener_descripcion_escenario(tipo):<74}")
            print("-" * 79)
            print()
            
            resultados = []
            
            for nombre, funcion in algoritmos:
                if tipo_idx == 0:
                    arr = generar_ordenado(n)
                elif tipo_idx == 1:
                    arr = generar_medianamente_ordenado(n)
                else:
                    arr = generar_inverso(n)
                
                tiempo = medir_tiempo(arr, funcion)
                resultados.append((nombre, tiempo))
                
                print(f"  {nombre:<22}: {tiempo:14.4f} ms")
            
            # Guardar para gráficas
            datos_graficas[tipo][n] = resultados
            
            mejor = min(resultados, key=lambda x: x[1])
            peor = max(resultados, key=lambda x: x[1])
            diferencia = peor[1] - mejor[1]
            factor = peor[1] / mejor[1] if mejor[1] > 0 else 0
            
            print(f"\n  {'=' * 71}")
            print(f"  |  🏆 MEJOR ALGORITMO: {mejor[0]:<22} {mejor[1]:14.4f} ms       |")
            print(f"  |  ❌ PEOR ALGORITMO:  {peor[0]:<22} {peor[1]:14.4f} ms       |")
            print(f"  |  📊 DIFERENCIA:      {diferencia:36.4f} ms       |")
            print(f"  |  ⚡ FACTOR DE MEJORA: {factor:35.2f}x        |")
            print("  " + "=" * 71)
    
    # Generar gráficas
    generar_graficas(datos_graficas, algoritmos, tamanios, tipos_orden)
    
    mostrar_recomendaciones()

# ==================== GENERAR GRÁFICAS ====================
def generar_graficas(datos: dict, algoritmos: list, tamanios: list, tipos: list) -> None:
    print("\n\n📊 Generando gráficas...")
    
    # Crear una figura con subgráficas
    fig, axes = plt.subplots(len(tipos), len(tamanios), figsize=(20, 12))
    fig.suptitle('Comparación de Tiempos de Ejecución por Algoritmo', fontsize=16, fontweight='bold')
    
    colores = plt.cm.tab20(range(len(algoritmos)))
    
    for i, tipo in enumerate(tipos):
        for j, tam in enumerate(tamanios):
            ax = axes[i][j] if len(tipos) > 1 else axes[j]
            
            resultados = datos[tipo][tam]
            nombres = [r[0] for r in resultados]
            tiempos = [r[1] for r in resultados]
            
            bars = ax.bar(range(len(nombres)), tiempos, color=colores)
            ax.set_title(f'{tipo}\n(n={tam})', fontsize=10, fontweight='bold')
            ax.set_ylabel('Tiempo (ms)', fontsize=9)
            ax.set_xticks(range(len(nombres)))
            ax.set_xticklabels(nombres, rotation=45, ha='right', fontsize=7)
            ax.grid(axis='y', alpha=0.3)
            
            # Resaltar mejor y peor
            mejor_idx = tiempos.index(min(tiempos))
            peor_idx = tiempos.index(max(tiempos))
            bars[mejor_idx].set_color('green')
            bars[peor_idx].set_color('red')
    
    plt.tight_layout()
    plt.savefig('comparacion_algoritmos.png', dpi=300, bbox_inches='tight')
    print("✅ Gráfica guardada como 'comparacion_algoritmos.png'")
    plt.show()

# ==================== MOSTRAR RECOMENDACIONES ====================
def mostrar_recomendaciones() -> None:
    print("\n\n" + "=" * 79)
    print("                    RESUMEN Y RECOMENDACIONES")
    print("=" * 79)
    print()
    
    print("-" * 79)
    print("  📋 ESCENARIO 1: DATOS YA ORDENADOS")
    print("     Descripción: [1, 2, 3, ..., n]")
    print("-" * 79)
    print("   🏆 Mejor opción: Counting Sort o Radix Sort")
    print("   ⚡ Complejidad: O(n) - Tiempo lineal")
    print("   💡 Alternativa: Insertion Sort (O(n) en mejor caso)\n")
    
    print("-" * 79)
    print("  📋 ESCENARIO 2: DATOS MEDIANAMENTE ORDENADOS")
    print("     Descripción: 70% ordenado, 30% elementos aleatorios")
    print("-" * 79)
    print("   🏆 Mejor opción: Quick Sort o Merge Sort")
    print("   ⚡ Complejidad: O(n log n) promedio")
    print("   💡 Ventaja: Buen balance entre velocidad y estabilidad\n")
    
    print("-" * 79)
    print("  📋 ESCENARIO 3: DATOS ORDENADOS INVERSAMENTE")
    print("     Descripción: [n, n-1, ..., 2, 1] - Peor caso para muchos algoritmos")
    print("-" * 79)
    print("   🏆 Mejor opción: Merge Sort o Heap Sort")
    print("   ⚡ Complejidad: O(n log n) garantizado")
    print("   ⚠️  EVITAR: Quick Sort estándar puede degradar (pero la versión aleatoria funciona bien)\n")
    
    print("=" * 79)
    print("                    RECOMENDACIÓN GENERAL POR CASO")
    print("=" * 79)
    print()
    print("  ✅ Uso general (datos aleatorios):     Quick Sort o Merge Sort")
    print("  ✅ Datos pequeños con enteros:         Counting Sort")
    print("  ✅ Estabilidad garantizada:            Merge Sort")
    print("  ✅ Memoria limitada:                   Heap Sort o Quick Sort (in-place)")
    print("  ✅ Datos casi ordenados:               Insertion Sort o Shell Sort")
    print("  ✅ Enteros con rango limitado:         Radix Sort o Counting Sort\n")

# ==================== MENÚ PRINCIPAL ====================
def main():
    while True:
        print("\n" + "=" * 79)
        print("         SISTEMA DE ANÁLISIS DE ALGORITMOS DE ORDENAMIENTO")
        print("=" * 79)
        print("-" * 79)
        print("  1.  Burbuja (Bubble Sort)")
        print("  2.  Por Cubos (Bucket Sort)")
        print("  3.  Comb Sort")
        print("  4.  Conteo (Counting Sort)")
        print("  5.  Heap Sort")
        print("  6.  Inserción (Insertion Sort)")
        print("  7.  Fusión (Merge Sort)")
        print("  8.  Rápido (Quick Sort)")
        print("  9.  Radix Sort")
        print("  10. Por Selección (Selection Sort)")
        print("  11. Shell Sort")
        print("-" * 79)
        print("  12. ⭐ COMPARAR TODOS LOS ALGORITMOS (CON GRÁFICAS)")
        print("-" * 79)
        print("  0.  Salir")
        print("-" * 79)
        
        try:
            opcion = int(input("\n>>> Seleccione una opción: "))
            
            if opcion == 0:
                print("\n" + "=" * 79)
                print("           ¡Gracias por usar el Sistema de Análisis!")
                print("=" * 79 + "\n")
                break
            elif opcion == 1:
                analizar_algoritmo("MÉTODO BURBUJA (BUBBLE SORT)", bubble_sort)
            elif opcion == 2:
                analizar_algoritmo("POR CUBOS (BUCKET SORT)", bucket_sort)
            elif opcion == 3:
                analizar_algoritmo("COMB SORT", comb_sort)
            elif opcion == 4:
                analizar_algoritmo("CONTEO (COUNTING SORT)", counting_sort)
            elif opcion == 5:
                analizar_algoritmo("HEAP SORT", heap_sort)
            elif opcion == 6:
                analizar_algoritmo("INSERCIÓN (INSERTION SORT)", insertion_sort)
            elif opcion == 7:
                analizar_algoritmo("FUSIÓN (MERGE SORT)", merge_sort)
            elif opcion == 8:
                analizar_algoritmo("RÁPIDO (QUICK SORT)", quick_sort)
            elif opcion == 9:
                analizar_algoritmo("RADIX SORT", radix_sort)
            elif opcion == 10:
                analizar_algoritmo("POR SELECCIÓN (SELECTION SORT)", selection_sort)
            elif opcion == 11:
                analizar_algoritmo("SHELL SORT", shell_sort)
            elif opcion == 12:
                comparar_todos_los_algoritmos()
            else:
                print("\n❌ Opción no válida. Por favor, intente de nuevo.")
        
        except ValueError:
            print("\n❌ Por favor ingrese un número válido.")
        except KeyboardInterrupt:
            print("\n\n❌ Operación cancelada por el usuario.")
            break
        
        if opcion != 0:
            input("\n>>> Presione Enter para continuar...")

if __name__ == "__main__":
    main()