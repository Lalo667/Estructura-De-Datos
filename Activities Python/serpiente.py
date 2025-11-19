import pygame
import random
import sys
import os
import math

pygame.init()

# Constantes
ANCHO = 800
ALTO = 800
TAMANO_CELDA = 30
FILAS = (ALTO - 120) // TAMANO_CELDA
COLUMNAS = ANCHO // TAMANO_CELDA
OFFSET_Y = 120

# Colores
NEGRO = (0, 0, 0)
BLANCO = (255, 255, 255)
VERDE = (0, 255, 0)
VERDE_OSCURO = (0, 180, 0)
VERDE_NEON = (57, 255, 20)
ROJO = (255, 50, 50)
ROJO_OSCURO = (180, 0, 0)
AMARILLO = (255, 220, 0)
AMARILLO_OSCURO = (200, 180, 0)
AZUL = (30, 144, 255)
AZUL_OSCURO = (0, 100, 200)
GRIS = (40, 40, 40)
GRIS_CLARO = (80, 80, 80)
PURPURA = (147, 51, 234)

pantalla = pygame.display.set_mode((ANCHO, ALTO))
pygame.display.set_caption("SNAKE EXTREME")
reloj = pygame.time.Clock()

fuente_titulo = pygame.font.Font(None, 90)
fuente_grande = pygame.font.Font(None, 56)
fuente_mediana = pygame.font.Font(None, 40)
fuente_pequena = pygame.font.Font(None, 28)
fuente_mini = pygame.font.Font(None, 22)

class Particula:
    def __init__(self, x, y, color):
        self.x = x
        self.y = y
        self.vx = random.uniform(-3, 3)
        self.vy = random.uniform(-5, -1)
        self.vida = 30
        self.color = color
        self.size = random.randint(3, 8)
        
    def actualizar(self):
        self.x += self.vx
        self.y += self.vy
        self.vy += 0.3
        self.vida -= 1
        
    def dibujar(self, pantalla):
        if self.vida > 0:
            alpha = int((self.vida / 30) * 255)
            s = pygame.Surface((self.size * 2, self.size * 2), pygame.SRCALPHA)
            color_con_alpha = (*self.color, alpha)
            pygame.draw.circle(s, color_con_alpha, (self.size, self.size), self.size)
            pantalla.blit(s, (self.x - self.size, self.y - self.size))

class Boton:
    def __init__(self, x, y, ancho, alto, texto, color, color_hover, icono=None):
        self.rect = pygame.Rect(x, y, ancho, alto)
        self.texto = texto
        self.color = color
        self.color_hover = color_hover
        self.hover = False
        self.icono = icono
        self.escala = 1.0
        self.target_escala = 1.0
        
    def dibujar(self, pantalla):
        self.escala += (self.target_escala - self.escala) * 0.3
        color = self.color_hover if self.hover else self.color
        centro = self.rect.center
        ancho_escalado = int(self.rect.width * self.escala)
        alto_escalado = int(self.rect.height * self.escala)
        rect_escalado = pygame.Rect(0, 0, ancho_escalado, alto_escalado)
        rect_escalado.center = centro
        
        sombra = pygame.Rect(rect_escalado.x + 4, rect_escalado.y + 4, rect_escalado.width, rect_escalado.height)
        pygame.draw.rect(pantalla, (0, 0, 0, 100), sombra, border_radius=15)
        pygame.draw.rect(pantalla, color, rect_escalado, border_radius=15)
        pygame.draw.rect(pantalla, BLANCO, rect_escalado, 3, border_radius=15)
        
        if self.icono:
            texto_completo = f"{self.icono} {self.texto}"
        else:
            texto_completo = self.texto
            
        texto_surface = fuente_mediana.render(texto_completo, True, BLANCO)
        texto_rect = texto_surface.get_rect(center=rect_escalado.center)
        pantalla.blit(texto_surface, texto_rect)
        
    def verificar_hover(self, pos):
        was_hover = self.hover
        self.hover = self.rect.collidepoint(pos)
        if self.hover and not was_hover:
            self.target_escala = 1.1
        elif not self.hover:
            self.target_escala = 1.0
        
    def es_clickeado(self, pos):
        return self.rect.collidepoint(pos)

class Serpiente:
    def __init__(self):
        self.reiniciar()
        
    def reiniciar(self):
        centro_x = COLUMNAS // 2
        centro_y = FILAS // 2
        self.cuerpo = []
        for i in range(5):
            self.cuerpo.append([centro_x - i, centro_y])
        self.direccion = [1, 0]
        self.nueva_direccion = [1, 0]
        
    def mover(self):
        self.direccion = self.nueva_direccion
        nueva_cabeza = [self.cuerpo[0][0] + self.direccion[0], self.cuerpo[0][1] + self.direccion[1]]
        
        if nueva_cabeza[0] < 0:
            nueva_cabeza[0] = COLUMNAS - 1
        elif nueva_cabeza[0] >= COLUMNAS:
            nueva_cabeza[0] = 0
        if nueva_cabeza[1] < 0:
            nueva_cabeza[1] = FILAS - 1
        elif nueva_cabeza[1] >= FILAS:
            nueva_cabeza[1] = 0
        
        self.cuerpo.insert(0, nueva_cabeza)
        
    def crecer(self):
        pass
        
    def reducir(self):
        if len(self.cuerpo) > 0:
            self.cuerpo.pop()
            
    def quitar_cola(self):
        if len(self.cuerpo) > 0:
            self.cuerpo.pop()
        
    def colision_consigo(self):
        if len(self.cuerpo) < 2:
            return False
        cabeza = self.cuerpo[0]
        for segmento in self.cuerpo[1:]:
            if cabeza[0] == segmento[0] and cabeza[1] == segmento[1]:
                return True
        return False
        
    def dibujar(self, pantalla):
        for i, segmento in enumerate(self.cuerpo):
            x = segmento[0] * TAMANO_CELDA
            y = segmento[1] * TAMANO_CELDA + OFFSET_Y
            
            if i == 0:
                # Cabeza - El vato en skate
                pygame.draw.rect(pantalla, VERDE_NEON, (x, y, TAMANO_CELDA - 2, TAMANO_CELDA - 2), border_radius=8)
                pygame.draw.rect(pantalla, VERDE, (x, y, TAMANO_CELDA - 2, TAMANO_CELDA - 2), 2, border_radius=8)
                
                # Cara del vato
                pygame.draw.circle(pantalla, (255, 200, 150), (x + 15, y + 12), 8)
                
                # Ojos
                pygame.draw.circle(pantalla, NEGRO, (x + 12, y + 10), 2)
                pygame.draw.circle(pantalla, NEGRO, (x + 18, y + 10), 2)
                
                # Gorra
                pygame.draw.rect(pantalla, NEGRO, (x + 7, y + 3, 16, 6))
                pygame.draw.rect(pantalla, ROJO, (x + 5, y + 6, 6, 3))
                
                # Skate
                if self.direccion == [1, 0]:  # Derecha
                    pygame.draw.rect(pantalla, (150, 75, 0), (x + 18, y + 22, 8, 3), border_radius=2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 19, y + 25), 2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 25, y + 25), 2)
                elif self.direccion == [-1, 0]:  # Izquierda
                    pygame.draw.rect(pantalla, (150, 75, 0), (x + 4, y + 22, 8, 3), border_radius=2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 5, y + 25), 2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 11, y + 25), 2)
                elif self.direccion == [0, -1]:  # Arriba
                    pygame.draw.rect(pantalla, (150, 75, 0), (x + 11, y + 4, 3, 8), border_radius=2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 10, y + 5), 2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 10, y + 11), 2)
                else:  # Abajo
                    pygame.draw.rect(pantalla, (150, 75, 0), (x + 11, y + 18, 3, 8), border_radius=2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 10, y + 19), 2)
                    pygame.draw.circle(pantalla, NEGRO, (x + 10, y + 25), 2)
            else:
                # Cuerpo - La estela del skate
                factor = i / len(self.cuerpo)
                color_r = int(VERDE_OSCURO[0] + (VERDE_NEON[0] - VERDE_OSCURO[0]) * (1 - factor))
                color_g = int(VERDE_OSCURO[1] + (VERDE_NEON[1] - VERDE_OSCURO[1]) * (1 - factor))
                color_b = int(VERDE_OSCURO[2] + (VERDE_NEON[2] - VERDE_OSCURO[2]) * (1 - factor))
                color = (color_r, color_g, color_b)
                pygame.draw.rect(pantalla, color, (x + 4, y + 4, TAMANO_CELDA - 10, TAMANO_CELDA - 10), border_radius=5)

class Elemento:
    def __init__(self, serpiente_cuerpo, otras_posiciones=None):
        if otras_posiciones is None:
            otras_posiciones = []
        self.posicion = self.generar_posicion(serpiente_cuerpo, otras_posiciones)
        self.animacion = 0
        
    def generar_posicion(self, serpiente_cuerpo, otras_posiciones):
        intentos = 0
        while intentos < 100:
            pos = [random.randint(0, COLUMNAS - 1), random.randint(0, FILAS - 1)]
            if pos not in serpiente_cuerpo and pos not in otras_posiciones:
                return pos
            intentos += 1
        return [random.randint(0, COLUMNAS - 1), random.randint(0, FILAS - 1)]
                
    def dibujar(self, pantalla, color, color_oscuro, simbolo):
        self.animacion += 0.1
        x = self.posicion[0] * TAMANO_CELDA
        y = self.posicion[1] * TAMANO_CELDA + OFFSET_Y
        
        escala = 1 + math.sin(self.animacion) * 0.1
        offset = (TAMANO_CELDA * (1 - escala)) / 2
        
        pygame.draw.rect(pantalla, (0, 0, 0, 100), (x + 3, y + 3, TAMANO_CELDA - 2, TAMANO_CELDA - 2), border_radius=8)
        
        size_scaled = int((TAMANO_CELDA - 2) * escala)
        pygame.draw.rect(pantalla, color, (x + offset, y + offset, size_scaled, size_scaled), border_radius=8)
        pygame.draw.rect(pantalla, color_oscuro, (x + offset, y + offset, size_scaled, size_scaled), 3, border_radius=8)
        
        texto = fuente_grande.render(simbolo, True, BLANCO)
        texto_rect = texto.get_rect(center=(x + TAMANO_CELDA // 2, y + TAMANO_CELDA // 2))
        pantalla.blit(texto, texto_rect)

class Juego:
    def __init__(self):
        self.serpiente = Serpiente()
        self.nivel = 1
        self.puntuacion = 0
        self.top_puntuaciones = []
        self.cargar_puntuaciones()
        self.velocidad_base = 8
        self.comidas = []
        self.trampas = []
        self.particulas = []
        self.generar_elementos()
        
    def generar_elementos(self):
        # Generar 1 comida (ropa Y2K)
        self.comidas = [Elemento(self.serpiente.cuerpo)]
        
        # Generar 2 trampas (policías)
        posiciones_ocupadas = self.serpiente.cuerpo + [c.posicion for c in self.comidas]
        self.trampas = []
        for _ in range(2):
            trampa = Elemento(self.serpiente.cuerpo, posiciones_ocupadas)
            self.trampas.append(trampa)
            posiciones_ocupadas.append(trampa.posicion)
        
    def crear_particulas(self, x, y, color, cantidad=10):
        for _ in range(cantidad):
            self.particulas.append(Particula(x, y, color))
        
    def actualizar(self):
        self.serpiente.mover()
        cabeza = self.serpiente.cuerpo[0]
        
        comio_algo = False
        
        # Verificar colisión con comidas (ropa Y2K)
        for comida in self.comidas[:]:
            if cabeza[0] == comida.posicion[0] and cabeza[1] == comida.posicion[1]:
                comio_algo = True
                self.puntuacion += 10 * self.nivel
                
                x = comida.posicion[0] * TAMANO_CELDA + TAMANO_CELDA // 2
                y = comida.posicion[1] * TAMANO_CELDA + OFFSET_Y + TAMANO_CELDA // 2
                self.crear_particulas(x, y, (50, 50, 50), 20)  # Partículas oscuras void
                
                self.comidas.remove(comida)
                
                if len(self.serpiente.cuerpo) >= 15:
                    self.subir_nivel()
                else:
                    self.generar_elementos()
                break
        
        # Verificar colisión con trampas (policías)
        for trampa in self.trampas[:]:
            if cabeza[0] == trampa.posicion[0] and cabeza[1] == trampa.posicion[1]:
                comio_algo = True
                
                x = trampa.posicion[0] * TAMANO_CELDA + TAMANO_CELDA // 2
                y = trampa.posicion[1] * TAMANO_CELDA + OFFSET_Y + TAMANO_CELDA // 2
                self.crear_particulas(x, y, (255, 182, 193), 25)  # Partículas rosa wholesome
                
                # Reducir tamaño: quitar 2 segmentos
                segmentos_a_quitar = 2
                for _ in range(segmentos_a_quitar):
                    if len(self.serpiente.cuerpo) > 0:
                        self.serpiente.cuerpo.pop()
                
                self.trampas.remove(trampa)
                self.generar_elementos()
                
                if len(self.serpiente.cuerpo) == 0:
                    return False
                break
        
        # Solo quitar cola si NO comió nada
        if not comio_algo:
            self.serpiente.quitar_cola()
        
        # Actualizar partículas
        self.particulas = [p for p in self.particulas if p.vida > 0]
        for p in self.particulas:
            p.actualizar()
            
        # Verificar colisión consigo mismo
        if self.serpiente.colision_consigo():
            return False
        
        return True
        
    def subir_nivel(self):
        self.nivel += 1
        self.serpiente.reiniciar()
        self.generar_elementos()
        
    def obtener_velocidad(self):
        return self.velocidad_base + (self.nivel - 1) * 2
        
    def cargar_puntuaciones(self):
        try:
            if os.path.exists('puntuaciones.txt'):
                with open('puntuaciones.txt', 'r') as f:
                    for linea in f.readlines():
                        try:
                            self.top_puntuaciones.append(int(linea.strip()))
                        except:
                            pass
        except:
            self.top_puntuaciones = []
            
    def guardar_puntuacion(self):
        try:
            self.top_puntuaciones.append(self.puntuacion)
            self.top_puntuaciones.sort(reverse=True)
            self.top_puntuaciones = self.top_puntuaciones[:5]
            with open('puntuaciones.txt', 'w') as f:
                for punt in self.top_puntuaciones:
                    f.write(str(punt) + '\n')
        except:
            pass
                
    def mostrar_hud(self, pantalla):
        for i in range(OFFSET_Y):
            color = (GRIS[0], GRIS[1], GRIS[2])
            pygame.draw.line(pantalla, color, (0, i), (ANCHO, i))
        
        pygame.draw.line(pantalla, VERDE_NEON, (0, OFFSET_Y - 2), (ANCHO, OFFSET_Y - 2), 4)
        
        texto_punt = fuente_grande.render(f'{self.puntuacion}', True, AMARILLO)
        pantalla.blit(texto_punt, (20, 20))
        
        texto_nivel = fuente_mediana.render(f'Nivel {self.nivel}', True, BLANCO)
        pantalla.blit(texto_nivel, (20, 75))
        
        progreso = len(self.serpiente.cuerpo) / 15
        barra_x = 350
        barra_y = 35
        barra_ancho = 400
        barra_alto = 30
        
        pygame.draw.rect(pantalla, GRIS_CLARO, (barra_x, barra_y, barra_ancho, barra_alto), border_radius=15)
        
        if progreso > 0:
            ancho_progreso = int(barra_ancho * progreso)
            color_progreso = VERDE_NEON if progreso < 1 else PURPURA
            pygame.draw.rect(pantalla, color_progreso, (barra_x, barra_y, ancho_progreso, barra_alto), border_radius=15)
        
        pygame.draw.rect(pantalla, BLANCO, (barra_x, barra_y, barra_ancho, barra_alto), 3, border_radius=15)
        
        texto_tam = fuente_pequena.render(f'{len(self.serpiente.cuerpo)}/15', True, BLANCO)
        texto_rect = texto_tam.get_rect(center=(barra_x + barra_ancho // 2, barra_y + barra_alto // 2))
        pantalla.blit(texto_tam, texto_rect)
        
        leyenda_y = 80
        # VOID (da puntos)
        pygame.draw.rect(pantalla, (10, 10, 10), (350, leyenda_y - 8, 16, 16), border_radius=3)
        pygame.draw.circle(pantalla, (255, 255, 0), (355, leyenda_y), 2)
        pygame.draw.circle(pantalla, (255, 255, 0), (361, leyenda_y), 2)
        texto_com = fuente_mini.render('VOID +1', True, BLANCO)
        pantalla.blit(texto_com, (370, leyenda_y - 10))
        
        # WHOLESOME (quita puntos)
        pygame.draw.rect(pantalla, (255, 182, 193), (500, leyenda_y - 8, 16, 16), border_radius=3)
        pygame.draw.circle(pantalla, (255, 100, 150), (508, leyenda_y - 4), 2)
        texto_trap = fuente_mini.render('Wholesome -2', True, BLANCO)
        pantalla.blit(texto_trap, (520, leyenda_y - 10))

def dibujar_fondo_animado(pantalla, tiempo):
    for i in range(0, ANCHO, 40):
        for j in range(0, ALTO, 40):
            alpha = int((math.sin(tiempo + i * 0.01 + j * 0.01) + 1) * 10)
            color = (0, alpha, 0)
            pygame.draw.rect(pantalla, color, (i, j, 2, 2))

def mostrar_menu(pantalla, top_puntuaciones):
    tiempo = 0
    boton_jugar = Boton(ANCHO // 2 - 150, 350, 300, 70, "JUGAR", VERDE_OSCURO, VERDE_NEON, "")
    boton_puntuaciones = Boton(ANCHO // 2 - 150, 440, 300, 70, "PUNTAJES", AZUL_OSCURO, AZUL, "")
    boton_salir = Boton(ANCHO // 2 - 150, 530, 300, 70, "SALIR", ROJO_OSCURO, ROJO, "")
    particulas_fondo = []
    
    while True:
        tiempo += 0.05
        pantalla.fill(NEGRO)
        dibujar_fondo_animado(pantalla, tiempo)
        
        if random.random() < 0.1:
            particulas_fondo.append(Particula(random.randint(0, ANCHO), random.randint(0, ALTO), VERDE_NEON))
        
        particulas_fondo = [p for p in particulas_fondo if p.vida > 0]
        for p in particulas_fondo:
            p.actualizar()
            p.dibujar(pantalla)
        
        # --- TITULO MODIFICADO ---
        texto_titulo = fuente_titulo.render("SNAKE DE LOS VOID", True, VERDE_NEON)
        sombra_titulo = fuente_titulo.render("SNAKE DE LOS VOID", True, VERDE_OSCURO)
        rect_sombra = sombra_titulo.get_rect(center=(ANCHO // 2 + 5, 135))
        pantalla.blit(sombra_titulo, rect_sombra)
        rect_titulo = texto_titulo.get_rect(center=(ANCHO // 2, 130))
        pantalla.blit(texto_titulo, rect_titulo)
        
        # --- SUBTITULO MODIFICADO ---
        texto_sub = "Los que no saben el contexto👀"
        colores_subtitulo = [ROJO, AMARILLO, VERDE, AZUL, PURPURA]
        x_inicio = ANCHO // 2 - 270
        for i, letra in enumerate(texto_sub):
            if letra == ' ':
                continue
            color = colores_subtitulo[i % len(colores_subtitulo)]
            offset_y = math.sin(tiempo * 3 + i * 0.3) * 5
            letra_surface = fuente_pequena.render(letra, True, color)
            pantalla.blit(letra_surface, (x_inicio + i * 16, 220 + offset_y))
        
        if top_puntuaciones:
            texto_mejor = fuente_mediana.render(f"Mejor: {top_puntuaciones[0]}", True, AMARILLO)
            rect_mejor = texto_mejor.get_rect(center=(ANCHO // 2, 280))
            pantalla.blit(texto_mejor, rect_mejor)
        
        pos_mouse = pygame.mouse.get_pos()
        boton_jugar.verificar_hover(pos_mouse)
        boton_puntuaciones.verificar_hover(pos_mouse)
        boton_salir.verificar_hover(pos_mouse)
        
        boton_jugar.dibujar(pantalla)
        boton_puntuaciones.dibujar(pantalla)
        boton_salir.dibujar(pantalla)
        
        texto_controles = fuente_mini.render("Controles: WASD o Flechas | ESC para pausar", True, GRIS_CLARO)
        rect_controles = texto_controles.get_rect(center=(ANCHO // 2, ALTO - 40))
        pantalla.blit(texto_controles, rect_controles)
        
        for evento in pygame.event.get():
            if evento.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if evento.type == pygame.MOUSEBUTTONDOWN:
                if boton_jugar.es_clickeado(pos_mouse):
                    return "jugar"
                elif boton_puntuaciones.es_clickeado(pos_mouse):
                    mostrar_puntuaciones(pantalla, top_puntuaciones)
                elif boton_salir.es_clickeado(pos_mouse):
                    pygame.quit()
                    sys.exit()
        
        pygame.display.flip()
        reloj.tick(60)


def mostrar_puntuaciones(pantalla, top_puntuaciones):
    boton_volver = Boton(ANCHO // 2 - 150, 650, 300, 70, "VOLVER", AZUL_OSCURO, AZUL)
    tiempo = 0
    
    while True:
        tiempo += 0.05
        pantalla.fill(NEGRO)
        dibujar_fondo_animado(pantalla, tiempo)
        
        texto_titulo = fuente_grande.render("TOP 5 PUNTAJES", True, AMARILLO)
        rect_titulo = texto_titulo.get_rect(center=(ANCHO // 2, 100))
        sombra = fuente_grande.render("TOP 5 PUNTAJES", True, AMARILLO_OSCURO)
        pantalla.blit(sombra, (rect_titulo.x + 3, rect_titulo.y + 3))
        pantalla.blit(texto_titulo, rect_titulo)
        
        if len(top_puntuaciones) == 0:
            texto_vacio = fuente_mediana.render("No hay puntajes aun", True, BLANCO)
            rect_vacio = texto_vacio.get_rect(center=(ANCHO // 2, 350))
            pantalla.blit(texto_vacio, rect_vacio)
        else:
            y_inicial = 200
            medallas = ["1", "2", "3", "4", "5"]
            colores_puesto = [AMARILLO, GRIS_CLARO, (205, 127, 50), BLANCO, BLANCO]
            
            for i, punt in enumerate(top_puntuaciones[:5]):
                rect_fondo = pygame.Rect(ANCHO // 2 - 250, y_inicial + i * 90, 500, 70)
                pygame.draw.rect(pantalla, GRIS, rect_fondo, border_radius=15)
                pygame.draw.rect(pantalla, colores_puesto[i], rect_fondo, 3, border_radius=15)
                
                texto_medalla = fuente_grande.render(medallas[i], True, colores_puesto[i])
                pantalla.blit(texto_medalla, (rect_fondo.x + 30, rect_fondo.y + 15))
                
                texto_punt = fuente_grande.render(f"{punt}", True, colores_puesto[i])
                texto_rect = texto_punt.get_rect(right=rect_fondo.right - 30, centery=rect_fondo.centery)
                pantalla.blit(texto_punt, texto_rect)
        
        pos_mouse = pygame.mouse.get_pos()
        boton_volver.verificar_hover(pos_mouse)
        boton_volver.dibujar(pantalla)
        
        for evento in pygame.event.get():
            if evento.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if evento.type == pygame.MOUSEBUTTONDOWN:
                if boton_volver.es_clickeado(pos_mouse):
                    return
        
        pygame.display.flip()
        reloj.tick(60)

def mostrar_game_over(pantalla, juego):
    boton_menu = Boton(ANCHO // 2 - 320, 650, 280, 70, "MENU", AZUL_OSCURO, AZUL)
    boton_reiniciar = Boton(ANCHO // 2 + 40, 650, 280, 70, "REINICIAR", VERDE_OSCURO, VERDE_NEON)
    tiempo = 0
    
    while True:
        tiempo += 0.05
        pantalla.fill(NEGRO)
        dibujar_fondo_animado(pantalla, tiempo)
        
        texto_go = fuente_titulo.render('GAME OVER', True, ROJO)
        sombra_go = fuente_titulo.render('GAME OVER', True, ROJO_OSCURO)
        rect_sombra = sombra_go.get_rect(center=(ANCHO // 2 + 5, 105))
        pantalla.blit(sombra_go, rect_sombra)
        rect_go = texto_go.get_rect(center=(ANCHO // 2, 100))
        pantalla.blit(texto_go, rect_go)
        
        y_actual = 220
        rect_stats = pygame.Rect(ANCHO // 2 - 300, y_actual - 20, 600, 160)
        pygame.draw.rect(pantalla, GRIS, rect_stats, border_radius=20)
        pygame.draw.rect(pantalla, VERDE_NEON, rect_stats, 4, border_radius=20)
        
        texto_punt = fuente_grande.render(f'Puntuacion: {juego.puntuacion}', True, AMARILLO)
        rect_punt = texto_punt.get_rect(center=(ANCHO // 2, y_actual + 20))
        pantalla.blit(texto_punt, rect_punt)
        
        texto_nivel = fuente_mediana.render(f'Nivel: {juego.nivel}', True, BLANCO)
        rect_nivel = texto_nivel.get_rect(center=(ANCHO // 2, y_actual + 70))
        pantalla.blit(texto_nivel, rect_nivel)
        
        texto_tam = fuente_mediana.render(f'Tamano: {len(juego.serpiente.cuerpo)}', True, VERDE)
        rect_tam = texto_tam.get_rect(center=(ANCHO // 2, y_actual + 110))
        pantalla.blit(texto_tam, rect_tam)
        
        y_top = 420
        texto_top = fuente_mediana.render('TOP 5', True, AMARILLO)
        rect_top = texto_top.get_rect(center=(ANCHO // 2, y_top))
        pantalla.blit(texto_top, rect_top)
        
        medallas = ["1", "2", "3", "4", "5"]
        for i, punt in enumerate(juego.top_puntuaciones[:5]):
            color = AMARILLO if punt == juego.puntuacion else BLANCO
            
            if punt == juego.puntuacion:
                rect_resaltar = pygame.Rect(ANCHO // 2 - 150, y_top + 40 + i * 35, 300, 30)
                pygame.draw.rect(pantalla, VERDE_OSCURO, rect_resaltar, border_radius=10)
                pygame.draw.rect(pantalla, VERDE_NEON, rect_resaltar, 2, border_radius=10)
            
            texto = fuente_pequena.render(f'{medallas[i]}  {punt} pts', True, color)
            texto_rect = texto.get_rect(center=(ANCHO // 2, y_top + 50 + i * 35))
            pantalla.blit(texto, texto_rect)
        
        pos_mouse = pygame.mouse.get_pos()
        boton_menu.verificar_hover(pos_mouse)
        boton_reiniciar.verificar_hover(pos_mouse)
        
        boton_menu.dibujar(pantalla)
        boton_reiniciar.dibujar(pantalla)
        
        for evento in pygame.event.get():
            if evento.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if evento.type == pygame.MOUSEBUTTONDOWN:
                if boton_menu.es_clickeado(pos_mouse):
                    return "menu"
                elif boton_reiniciar.es_clickeado(pos_mouse):
                    return "reiniciar"
        
        pygame.display.flip()
        reloj.tick(60)

def main():
    while True:
        top_puntuaciones = []
        try:
            if os.path.exists('puntuaciones.txt'):
                with open('puntuaciones.txt', 'r') as f:
                    for linea in f.readlines():
                        try:
                            top_puntuaciones.append(int(linea.strip()))
                        except:
                            pass
        except:
            pass
        
        accion = mostrar_menu(pantalla, top_puntuaciones)
        
        if accion == "jugar":
            juego = Juego()
            jugando = True
            
            while jugando:
                for evento in pygame.event.get():
                    if evento.type == pygame.QUIT:
                        pygame.quit()
                        sys.exit()
                    if evento.type == pygame.KEYDOWN:
                        if (evento.key == pygame.K_w or evento.key == pygame.K_UP) and juego.serpiente.direccion[1] != 1:
                            juego.serpiente.nueva_direccion = [0, -1]
                        elif (evento.key == pygame.K_s or evento.key == pygame.K_DOWN) and juego.serpiente.direccion[1] != -1:
                            juego.serpiente.nueva_direccion = [0, 1]
                        elif (evento.key == pygame.K_a or evento.key == pygame.K_LEFT) and juego.serpiente.direccion[0] != 1:
                            juego.serpiente.nueva_direccion = [-1, 0]
                        elif (evento.key == pygame.K_d or evento.key == pygame.K_RIGHT) and juego.serpiente.direccion[0] != -1:
                            juego.serpiente.nueva_direccion = [1, 0]
                        elif evento.key == pygame.K_ESCAPE:
                            jugando = False
                
                if not juego.actualizar():
                    juego.guardar_puntuacion()
                    accion_go = mostrar_game_over(pantalla, juego)
                    if accion_go == "menu":
                        break
                    elif accion_go == "reiniciar":
                        juego = Juego()
                else:
                    pantalla.fill(NEGRO)
                    
                    for x in range(0, ANCHO, TAMANO_CELDA):
                        pygame.draw.line(pantalla, (20, 20, 20), (x, OFFSET_Y), (x, ALTO))
                    for y in range(OFFSET_Y, ALTO, TAMANO_CELDA):
                        pygame.draw.line(pantalla, (20, 20, 20), (0, y), (ANCHO, y))
                    
                    juego.serpiente.dibujar(pantalla)
                    
                    # Dibujar VOID (comidas - dan puntos)
                    for comida in juego.comidas:
                        x = comida.posicion[0] * TAMANO_CELDA
                        y = comida.posicion[1] * TAMANO_CELDA + OFFSET_Y
                        
                        comida.animacion += 0.15
                        escala = 1 + math.sin(comida.animacion) * 0.08
                        offset = (TAMANO_CELDA * (1 - escala)) / 2
                        size_scaled = int((TAMANO_CELDA - 2) * escala)
                        
                        # Fondo negro profundo del void
                        pygame.draw.rect(pantalla, (10, 10, 10), 
                                       (x + offset, y + offset, size_scaled, size_scaled), border_radius=8)
                        pygame.draw.rect(pantalla, (50, 50, 50), 
                                       (x + offset, y + offset, size_scaled, size_scaled), 2, border_radius=8)
                        
                        # 👀 Ojitos estilo emoji
                        parpadeo = math.sin(comida.animacion * 3)
                        if parpadeo > -0.3:
                            # Esclera (blanca)
                            pygame.draw.ellipse(pantalla, BLANCO, (x + 7, y + 8, 8, 10))
                            pygame.draw.ellipse(pantalla, BLANCO, (x + 16, y + 8, 8, 10))
    
                            # Pupilas (mirando a la derecha 👀)
                            pygame.draw.circle(pantalla, NEGRO, (x + 11, y + 12), 3)
                            pygame.draw.circle(pantalla, NEGRO, (x + 20, y + 12), 3)
    
                            # Brillo en pupilas
                            pygame.draw.circle(pantalla, BLANCO, (x + 10, y + 11), 1)
                            pygame.draw.circle(pantalla, BLANCO, (x + 19, y + 11), 1)

                    
                    # Dibujar WHOLESOME (trampas - quitan puntos)
                    for trampa in juego.trampas:
                        x = trampa.posicion[0] * TAMANO_CELDA
                        y = trampa.posicion[1] * TAMANO_CELDA + OFFSET_Y
                        
                        trampa.animacion += 0.12
                        escala = 1 + math.sin(trampa.animacion) * 0.1
                        offset = (TAMANO_CELDA * (1 - escala)) / 2
                        size_scaled = int((TAMANO_CELDA - 2) * escala)
                        
                        # Fondo rosa wholesome
                        pygame.draw.rect(pantalla, (255, 182, 193), 
                                       (x + offset, y + offset, size_scaled, size_scaled), border_radius=12)
                        pygame.draw.rect(pantalla, (255, 105, 180), 
                                       (x + offset, y + offset, size_scaled, size_scaled), 3, border_radius=12)
                        
                        # Carita wholesome super feliz
                        # Ojitos cerrados felices (^_^)
                        pygame.draw.arc(pantalla, (100, 50, 50), 
                                      (x + 8, y + 10, 6, 4), 3.14, 0, 2)
                        pygame.draw.arc(pantalla, (100, 50, 50), 
                                      (x + 16, y + 10, 6, 4), 3.14, 0, 2)
                        
                        # Boca super feliz
                        pygame.draw.arc(pantalla, (100, 50, 50), 
                                      (x + 8, y + 12, 14, 10), 0, 3.14, 2)
                        
                        # Cachetes sonrojados
                        pygame.draw.circle(pantalla, (255, 150, 170), (x + 7, y + 16), 3)
                        pygame.draw.circle(pantalla, (255, 150, 170), (x + 23, y + 16), 3)
                        
                        # Corazoncitos flotantes
                        offset_heart = math.sin(trampa.animacion * 2) * 2
                        pygame.draw.circle(pantalla, (255, 100, 150), (x + 5, y + 5 - offset_heart), 2)
                        pygame.draw.circle(pantalla, (255, 100, 150), (x + 25, y + 5 - offset_heart), 2)
                    
                    for p in juego.particulas:
                        p.dibujar(pantalla)
                    
                    juego.mostrar_hud(pantalla)
                    pygame.display.flip()
                    
                reloj.tick(juego.obtener_velocidad())

if __name__ == '__main__':
    main()