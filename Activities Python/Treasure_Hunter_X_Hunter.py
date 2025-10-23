import random
import os
import msvcrt
import time
import pickle
from colorama import Fore, Style, init

init(autoreset=True)

# ---------- CONFIG ----------
SIZE = 20
NIVELES = 3
EMPTY = " "
WALL = "█"
PLAYER = "P"
EXIT = "X"
FOG = "░"
TRAP = "T"
TREASURE = "💎"
VIDA_ITEM = "❤️"
ENERGIA_ITEM = "⚡"
VISION_RADIUS = 1
MAX_VIDAS = 3
NUM_TRAMPAS = 3
NUM_TESOROS = 3
NUM_VIDAS_ITEM = 1
NUM_ENERGIA_ITEM = 3
TIEMPO_LIMITE = 180
ENERGIA_INICIAL = 20000
SAVE_FILE = "laberinto_saves.pkl"
SCORES_FILE = "laberinto_scores.pkl"
# ---------------------------

class Juego:
    def __init__(self):
        self.laberinto_3d = []
        self.jugador_pos = [0, 0, 0]
        self.salidas = []
        self.trampas = []
        self.tesoros = []
        self.vidas_items = []
        self.energia_items = []
        self.vidas = MAX_VIDAS
        self.energia = ENERGIA_INICIAL
        self.inventario = []
        self.nivel_actual = 0
        self.descubierto = [set() for _ in range(NIVELES)]
        self.items_recolectados = [set() for _ in range(NIVELES)]
        self.movimientos_sin_chocar = 0
        self.tiempo_restante = TIEMPO_LIMITE
        self.modo_revelado = False
        self.puntuacion_final = 0
        self.salir_al_menu = False
        self.nombre_partida = ""
        self.trampas_pisadas = 0
        
    def clear_console(self):
        os.system("cls" if os.name == "nt" else "clear")
    
    def crear_tablero(self):
        return [[WALL for _ in range(SIZE)] for _ in range(SIZE)]
    
    def movimientos_validos(self, x, y):
        dirs = [(1,0), (-1,0), (0,1), (0,-1)]
        return [(x+dx, y+dy) for dx, dy in dirs if 0 <= x+dx < SIZE and 0 <= y+dy < SIZE]
    
    def colocar_inicial(self):
        """Coloca jugador en una posición válida del borde"""
        if hasattr(self, 'laberinto_3d') and self.laberinto_3d and self.nivel_actual < len(self.laberinto_3d):
            tablero = self.laberinto_3d[self.nivel_actual]
            
            posiciones_validas = []
            
            for i in range(1, SIZE - 1):
                if tablero[1][i] == EMPTY:
                    posiciones_validas.append((1, i, "top"))
                if tablero[SIZE - 2][i] == EMPTY:
                    posiciones_validas.append((SIZE - 2, i, "bottom"))
                if tablero[i][1] == EMPTY:
                    posiciones_validas.append((i, 1, "left"))
                if tablero[i][SIZE - 2] == EMPTY:
                    posiciones_validas.append((i, SIZE - 2, "right"))
            
            if posiciones_validas:
                x, y, lado = random.choice(posiciones_validas)
                return x, y, lado
        
        return 1, 1, "top"
    
    def generar_laberinto_prim_mejorado(self, tablero):
        """Generación de laberinto con Prim modificado que crea callejones sin salida"""
        # Iniciar desde el centro
        start_x = SIZE // 2
        start_y = SIZE // 2
        if start_x % 2 == 0:
            start_x -= 1
        if start_y % 2 == 0:
            start_y -= 1
        
        tablero[start_x][start_y] = EMPTY
        paredes = []
        
        # Agregar paredes adyacentes
        for dx, dy in [(0, 2), (2, 0), (0, -2), (-2, 0)]:
            nx, ny = start_x + dx, start_y + dy
            if 1 <= nx < SIZE - 1 and 1 <= ny < SIZE - 1:
                paredes.append((start_x + dx // 2, start_y + dy // 2, nx, ny))
        
        while paredes:
            # No aleatorizar completamente - tomar de las últimas agregadas con más frecuencia
            # Esto crea más ramificaciones y callejones sin salida
            if random.random() < 0.7 and len(paredes) > 5:
                # 70% del tiempo, tomar de los últimos 5 agregados (más recientes)
                idx = random.randint(max(0, len(paredes) - 5), len(paredes) - 1)
            else:
                # 30% del tiempo, tomar uno aleatorio
                idx = random.randint(0, len(paredes) - 1)
            
            pared_x, pared_y, celda_x, celda_y = paredes.pop(idx)
            
            if tablero[celda_x][celda_y] == WALL:
                tablero[pared_x][pared_y] = EMPTY
                tablero[celda_x][celda_y] = EMPTY
                
                # Agregar nuevas paredes en orden aleatorio
                nuevas_paredes = []
                for dx, dy in [(0, 2), (2, 0), (0, -2), (-2, 0)]:
                    nx, ny = celda_x + dx, celda_y + dy
                    if 1 <= nx < SIZE - 1 and 1 <= ny < SIZE - 1:
                        if tablero[nx][ny] == WALL:
                            nuevas_paredes.append((celda_x + dx // 2, celda_y + dy // 2, nx, ny))
                
                random.shuffle(nuevas_paredes)
                paredes.extend(nuevas_paredes)
    
    def crear_callejones_adicionales(self, tablero, num_callejones=8):
        """Crea callejones sin salida adicionales para hacer el laberinto más desafiante"""
        callejones_creados = 0
        intentos = 0
        max_intentos = num_callejones * 20
        
        while callejones_creados < num_callejones and intentos < max_intentos:
            intentos += 1
            
            # Buscar una pared que pueda convertirse en callejón
            x = random.randint(2, SIZE - 3)
            y = random.randint(2, SIZE - 3)
            
            if tablero[x][y] == WALL:
                # Contar vecinos vacíos
                vecinos_empty = []
                for vx, vy in self.movimientos_validos(x, y):
                    if tablero[vx][vy] == EMPTY:
                        vecinos_empty.append((vx, vy))
                
                # Si tiene exactamente 1 vecino vacío, puede ser un callejón
                if len(vecinos_empty) == 1:
                    # Verificar que no esté creando un ciclo
                    tablero[x][y] = EMPTY
                    
                    # Contar nuevos vecinos vacíos desde esta posición
                    nuevos_vecinos = 0
                    for vx, vy in self.movimientos_validos(x, y):
                        if tablero[vx][vy] == EMPTY and (vx, vy) != vecinos_empty[0]:
                            nuevos_vecinos += 1
                    
                    # Si solo conecta con 1 celda (callejón sin salida), mantener
                    if nuevos_vecinos == 0:
                        callejones_creados += 1
                    else:
                        # Si conecta con más, deshacer
                        tablero[x][y] = WALL
    
    def abrir_caminos_controlados(self, tablero, num_aperturas=10):
        """Abre algunas paredes estratégicamente pero de forma más controlada"""
        abiertos = 0
        intentos = 0
        max_intentos = num_aperturas * 15
        
        # Asegurar conectividad mínima en bordes
        for i in range(1, SIZE - 1, 2):
            # Borde superior
            if tablero[1][i] == WALL and tablero[2][i] == EMPTY:
                if random.random() < 0.3:  # Solo 30% de probabilidad
                    tablero[1][i] = EMPTY
            # Borde inferior
            if tablero[SIZE-2][i] == WALL and tablero[SIZE-3][i] == EMPTY:
                if random.random() < 0.3:
                    tablero[SIZE-2][i] = EMPTY
            # Borde izquierdo
            if tablero[i][1] == WALL and tablero[i][2] == EMPTY:
                if random.random() < 0.3:
                    tablero[i][1] = EMPTY
            # Borde derecho
            if tablero[i][SIZE-2] == WALL and tablero[i][SIZE-3] == EMPTY:
                if random.random() < 0.3:
                    tablero[i][SIZE-2] = EMPTY
        
        # Abrir pocas paredes internas para mantener complejidad
        while abiertos < num_aperturas and intentos < max_intentos:
            x = random.randint(3, SIZE - 4)
            y = random.randint(3, SIZE - 4)
            
            if tablero[x][y] == WALL:
                vecinos_empty = sum(1 for vx, vy in self.movimientos_validos(x, y) 
                                   if tablero[vx][vy] == EMPTY)
                
                # Solo abrir si tiene exactamente 2 vecinos vacíos opuestos
                if vecinos_empty == 2:
                    vecinos_pos = [(vx, vy) for vx, vy in self.movimientos_validos(x, y) 
                                  if tablero[vx][vy] == EMPTY]
                    
                    if len(vecinos_pos) == 2:
                        v1, v2 = vecinos_pos
                        # Verificar que sean opuestos
                        if (v1[0] == v2[0]) or (v1[1] == v2[1]):
                            if random.random() < 0.4:  # Solo 40% de probabilidad
                                tablero[x][y] = EMPTY
                                abiertos += 1
            
            intentos += 1
    
    def encontrar_salida_valida(self, tablero, start_x, start_y, distancia_minima=15):
        bordes = []
        for i in range(1, SIZE - 1):
            if tablero[1][i] == EMPTY:
                bordes.append((1, i))
            if tablero[SIZE-2][i] == EMPTY:
                bordes.append((SIZE-2, i))
            if tablero[i][1] == EMPTY:
                bordes.append((i, 1))
            if tablero[i][SIZE-2] == EMPTY:
                bordes.append((i, SIZE-2))
        
        bordes_validos = [
            (x, y) for x, y in bordes
            if abs(x - start_x) + abs(y - start_y) >= distancia_minima and (x, y) != (start_x, start_y)
        ]
        
        if bordes_validos:
            return max(bordes_validos, key=lambda p: abs(p[0] - start_x) + abs(p[1] - start_y))
        elif bordes:
            return max(bordes, key=lambda p: abs(p[0] - start_x) + abs(p[1] - start_y))
        return None
    
    def colocar_items(self, tablero, jugador_pos, salida_pos):
        posiciones_libres = []
        for i in range(SIZE):
            for j in range(SIZE):
                if tablero[i][j] == EMPTY and (i, j) != jugador_pos and (i, j) != salida_pos:
                    posiciones_libres.append((i, j))
        
        total_items = NUM_TRAMPAS + NUM_TESOROS + NUM_VIDAS_ITEM + NUM_ENERGIA_ITEM
        if len(posiciones_libres) < total_items:
            return set(), set(), set(), set()
        
        random.shuffle(posiciones_libres)
        idx = 0
        
        trampas = set(posiciones_libres[idx:idx+NUM_TRAMPAS])
        idx += NUM_TRAMPAS
        
        tesoros = set(posiciones_libres[idx:idx+NUM_TESOROS])
        idx += NUM_TESOROS
        
        vidas_items = set(posiciones_libres[idx:idx+NUM_VIDAS_ITEM])
        idx += NUM_VIDAS_ITEM
        
        energia_items = set(posiciones_libres[idx:idx+NUM_ENERGIA_ITEM])
        
        return trampas, tesoros, vidas_items, energia_items
    
    def generar_todos_los_niveles(self):
        print(f"{Fore.CYAN}{'═' * 60}{Style.RESET_ALL}")
        print(f"{Fore.YELLOW}     Generando los 3 niveles del laberinto...{Style.RESET_ALL}")
        print(f"{Fore.CYAN}{'═' * 60}{Style.RESET_ALL}")
        
        for nivel in range(NIVELES):
            tablero = self.crear_tablero()
            
            # Generar laberinto base con más callejones sin salida1
            self.generar_laberinto_prim_mejorado(tablero)
            
            # Crear callejones sin salida adicionales
            num_callejones = 8 + (nivel * 2)  # Más callejones en niveles superiores
            self.crear_callejones_adicionales(tablero, num_callejones)
            
            # Abrir muy pocos caminos extra para mantener complejidad
            num_aperturas = max(5, 10 - nivel * 2)  # Menos aperturas en niveles altos
            self.abrir_caminos_controlados(tablero, num_aperturas)
            
            self.laberinto_3d.append(tablero)
            self.nivel_actual = nivel
            
            x, y, _ = self.colocar_inicial()
            
            if nivel == 0:
                self.jugador_pos = [nivel, x, y]
            
            salida = self.encontrar_salida_valida(tablero, x, y, distancia_minima=12)
            
            if salida is None:
                posibles_salidas = []
                for i in range(1, SIZE - 1):
                    if tablero[1][i] == EMPTY and (1, i) != (x, y):
                        posibles_salidas.append((1, i))
                    if tablero[SIZE-2][i] == EMPTY and (SIZE-2, i) != (x, y):
                        posibles_salidas.append((SIZE-2, i))
                    if tablero[i][1] == EMPTY and (i, 1) != (x, y):
                        posibles_salidas.append((i, 1))
                    if tablero[i][SIZE-2] == EMPTY and (i, SIZE-2) != (x, y):
                        posibles_salidas.append((i, SIZE-2))
                
                if posibles_salidas:
                    salida = max(posibles_salidas, 
                               key=lambda p: abs(p[0] - x) + abs(p[1] - y))
                else:
                    salida = (SIZE - 2, SIZE - 2)
                    tablero[salida[0]][salida[1]] = EMPTY
            
            self.salidas.append(salida)
            
            trampas, tesoros, vidas_items, energia_items = self.colocar_items(tablero, (x, y), salida)
            self.trampas.append(trampas)
            self.tesoros.append(tesoros)
            self.vidas_items.append(vidas_items)
            self.energia_items.append(energia_items)
            
            print(f"{Fore.GREEN}✓ Nivel {nivel + 1} generado (con callejones sin salida){Style.RESET_ALL}")
            time.sleep(0.3)
        
        self.nivel_actual = 0
        
        print(f"{Fore.YELLOW}¡Todos los niveles listos!{Style.RESET_ALL}")
        time.sleep(1)
        return True
    
    def obtener_multiplicadores(self):
        if self.nivel_actual == 0:
            return {'costo_movimiento': 100, 'recompensa_3_movimientos': 300, 'valor_tesoro': 500, 'valor_energia': 300}
        elif self.nivel_actual == 1:
            return {'costo_movimiento': 200, 'recompensa_3_movimientos': 300, 'valor_tesoro': 1000, 'valor_energia': 600}
        else:
            return {'costo_movimiento': 200, 'recompensa_3_movimientos': 300, 'valor_tesoro': 2000, 'valor_energia': 1200}
    
    def usar_item_inventario(self, indice):
        if 0 <= indice < len(self.inventario):
            item = self.inventario[indice]
            if item['tipo'] == 'energia':
                self.energia += item['valor']
                self.inventario.pop(indice)
                return f"energía +{item['valor']}"
            elif item['tipo'] == 'vida':
                self.vidas += 1
                self.inventario.pop(indice)
                return "vida +1"
        return None
    
    def agregar_a_inventario(self, item):
        if len(self.inventario) < 5:
            self.inventario.append(item)
            return True
        else:
            return False
    
    def menu_tirar_item(self, nuevo_item):
        while True:
            self.imprimir_tablero()
            print(f"\n{Fore.YELLOW}⚠️  Inventario lleno! ¿Qué item deseas tirar?{Style.RESET_ALL}")
            print(f"{Fore.CYAN}Nuevo item: {self.formato_item(nuevo_item)}{Style.RESET_ALL}\n")
            
            for i, item in enumerate(self.inventario):
                print(f"  [{i+1}] {self.formato_item(item)}")
            print(f"  [6] Descartar nuevo item")
            
            tecla = msvcrt.getch().decode('utf-8')
            if tecla in '123456':
                idx = int(tecla) - 1
                if idx < 5:
                    self.inventario.pop(idx)
                    self.inventario.append(nuevo_item)
                    return True
                else:
                    return False
    
    def formato_item(self, item):
        if item['tipo'] == 'tesoro':
            return f"{Fore.YELLOW}💎 Tesoro ({item['valor']} pts){Style.RESET_ALL}"
        elif item['tipo'] == 'energia':
            return f"{Fore.CYAN}⚡ Energía (+{item['valor']}){Style.RESET_ALL}"
        elif item['tipo'] == 'vida':
            return f"{Fore.RED}❤️  Vida (+1){Style.RESET_ALL}"
        return ""
    
    def dibujar_barra_energia(self):
        segmentos = 5
        energia_por_segmento = ENERGIA_INICIAL // segmentos
        llenos = min(self.energia // energia_por_segmento, segmentos)
        
        barra = ""
        for i in range(segmentos):
            if i < llenos:
                barra += Fore.GREEN + "█" + Style.RESET_ALL
            else:
                barra += Fore.RED + "░" + Style.RESET_ALL
        
        extra = ""
        if self.energia > ENERGIA_INICIAL:
            extra = f" {Fore.CYAN}+{self.energia - ENERGIA_INICIAL}{Style.RESET_ALL}"
        
        return f"[{barra}] {self.energia}{extra}"
    
    def imprimir_tablero(self):
        self.clear_console()
        nivel = self.nivel_actual
        _, x_player, y_player = self.jugador_pos
        tablero = self.laberinto_3d[nivel]
        salida = self.salidas[nivel]
        
        for i in range(SIZE):
            fila = []
            for j in range(SIZE):
                distancia = abs(i - x_player) + abs(j - y_player)
                visible = distancia <= VISION_RADIUS
                
                if (i, j) == (x_player, y_player):
                    fila.append(Fore.GREEN + PLAYER + Style.RESET_ALL)
                elif self.modo_revelado or visible or (i, j) in self.descubierto[nivel]:
                    if visible:
                        self.descubierto[nivel].add((i, j))
                    
                    if (i, j) == salida:
                        fila.append(Fore.RED + EXIT + Style.RESET_ALL)
                    elif (i, j) in self.tesoros[nivel] and (i, j) not in self.items_recolectados[nivel]:
                        fila.append(Fore.YELLOW + "💎" + Style.RESET_ALL)
                    elif (i, j) in self.vidas_items[nivel] and (i, j) not in self.items_recolectados[nivel]:
                        fila.append(Fore.RED + "♥" + Style.RESET_ALL)
                    elif (i, j) in self.energia_items[nivel] and (i, j) not in self.items_recolectados[nivel]:
                        fila.append(Fore.CYAN + "⚡" + Style.RESET_ALL)
                    elif (i, j) in self.trampas[nivel] and self.modo_revelado:
                        fila.append(Fore.MAGENTA + "T" + Style.RESET_ALL)
                    elif tablero[i][j] == WALL:
                        fila.append(Fore.WHITE + WALL + Style.RESET_ALL)
                    else:
                        fila.append(Fore.CYAN + EMPTY + Style.RESET_ALL)
                else:
                    fila.append(Fore.BLACK + FOG + Style.RESET_ALL)
            print("".join(fila))
        
        mins = self.tiempo_restante // 60
        segs = self.tiempo_restante % 60
        tiempo_color = Fore.RED if self.tiempo_restante < 30 else Fore.YELLOW
        vidas_str = Fore.RED + "♥ " * self.vidas + Style.RESET_ALL
        
        print(f"\n{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
        print(f"{Fore.WHITE}NIVEL {self.nivel_actual + 1}/3{Style.RESET_ALL} | Vidas: {vidas_str} | Tiempo: {tiempo_color}{mins:02d}:{segs:02d}{Style.RESET_ALL}")
        print(f"Energía: {self.dibujar_barra_energia()}")
        
        if self.inventario:
            print(f"Inventario ({len(self.inventario)}/5):")
            for i, item in enumerate(self.inventario):
                print(f"  [{i+1}] {self.formato_item(item)}", end="  ")
            print()
        
        mult = self.obtener_multiplicadores()
        print(f"{Fore.WHITE}Mov: -{mult['costo_movimiento']} | 3 sin chocar: +{mult['recompensa_3_movimientos']}{Style.RESET_ALL}")
        print(f"{Fore.CYAN}W/A/S/D: Mover | 1-5: Usar item | ESC: Menú{Style.RESET_ALL}")
        print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
    
    def jugar(self):
        if not self.laberinto_3d:
            if not self.generar_todos_los_niveles():
                return
        
        self.imprimir_tablero()
        
        while self.nivel_actual < NIVELES and not self.salir_al_menu:
            nivel, x, y = self.jugador_pos
            
            if self.tiempo_restante <= 0:
                self.imprimir_tablero()
                print(f"\n{Fore.RED}⏰ ¡TIEMPO AGOTADO! GAME OVER{Style.RESET_ALL}")
                time.sleep(2)
                return "game_over"
            
            if self.energia <= 0:
                self.imprimir_tablero()
                print(f"\n{Fore.RED}⚡ ¡SIN ENERGÍA! GAME OVER{Style.RESET_ALL}")
                time.sleep(2)
                return "game_over"
            
            if msvcrt.kbhit():
                tecla_byte = msvcrt.getch()
                
                if tecla_byte == b'\x1b':
                    self.salir_al_menu = True
                    return "menu"
                
                tecla = tecla_byte.decode('utf-8').lower()
                
                if tecla in '12345':
                    idx = int(tecla) - 1
                    resultado = self.usar_item_inventario(idx)
                    if resultado:
                        self.imprimir_tablero()
                        print(f"{Fore.GREEN}✓ Item usado: {resultado}{Style.RESET_ALL}")
                        time.sleep(1)
                        self.imprimir_tablero()
                    continue
                
                if tecla == 'm':
                    self.modo_revelado = not self.modo_revelado
                    self.imprimir_tablero()
                    continue
                
                nuevo_x, nuevo_y = x, y
                movimiento_valido = False
                
                if tecla == 'w' and x > 0:
                    nuevo_x = x - 1
                    movimiento_valido = True
                elif tecla == 's' and x < SIZE - 1:
                    nuevo_x = x + 1
                    movimiento_valido = True
                elif tecla == 'a' and y > 0:
                    nuevo_y = y - 1
                    movimiento_valido = True
                elif tecla == 'd' and y < SIZE - 1:
                    nuevo_y = y + 1
                    movimiento_valido = True
                
                if not movimiento_valido:
                    continue
                
                tablero = self.laberinto_3d[nivel]
                mult = self.obtener_multiplicadores()
                
                if tablero[nuevo_x][nuevo_y] != WALL:
                    self.jugador_pos = [nivel, nuevo_x, nuevo_y]
                    self.energia -= mult['costo_movimiento']
                    self.tiempo_restante -= 1
                    self.movimientos_sin_chocar += 1
                    
                    if self.movimientos_sin_chocar >= 3:
                        self.energia += mult['recompensa_3_movimientos']
                        self.movimientos_sin_chocar = 0
                    
                    self.imprimir_tablero()
                    
                    if (nuevo_x, nuevo_y) in self.trampas[nivel]:
                        self.vidas -= 1
                        self.trampas_pisadas += 1
                        self.trampas[nivel].remove((nuevo_x, nuevo_y))
                        self.movimientos_sin_chocar = 0
                        print(f"\n{Fore.RED}💥 ¡TRAMPA! -1 vida{Style.RESET_ALL}")
                        time.sleep(1)
                        
                        if self.vidas <= 0:
                            print(f"\n{Fore.RED}☠️  GAME OVER ☠️{Style.RESET_ALL}")
                            time.sleep(2)
                            return "game_over"
                        
                        self.imprimir_tablero()
                    
                    if (nuevo_x, nuevo_y) in self.tesoros[nivel] and (nuevo_x, nuevo_y) not in self.items_recolectados[nivel]:
                        self.items_recolectados[nivel].add((nuevo_x, nuevo_y))
                        item = {'tipo': 'tesoro', 'valor': mult['valor_tesoro']}
                        if not self.agregar_a_inventario(item):
                            if self.menu_tirar_item(item):
                                pass
                        print(f"\n{Fore.YELLOW}✨ ¡Tesoro recolectado! ({mult['valor_tesoro']} pts){Style.RESET_ALL}")
                        time.sleep(1)
                        self.imprimir_tablero()
                    
                    if (nuevo_x, nuevo_y) in self.vidas_items[nivel] and (nuevo_x, nuevo_y) not in self.items_recolectados[nivel]:
                        self.items_recolectados[nivel].add((nuevo_x, nuevo_y))
                        item = {'tipo': 'vida', 'valor': 1}
                        if not self.agregar_a_inventario(item):
                            if self.menu_tirar_item(item):
                                pass
                        print(f"\n{Fore.RED}❤️  ¡Vida encontrada!{Style.RESET_ALL}")
                        time.sleep(1)
                        self.imprimir_tablero()
                    
                    if (nuevo_x, nuevo_y) in self.energia_items[nivel] and (nuevo_x, nuevo_y) not in self.items_recolectados[nivel]:
                        self.items_recolectados[nivel].add((nuevo_x, nuevo_y))
                        item = {'tipo': 'energia', 'valor': mult['valor_energia']}
                        if not self.agregar_a_inventario(item):
                            if self.menu_tirar_item(item):
                                pass
                        print(f"\n{Fore.CYAN}⚡ ¡Energía encontrada! (+{mult['valor_energia']}){Style.RESET_ALL}")
                        time.sleep(1)
                        self.imprimir_tablero()
                    
                    if (nuevo_x, nuevo_y) == self.salidas[nivel]:
                        self.nivel_actual += 1
                        
                        if self.nivel_actual < NIVELES:
                            self.vidas += 1
                            
                            new_x, new_y, _ = self.colocar_inicial()
                            self.jugador_pos = [self.nivel_actual, new_x, new_y]
                            self.tiempo_restante = TIEMPO_LIMITE
                            
                            self.imprimir_tablero()
                            print(f"\n{Fore.GREEN}🎉 ¡NIVEL {nivel + 1} COMPLETADO! +1 vida{Style.RESET_ALL}")
                            time.sleep(2)
                            self.imprimir_tablero()
                        else:
                            self.calcular_puntuacion_final()
                            self.mostrar_victoria()
                            self.guardar_puntuacion()
                            return "victoria"
                else:
                    self.movimientos_sin_chocar = 0
        
        return "menu" if self.salir_al_menu else "victoria"
    
    def calcular_puntuacion_final(self):
        puntos_energia = self.energia
        puntos_tesoros = sum(item['valor'] for item in self.inventario if item['tipo'] == 'tesoro')
        penalizacion_trampas = self.trampas_pisadas * 200
        self.puntuacion_final = puntos_energia + puntos_tesoros - penalizacion_trampas
    
    def mostrar_victoria(self):
        self.clear_console()
        print(f"\n{Fore.YELLOW}{'═' * 70}{Style.RESET_ALL}")
        print(f"{Fore.GREEN}   🏆 ¡FELICIDADES! COMPLETASTE LOS 3 NIVELES 🏆{Style.RESET_ALL}")
        print(f"{Fore.YELLOW}{'═' * 70}{Style.RESET_ALL}")
        print(f"\n{Fore.CYAN}PUNTUACIÓN FINAL:{Style.RESET_ALL}")
        
        puntos_energia = self.energia
        puntos_tesoros = sum(item['valor'] for item in self.inventario if item['tipo'] == 'tesoro')
        penalizacion = self.trampas_pisadas * 200
        
        print(f"  Energía restante: {puntos_energia} pts")
        print(f"  Tesoros en inventario: {puntos_tesoros} pts")
        print(f"  Trampas pisadas: -{penalizacion} pts")
        print(f"\n{Fore.YELLOW}  TOTAL: {self.puntuacion_final} PUNTOS{Style.RESET_ALL}")
        print(f"\n{Fore.YELLOW}{'═' * 70}{Style.RESET_ALL}")
        print(f"\n{Fore.CYAN}Presiona cualquier tecla para continuar...{Style.RESET_ALL}")
        msvcrt.getch()
    
    def guardar_puntuacion(self):
        scores = cargar_puntuaciones()
        scores.append({
            'nombre': self.nombre_partida,
            'puntos': self.puntuacion_final,
            'fecha': time.strftime("%Y-%m-%d %H:%M")
        })
        scores.sort(key=lambda x: x['puntos'], reverse=True)
        scores = scores[:10]
        guardar_puntuaciones(scores)

def cargar_partidas():
    if os.path.exists(SAVE_FILE):
        with open(SAVE_FILE, 'rb') as f:
            return pickle.load(f)
    return [None, None, None]

def guardar_partidas(partidas):
    with open(SAVE_FILE, 'wb') as f:
        pickle.dump(partidas, f)

def cargar_puntuaciones():
    if os.path.exists(SCORES_FILE):
        with open(SCORES_FILE, 'rb') as f:
            return pickle.load(f)
    return []

def guardar_puntuaciones(scores):
    with open(SCORES_FILE, 'wb') as f:
        pickle.dump(scores, f)

def guardar_partida(juego, slot):
    partidas = cargar_partidas()
    juego_data = {
        'laberinto_3d': juego.laberinto_3d,
        'jugador_pos': juego.jugador_pos,
        'salidas': juego.salidas,
        'trampas': [list(t) for t in juego.trampas],
        'tesoros': [list(t) for t in juego.tesoros],
        'vidas_items': [list(v) for v in juego.vidas_items],
        'energia_items': [list(e) for e in juego.energia_items],
        'vidas': juego.vidas,
        'energia': juego.energia,
        'inventario': juego.inventario,
        'nivel_actual': juego.nivel_actual,
        'descubierto': [list(d) for d in juego.descubierto],
        'items_recolectados': [list(i) for i in juego.items_recolectados],
        'tiempo_restante': juego.tiempo_restante,
        'nombre_partida': juego.nombre_partida,
        'trampas_pisadas': juego.trampas_pisadas
    }
    partidas[slot] = juego_data
    guardar_partidas(partidas)

def cargar_partida_slot(slot):
    partidas = cargar_partidas()
    if partidas[slot]:
        juego = Juego()
        data = partidas[slot]
        juego.laberinto_3d = data['laberinto_3d']
        juego.jugador_pos = data['jugador_pos']
        juego.salidas = data['salidas']
        juego.trampas = [set(t) for t in data['trampas']]
        juego.tesoros = [set(t) for t in data['tesoros']]
        juego.vidas_items = [set(v) for v in data['vidas_items']]
        juego.energia_items = [set(e) for e in data['energia_items']]
        juego.vidas = data['vidas']
        juego.energia = data['energia']
        juego.inventario = data['inventario']
        juego.nivel_actual = data['nivel_actual']
        juego.descubierto = [set(d) for d in data['descubierto']]
        juego.items_recolectados = [set(i) for i in data['items_recolectados']]
        juego.tiempo_restante = data['tiempo_restante']
        juego.nombre_partida = data['nombre_partida']
        juego.trampas_pisadas = data['trampas_pisadas']
        return juego
    return None

def menu_principal():
    while True:
        os.system("cls" if os.name == "nt" else "clear")
        print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
        print(f"{Fore.YELLOW}          🏰 LABERINTO 3D - MENÚ PRINCIPAL 🏰{Style.RESET_ALL}")
        print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}\n")
        print(f"{Fore.WHITE}  [1] Nueva Partida{Style.RESET_ALL}")
        print(f"{Fore.WHITE}  [2] Cargar Partida{Style.RESET_ALL}")
        print(f"{Fore.WHITE}  [3] Tabla de Puntuaciones{Style.RESET_ALL}")
        print(f"{Fore.WHITE}  [4] Salir{Style.RESET_ALL}")
        print(f"\n{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
        print(f"{Fore.CYAN}Selecciona una opción: {Style.RESET_ALL}", end='')
        
        opcion = msvcrt.getch().decode('utf-8')
        
        if opcion == '1':
            menu_nueva_partida()
        elif opcion == '2':
            menu_cargar_partida()
        elif opcion == '3':
            menu_puntuaciones()
        elif opcion == '4':
            print(f"\n{Fore.YELLOW}¡Hasta luego!{Style.RESET_ALL}")
            break

def menu_nueva_partida():
    os.system("cls" if os.name == "nt" else "clear")
    print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
    print(f"{Fore.YELLOW}          NUEVA PARTIDA{Style.RESET_ALL}")
    print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}\n")
    
    partidas = cargar_partidas()
    print(f"{Fore.WHITE}Selecciona un slot para guardar (1-3):{Style.RESET_ALL}\n")
    for i in range(3):
        if partidas[i]:
            print(f"  [{i+1}] {partidas[i]['nombre_partida']} - Nivel {partidas[i]['nivel_actual']+1}")
        else:
            print(f"  [{i+1}] [Vacío]")
    
    print(f"\n{Fore.CYAN}Slot: {Style.RESET_ALL}", end='')
    slot = msvcrt.getch().decode('utf-8')
    
    if slot not in '123':
        return
    
    slot = int(slot) - 1
    
    print(f"\n{Fore.CYAN}Nombre de la partida: {Style.RESET_ALL}", end='')
    nombre = input().strip()
    if not nombre:
        nombre = f"Partida {slot + 1}"
    
    juego = Juego()
    juego.nombre_partida = nombre
    
    resultado = juego.jugar()
    
    if resultado == "menu":
        print(f"\n{Fore.YELLOW}¿Guardar partida antes de salir? (S/N): {Style.RESET_ALL}", end='')
        guardar = msvcrt.getch().decode('utf-8').lower()
        if guardar == 's':
            guardar_partida(juego, slot)
            print(f"\n{Fore.GREEN}✓ Partida guardada{Style.RESET_ALL}")
            time.sleep(1)

def menu_cargar_partida():
    os.system("cls" if os.name == "nt" else "clear")
    print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
    print(f"{Fore.YELLOW}          CARGAR PARTIDA{Style.RESET_ALL}")
    print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}\n")
    
    partidas = cargar_partidas()
    hay_partidas = False
    
    for i in range(3):
        if partidas[i]:
            hay_partidas = True
            print(f"  [{i+1}] {partidas[i]['nombre_partida']} - Nivel {partidas[i]['nivel_actual']+1} - Energía: {partidas[i]['energia']}")
        else:
            print(f"  [{i+1}] [Vacío]")
    
    if not hay_partidas:
        print(f"\n{Fore.RED}No hay partidas guardadas.{Style.RESET_ALL}")
        time.sleep(2)
        return
    
    print(f"\n{Fore.WHITE}[4] Volver{Style.RESET_ALL}")
    print(f"\n{Fore.CYAN}Selecciona slot: {Style.RESET_ALL}", end='')
    
    slot = msvcrt.getch().decode('utf-8')
    
    if slot == '4':
        return
    
    if slot not in '123':
        return
    
    slot = int(slot) - 1
    juego = cargar_partida_slot(slot)
    
    if juego:
        resultado = juego.jugar()
        
        if resultado == "menu":
            print(f"\n{Fore.YELLOW}¿Guardar progreso? (S/N): {Style.RESET_ALL}", end='')
            guardar = msvcrt.getch().decode('utf-8').lower()
            if guardar == 's':
                guardar_partida(juego, slot)
                print(f"\n{Fore.GREEN}✓ Partida guardada{Style.RESET_ALL}")
                time.sleep(1)
    else:
        print(f"{Fore.RED}Error cargando partida.{Style.RESET_ALL}")
        time.sleep(2)

def menu_puntuaciones():
    os.system("cls" if os.name == "nt" else "clear")
    print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
    print(f"{Fore.YELLOW}          🏆 TABLA DE PUNTUACIONES - TOP 10 🏆{Style.RESET_ALL}")
    print(f"{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}\n")
    
    scores = cargar_puntuaciones()
    
    if not scores:
        print(f"{Fore.RED}  No hay puntuaciones registradas aún.{Style.RESET_ALL}")
    else:
        print(f"{Fore.WHITE}{'#':<4} {'Nombre':<25} {'Puntos':<15} {'Fecha':<20}{Style.RESET_ALL}")
        print(f"{Fore.CYAN}{'-' * 70}{Style.RESET_ALL}")
        
        for i, score in enumerate(scores, 1):
            if i == 1:
                color = Fore.YELLOW
            elif i == 2:
                color = Fore.WHITE
            elif i == 3:
                color = Fore.RED
            else:
                color = Fore.CYAN
            
            print(f"{color}{i:<4} {score['nombre']:<25} {score['puntos']:<15} {score['fecha']:<20}{Style.RESET_ALL}")
    
    print(f"\n{Fore.CYAN}{'═' * 70}{Style.RESET_ALL}")
    print(f"{Fore.CYAN}Presiona cualquier tecla para volver...{Style.RESET_ALL}")
    msvcrt.getch()

def main():
    menu_principal()

if __name__ == "__main__":
    main()