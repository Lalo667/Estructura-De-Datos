
TIEMPO_LIMITE = 120
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