#pragma once
#include <string>

/// ==================== FUNCIONES PARA DIBUJAR EN LA CONSOLA ====================
/// La pantalla de trabajo es de 120 columnas x 30 filas (el tamaño normal de la consola).

// Colores de cada jugador: dorado para el jugador 1 y magenta para el jugador 2
int colorJugador(int numero);

// Cambia el color de las letras y del fondo al mismo tiempo
void colores(int letra, int fondo);

// Cantidad de letras que se ven en pantalla (las letras con tilde ocupan más lugares en el string)
int largoTexto(std::string texto);

// Escribe el texto centrado en la columna "centro", en la fila indicada
void escribirCentrado(std::string texto, int centro, int fila);

// Borra el contenido de una fila entre dos columnas (sin tocar los bordes)
void limpiarFila(int desde, int hasta, int fila);

// Recuadro de línea simple: (x1, y1) esquina de arriba a la izquierda y (x2, y2) la de abajo a la derecha
void dibujarRecuadro(int x1, int y1, int x2, int y2);

// Línea horizontal que divide un recuadro en la fila indicada
void dibujarSeparador(int x1, int x2, int fila);

// Recuadro de línea simple (se usa para la caja de ayuda)
void dibujarRecuadroSimple(int x1, int y1, int x2, int y2);

// Ventana: recuadro con una banda de color arriba donde va el título
void dibujarVentana(int x1, int y1, int x2, int y2, std::string titulo, int color);

// Dado relleno (cara del color indicado y puntos negros). Ocupa 9 columnas y 5 filas
void dibujarDado(int valor, int x, int y, int colorCara);

// Logo GENERALA en letras grandes con un dado a cada lado (5 filas)
void dibujarLogo(int fila);

// Escribe "Presioná cualquier tecla..." en gris y espera una tecla
void esperarTecla(int centro, int fila);
