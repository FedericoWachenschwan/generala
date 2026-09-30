#include <iostream>
#include <string>
#include "pantalla.h"
#include "rlutil.h"
using namespace std;

/// Estilo visual "mesa de casino": recuadros verdes de línea simple, títulos en una banda de color,
/// dados blancos rellenos con puntos negros y colores dorado (jugador 1) y magenta (jugador 2).

int colorJugador(int numero)
{
    if (numero == 1)
    {
        return rlutil::BROWN;          // dorado
    }
    return rlutil::LIGHTMAGENTA;       // magenta
}

// Cambia el color de las letras y del fondo al mismo tiempo
void colores(int letra, int fondo)
{
    rlutil::setBackgroundColor(fondo);
    rlutil::setColor(letra);
}

int largoTexto(string texto)
{
    int cantLugares = texto.length();
    int largo = 0;

    for (int i = 0; i < cantLugares; i++)
    {
        // Las letras con tilde, la ñ, ¡, ¿ y los símbolos como ● ocupan 2 o 3 lugares dentro del string,
        // pero en pantalla se ven como una sola letra. Los lugares "de más" valen menos de -64, así que no se cuentan.
        if (texto[i] >= -64)
        {
            largo++;
        }
    }
    return largo;
}

void escribirCentrado(string texto, int centro, int fila)
{
    rlutil::locate(centro - largoTexto(texto) / 2, fila);
    cout << texto;
}

void limpiarFila(int desde, int hasta, int fila)
{
    rlutil::locate(desde, fila);
    for (int i = desde; i <= hasta; i++)
    {
        cout << " ";
    }
}

void dibujarRecuadro(int x1, int y1, int x2, int y2)
{
    // Línea de arriba
    rlutil::locate(x1, y1);
    cout << "┌";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "─";
    }
    cout << "┐";

    // Paredes verticales
    for (int j = y1 + 1; j < y2; j++)
    {
        rlutil::locate(x1, j);
        cout << "│";
        rlutil::locate(x2, j);
        cout << "│";
    }

    // Línea de abajo
    rlutil::locate(x1, y2);
    cout << "└";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "─";
    }
    cout << "┘";
}

void dibujarSeparador(int x1, int x2, int fila)
{
    rlutil::locate(x1, fila);
    cout << "├";
    for (int i = x1 + 1; i < x2; i++)
    {
        cout << "─";
    }
    cout << "┤";
}

void dibujarRecuadroSimple(int x1, int y1, int x2, int y2)
{
    dibujarRecuadro(x1, y1, x2, y2);
}

void dibujarVentana(int x1, int y1, int x2, int y2, string titulo, int color)
{
    rlutil::setColor(color);
    dibujarRecuadro(x1, y1, x2, y2);

    // Banda de título: la primera fila de adentro pintada del color, con el título en negro
    if (titulo != "")
    {
        colores(rlutil::BLACK, color);
        limpiarFila(x1 + 1, x2 - 1, y1 + 1);
        escribirCentrado(titulo, (x1 + x2) / 2, y1 + 1);
        colores(rlutil::WHITE, rlutil::BLACK);
    }
}

void dibujarDado(int valor, int x, int y, int colorCara)
{
    // Cada lugar donde puede ir un puntito empieza vacío
    string arribaIzq = " ", arribaDer = " ";
    string medioIzq = " ", centro = " ", medioDer = " ";
    string abajoIzq = " ", abajoDer = " ";

    if (valor >= 2)   // 2, 3, 4, 5 y 6 tienen las esquinas de una diagonal
    {
        arribaIzq = "●";
        abajoDer = "●";
    }
    if (valor >= 4)   // 4, 5 y 6 tienen las otras dos esquinas
    {
        arribaDer = "●";
        abajoIzq = "●";
    }
    if (valor == 6)   // 6 tiene además los dos del medio
    {
        medioIzq = "●";
        medioDer = "●";
    }
    if (valor % 2 == 1)   // 1, 3 y 5 (los impares) tienen el puntito del centro
    {
        centro = "●";
    }

    // Borde de arriba y de abajo con medios bloques, para que el dado se vea redondeado
    colores(colorCara, rlutil::BLACK);
    rlutil::locate(x, y);
    cout << " ▄▄▄▄▄▄▄ ";
    rlutil::locate(x, y + 4);
    cout << " ▀▀▀▀▀▀▀ ";

    // Cara del dado: fondo del color de la cara y puntos negros
    colores(rlutil::BLACK, colorCara);
    rlutil::locate(x, y + 1);
    cout << "  " << arribaIzq << "   " << arribaDer << "  ";
    rlutil::locate(x, y + 2);
    cout << "  " << medioIzq << " " << centro << " " << medioDer << "  ";
    rlutil::locate(x, y + 3);
    cout << "  " << abajoIzq << "   " << abajoDer << "  ";

    colores(rlutil::WHITE, rlutil::BLACK);
}

void dibujarLogo(int fila)
{
    // Un dado a cada lado, con el color de cada jugador
    dibujarDado(5, 23, fila, colorJugador(1));
    dibujarDado(6, 89, fila, colorJugador(2));

    // "GENERALA" en letras grandes (47 columnas, arranca en la columna 37)
    rlutil::setColor(rlutil::LIGHTGREEN);
    rlutil::locate(37, fila);
    cout << " ████ █████ █   █ █████ ████   ███  █      ███ ";
    rlutil::locate(37, fila + 1);
    cout << "█     █     ██  █ █     █   █ █   █ █     █   █";
    rlutil::locate(37, fila + 2);
    cout << "█  ██ ████  █ █ █ ████  ████  █████ █     █████";
    rlutil::locate(37, fila + 3);
    cout << "█   █ █     █  ██ █     █  █  █   █ █     █   █";
    rlutil::locate(37, fila + 4);
    cout << " ███  █████ █   █ █████ █   █ █   █ █████ █   █";
}

void esperarTecla(int centro, int fila)
{
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Presioná cualquier tecla para continuar", centro, fila);
    rlutil::msleep(300);
    rlutil::anykey();
    rlutil::setColor(rlutil::WHITE);
    rlutil::cls();
}
