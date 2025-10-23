import random # Permite generar valores aleatorios. Se usa más adelante para decidir posiciones, caminos, trampas,tesoros, etc.
import os # Permite interactuar con el sistema operativo. Se usa para limpiar la consola.
import msvcrt # Permite capturar entradas de teclado sin necesidad de presionar Enter.
from colorama import Fore, Style, init # Permite imprimir texto en colores en la consola.

init(autoreset=True) # Inicializa colorama para que los colores se restablezcan automáticamente después de cada impresión.

# ---------- CONFIG ----------
SIZE = 25
EMPTY = " "      # Camino transitable
WALL = "█"       # Pared sólida
PLAYER = "P"
EXIT = "X"
FOG = "░"        # Niebla (celdas no descubiertas)
TRAP = "T"       # Trampa (invisible para el jugador)
TREASURE = "💎"  # Tesoro
VISION_RADIUS = 1  # Radio de visión del jugador
MAX_VIDAS = 3
NUM_TRAMPAS = 6
NUM_TESOROS = 3
# ---------------------------

def clear_console(): # Función que limpia la consola, con el objetivo de que el juego (el laberinto) se vea limpio cada vez que el jugador se mueve.
    os.system("cls" if os.name == "nt" else "clear") # Usa "cls" para Windows y "clear" para otros sistemas operativos.

def crear_tablero(): # Esto representa un laberinto vacío, sin caminos abiertos aún.
    """Crear tablero lleno de paredes"""
    return [[WALL for _ in range(SIZE)] for _ in range(SIZE)] # La parte interna [WALL for _ in range(SIZE)] crea una fila con SIZE paredes. La parte externa repite esto SIZE veces para crear una matriz cuadrada.

def movimientos_validos(x, y):
    dirs = [(1,0), (-1,0), (0,1), (0,-1)] # Direcciones posibles: abajo, arriba, derecha, izquierda
    return [(x+dx, y+dy) for dx, dy in dirs if 0 <= x+dx < SIZE and 0 <= y+dy < SIZE] # Retorna solo las coordenadas que están dentro de los límites del tablero.

def colocar_inicial():

    """Coloca al jugador en una orilla aleatoria, nunca en esquinas."""
    lado = random.choice(["top", "bottom", "left", "right"]) # Escoge un lado aleatorio del tablero.
    
    if lado == "top":
        x, y = 1, random.randint(2, SIZE - 3) # Evita columnas 0 y SIZE-1 para no estar en esquinas.
    elif lado == "bottom":
        x, y = SIZE - 2, random.randint(2, SIZE - 3) # Lo mismo pero en la parte inferior
    elif lado == "left":
        x, y = random.randint(2, SIZE - 3), 1 #tmb evita filas 0 y SIZE-1
    else:  # right
        x, y = random.randint(2, SIZE - 3), SIZE - 2
    
    return x, y, lado

def generar_laberinto_recursivo(tablero, x, y, visitados):
    """Genera laberinto usando algoritmo de Recursive Backtracking"""
    visitados.add((x, y)) # Marca la celda (x, y) como visitada para no volver a ella.
    tablero[x][y] = EMPTY # Abre la celda actual como un camino transitable.
    
    # Direcciones aleatorias
    direcciones = [(0, 2), (2, 0), (0, -2), (-2, 0)] # Movimientos de 2 en 2 para dejar paredes entre caminos.
    random.shuffle(direcciones) # Mezcla las direcciones para que el laberinto sea diferente cada vez.
    
    for dx, dy in direcciones:
        nx, ny = x + dx, y + dy # Nueva posición al mover en la dirección actual.
        
        if (0 < nx < SIZE - 1 and 0 < ny < SIZE - 1 and # Asegura que la nueva posición esté dentro de los límites del tablero (no en los bordes).
            (nx, ny) not in visitados): # y las q no haya sido visitada aún.
            # Romper la pared entre celdas
            tablero[x + dx//2][y + dy//2] = EMPTY # Abre la pared entre la celda actual y la nueva celda.
            generar_laberinto_recursivo(tablero, nx, ny, visitados) # Llama recursivamente para continuar generando el laberinto desde la nueva celda.

def abrir_caminos_extra(tablero, num_aperturas=15): # Función para abrir caminos adicionales en el laberinto.
    """Abre paredes aleatorias para crear caminos alternativos"""
    for _ in range(num_aperturas): # Repite el proceso num_aperturas veces.
        x = random.randint(2, SIZE - 3) # Escoge una fila aleatoria, evitando los bordes.
        y = random.randint(2, SIZE - 3) # Escoge una columna aleatoria, evitando los bordes.
        if tablero[x][y] == WALL: # Solo abre si es una pared
            # Verificar que al menos tenga 2 vecinos EMPTY
            vecinos_empty = sum( # Cuenta cuántos vecinos son caminos abiertos (EMPTY).
                1 for vx, vy in movimientos_validos(x, y) # Por cada vecino (vx, vy), cuenta cuántos ya son EMPTY 
                if tablero[vx][vy] == EMPTY # Si el vecino es EMPTY, suma 1
            )
            if vecinos_empty >= 2: # Si tiene al menos 2 vecinos EMPTY, entonces abre esta pared.
                tablero[x][y] = EMPTY # Abre la pared convirtiéndola en un camino transitable.

def colocar_trampas_y_tesoros(tablero, jugador_pos, salida_pos):
    """Coloca trampas y tesoros en posiciones aleatorias del camino"""
    posiciones_libres = [] # Lista para almacenar posiciones válidas para trampas y tesoros.
    
    # Encontrar todas las posiciones vacías (excepto jugador y salida)
    for i in range(SIZE): # Recorre todas las filas del tablero
        for j in range(SIZE): # Recorre todas las columnas del tablero
            if (tablero[i][j] == EMPTY and (i, j) != jugador_pos and (i, j) != salida_pos): # Si la celda es un camino transitable y no es la posición del jugador ni la de la salida
                posiciones_libres.append((i, j)) # Añade la posición (i, j) a la lista de posiciones libres.
    
    if len(posiciones_libres) < NUM_TRAMPAS + NUM_TESOROS: # Si no hay suficientes posiciones libres para colocar todas las trampas y tesoros
        return {}, {} # Retorna conjuntos vacíos (no se pueden colocar).
    
    random.shuffle(posiciones_libres) # Mezcla las posiciones libres para que la colocación sea aleatoria.
    
    # Colocar trampas
    trampas = set() # Usamos un conjunto para evitar duplicados
    for i in range(NUM_TRAMPAS): # Coloca NUM_TRAMPAS trampas
        pos = posiciones_libres[i] # Toma la posición de la lista mezclada
        trampas.add(pos) # Añade la posición al conjunto de trampas
    
    # Colocar tesoros
    tesoros = set() # Otro conjunto para los tesoros
    for i in range(NUM_TRAMPAS, NUM_TRAMPAS + NUM_TESOROS): # Coloca NUM_TESOROS tesoros, empezando después de las trampas
        pos = posiciones_libres[i] # Toma la posición de la lista mezclada
        tesoros.add(pos) # Añade la posición al conjunto de tesoros    
    return trampas, tesoros # Retorna los conjuntos de trampas y tesoros colocados.

def encontrar_salida_valida(tablero, start_x, start_y, distancia_minima=15): # Encuentra una salida válida
    """Encuentra un punto válido para la salida en los bordes"""
    bordes = [] # Lista para almacenar posibles posiciones de salida.
    
    # Buscar en todos los bordes
    for i in range(1, SIZE - 1): # Recorre las columnas
        # Borde superior
        if tablero[1][i] == EMPTY: # Si la celda en el borde superior es un camino transitable
            bordes.append((1, i)) # Añade la posición a la lista de bordes
        # Borde inferior
        if tablero[SIZE-2][i] == EMPTY: #   Misma pero en el borde inferior
            bordes.append((SIZE-2, i))
        # Borde izquierdo
        if tablero[i][1] == EMPTY:
            bordes.append((i, 1))
        # Borde derecho
        if tablero[i][SIZE-2] == EMPTY:
            bordes.append((i, SIZE-2))
    
    # Filtrar por distancia mínima
    bordes_validos = [ # Lista de bordes que cumplen con la distancia mínima desde la posición inicial del jugador.
        (x, y) for x, y in bordes # Solo incluye bordes cuya distancia desde la posición inicial del jugador sea al menos distancia_minima
        if abs(x - start_x) + abs(y - start_y) >= distancia_minima # Comprueba la distancia
        and (x, y) != (start_x, start_y) # Asegura que la salida no esté en la misma posición que el jugador
    ]
    
    if bordes_validos:
        # Retornar el más alejado
        return max(bordes_validos, key=lambda p: abs(p[0] - start_x) + abs(p[1] - start_y)) # Retorna la posición de salida que está más lejos del jugador.
    elif bordes:
        # Si no hay suficientemente lejos, retornar el más alejado disponible
        return max(bordes, key=lambda p: abs(p[0] - start_x) + abs(p[1] - start_y)) # Retorna la posición de salida que está más lejos del jugador.
    
    return None # Si no hay bordes disponibles, retorna None. None es un valor especial en Python que indica ausencia de valor.

def imprimir_tablero(tablero, jugador, salida, descubierto, trampas, tesoros, tesoros_recolectados, modo_revelado=False): # Función para imprimir el tablero en la consola.
    clear_console()  # Limpia la consola para una visualización limpia.
    x_player, y_player = jugador # Posición actual del jugador.
    
    for i in range(SIZE): # Recorre cada fila del tablero
        fila = [] # Lista para almacenar los caracteres de la fila actual
        for j in range(SIZE): # Recorre cada columna de la fila actual
            # Calcular si está dentro del radio de visión
            distancia = abs(i - x_player) + abs(j - y_player) # Distancia desde el jugador, abs es valor absoluto
            visible = distancia <= VISION_RADIUS # Determina si la celda está dentro del radio de visión
            
            if (i, j) == jugador: # Si la celda es la posición del jugador
                fila.append(Fore.GREEN + PLAYER + Style.RESET_ALL) # Imprime el jugador en verde
            elif modo_revelado or visible or (i, j) in descubierto: # Si está en modo revelado, es visible o ya ha sido descubierto
                # Marcar como descubierto si está visible
                if visible: # Si la celda está dentro del radio de visión, Pone los colores correspondientes
                    descubierto.add((i, j)) # Añade la celda a las descubiertas
                
                if (i, j) == salida: # Si la celda es la posición de la salida
                    fila.append(Fore.RED + EXIT + Style.RESET_ALL) # Imprime la salida en rojo
                elif (i, j) in tesoros and (i, j) not in tesoros_recolectados:
                    fila.append(Fore.YELLOW + TREASURE + Style.RESET_ALL)
                elif (i, j) in trampas and modo_revelado:
                    # Trampas solo visibles en modo revelado
                    fila.append(Fore.MAGENTA + TRAP + Style.RESET_ALL)
                elif tablero[i][j] == WALL:
                    fila.append(Fore.WHITE + WALL + Style.RESET_ALL)
                else:
                    fila.append(Fore.CYAN + EMPTY + Style.RESET_ALL)
            else:
                # Niebla GUERRAAAAAAAAAAAAAAAAAAAAAA
                fila.append(Fore.BLACK + FOG + Style.RESET_ALL)
        print("".join(fila)) # Imprime la fila completa uniendo los caracteres.

def mover_jugador(tablero, jugador, salida, trampas, tesoros, vidas, nivel):
    x, y = jugador # Posición inicial del jugador
    descubierto = set() # Celdas descubiertas
    tesoros_recolectados = set() # Tesoros recolectados
    modo_revelado = False # Modo secreto para revelar todo el mapa
    
    while True:
        imprimir_tablero(tablero, (x, y), salida, descubierto, trampas, tesoros, tesoros_recolectados, modo_revelado)
        
        # Mostrar información del juego
        vidas_str = Fore.RED + "❤️ " * vidas + Style.RESET_ALL
        tesoros_str = f"{Fore.YELLOW}{len(tesoros_recolectados)}/{NUM_TESOROS}{Style.RESET_ALL}"
        print(f"\n{Fore.CYAN}═══════════════════════════════════════════════════{Style.RESET_ALL}")
        print(f"{Fore.WHITE}NIVEL {nivel} {Style.RESET_ALL}| Vidas: {vidas_str}| Tesoros: {tesoros_str}")
        print(f"{Fore.CYAN}Usa W/A/S/D para moverte. Llega a la '{Fore.RED}X{Fore.CYAN}' para ganar.{Style.RESET_ALL}")
        print(f"{Fore.CYAN}═══════════════════════════════════════════════════{Style.RESET_ALL}")
        
        tecla = msvcrt.getch().decode('utf-8').lower() # Captura la tecla presionada y la convierte a minúscula.

        # Tecla secreta 'm' para revelar todo el mapa
        if tecla == 'm':
            modo_revelado = not modo_revelado
            continue

        nuevo_x, nuevo_y = x, y # Nuevas coordenadas iniciales (sin movimiento)
        
        if tecla == 'w' and x > 0: # Mover arriba
            nuevo_x = x - 1
        elif tecla == 's' and x < SIZE - 1: # Mover abajo
            nuevo_x = x + 1
        elif tecla == 'a' and y > 0: # Mover izq
            nuevo_y = y - 1
        elif tecla == 'd' and y < SIZE - 1: # Mover der
            nuevo_y = y + 1
        else:
            continue

        # Verificar si el movimiento es válido (no es pared)
        if tablero[nuevo_x][nuevo_y] != WALL: # Si no es pared, permite el movimiento
            x, y = nuevo_x, nuevo_y # Actualiza la posición del jugador
            
            # Verificar si pisó una trampa
            if (x, y) in trampas:
                vidas -= 1
                trampas.remove((x, y))  # Eliminar trampa después de activarla
                imprimir_tablero(tablero, (x, y), salida, descubierto, trampas, tesoros, tesoros_recolectados, modo_revelado) # Actualiza el tablero para mostrar la trampa activada
                print(f"\n{Fore.RED}💥 ¡TRAMPA! Perdiste una vida. Vidas restantes: {vidas}{Style.RESET_ALL}")
                msvcrt.getch()
                
                if vidas <= 0: # Si no quedan vidas, termina el juego
                    imprimir_tablero(tablero, (x, y), salida, descubierto, trampas, tesoros, tesoros_recolectados, modo_revelado)
                    print(f"\n{Fore.RED}☠️  pnjo ☠️{Style.RESET_ALL}")
                    print(f"{Fore.YELLOW}Tesoros recolectados: {len(tesoros_recolectados)}/{NUM_TESOROS}{Style.RESET_ALL}") # Muestra los tesoros recolectados al perder
                    return False
            
            # Verificar si recogió un tesoro
            if (x, y) in tesoros and (x, y) not in tesoros_recolectados: # Si la posición actual tiene un tesoro y no ha sido recolectado aún
                tesoros_recolectados.add((x, y)) # Añade el tesoro a los recolectados
                imprimir_tablero(tablero, (x, y), salida, descubierto, trampas, tesoros, tesoros_recolectados, modo_revelado) # Actualiza el tablero para mostrar el tesoro recolectado
                print(f"\n{Fore.YELLOW}✨ ¡Tesoro encontrado! ({len(tesoros_recolectados)}/{NUM_TESOROS}){Style.RESET_ALL}") # Mensaje de tesoro encontrado
                msvcrt.getch() # Pausa para que el jugador vea el mensaje
            
            # Verificar si llegó a la salida
            if (x, y) == salida:
                imprimir_tablero(tablero, (x, y), salida, descubierto, trampas, tesoros, tesoros_recolectados, modo_revelado)
                print(f"\n{Fore.YELLOW}🎉 ¡NIVEL {nivel} COMPLETADO! 🎉{Style.RESET_ALL}")
                print(f"{Fore.CYAN}Vidas restantes: {vidas} | Tesoros: {len(tesoros_recolectados)}/{NUM_TESOROS}{Style.RESET_ALL}")
                msvcrt.getch() # Pausa para que el jugador vea el mensaje
                return True 

def generar_nivel(nivel, vidas): # Función para generar y jugar un nivel completo
    """Genera un nivel completo del laberinto"""
    print(f"\n{Fore.CYAN}{'═' * 50}{Style.RESET_ALL}")
    print(f"{Fore.YELLOW}     Generando NIVEL {nivel}...{Style.RESET_ALL}")
    print(f"{Fore.CYAN}{'═' * 50}{Style.RESET_ALL}")
    
    # Crear tablero con paredes
    tablero = crear_tablero()
    
    # Colocar jugador inicial
    x, y, lado = colocar_inicial()
    
    # Generar laberinto desde la posición del jugador
    visitados = set()
    generar_laberinto_recursivo(tablero, x, y, visitados)
    
    # Abrir algunos caminos extra para múltiples rutas
    abrir_caminos_extra(tablero, num_aperturas=20)
    
    # Encontrar posición de salida
    salida = encontrar_salida_valida(tablero, x, y, distancia_minima=15)
    
    if salida is None:
        print("Error: No se pudo generar una salida válida. Intenta de nuevo.")
        return None
    
    # Colocar trampas y tesoros
    trampas, tesoros = colocar_trampas_y_tesoros(tablero, (x, y), salida)
    
    import time
    time.sleep(1) # Pausa para que el jugador vea el mensaje de generación
    
    # Jugar nivel
    completado = mover_jugador(tablero, (x, y), salida, trampas, tesoros, vidas, nivel)
    
    return completado

def main():
    vidas = MAX_VIDAS
    
    print(f"{Fore.CYAN}{'═' * 50}{Style.RESET_ALL}") # Imprime tdo el mensaje de bienvenida
    print(f"{Fore.YELLOW}     ¡Bienvenido al Laberinto de los 3 Niveles!{Style.RESET_ALL}")
    print(f"{Fore.CYAN}{'═' * 50}{Style.RESET_ALL}")
    print(f"{Fore.WHITE}Objetivo: Completa los 3 niveles y encuentra la salida.{Style.RESET_ALL}")
    print(f"{Fore.RED}¡Cuidado con las trampas ocultas!{Style.RESET_ALL}")
    print(f"{Fore.YELLOW}Recolecta tesoros en el camino.{Style.RESET_ALL}")
    msvcrt.getch()
    
    for nivel in range(1, 4): # Ciclo para jugar 3 niveles
        resultado = generar_nivel(nivel, vidas) # Genera y juega el nivel actual
        
        if resultado is False:
            # Game Over
            print(f"\n{Fore.RED}Llegaste hasta el nivel {nivel}.{Style.RESET_ALL}")
            break
        
        if nivel == 3:
            # Ganó los 3 niveles
            print(f"\n{Fore.YELLOW}{'═' * 50}{Style.RESET_ALL}")
            print(f"{Fore.GREEN}   🏆 congratuleichon 🏆{Style.RESET_ALL}")
            print(f"{Fore.YELLOW}{'═' * 50}{Style.RESET_ALL}")
            break
    
    print("\nJugar de nuevo? Ejecuta el programa otra vez.")

if __name__ == "__main__":
    main()