#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "rlutil.h"
#include "pantalla.h"
#include "generala.h"
using namespace std;

/// ==================== MENÚ PRINCIPAL ====================

int menuPrincipal()
{
    while (true)
    {
        rlutil::cls();
        dibujarVentana(15, 2, 105, 27, "MENÚ PRINCIPAL", rlutil::GREEN);
        dibujarLogo(4);
        rlutil::setColor(rlutil::DARKGREY);
        escribirCentrado("El clásico juego de dados, para 1 o 2 jugadores", 60, 10);
        rlutil::setColor(rlutil::GREEN);
        dibujarSeparador(15, 105, 11);

        // Opciones: número en amarillo, nombre en blanco y descripción en gris
        string numeros[5] = { "1", "2", "3", "4", "0" };
        string nombres[5] = { "UN JUGADOR", "DOS JUGADORES", "REGLAS", "RÉCORD", "SALIR" };
        string detalles[5] = { "Hacé la mayor cantidad de puntos", "Enfrentá a otra persona",
                               "Cómo se juega y cuánto vale cada jugada", "El mejor puntaje de la sesión", "Cerrar el juego" };
        for (int i = 0; i < 5; i++)
        {
            int fila = 13 + i * 2;
            rlutil::setColor(rlutil::GREEN);  rlutil::locate(30, fila); cout << "[" << numeros[i] << "]";
            rlutil::setColor(rlutil::WHITE);   rlutil::locate(35, fila); cout << nombres[i];
            rlutil::setColor(rlutil::DARKGREY); rlutil::locate(53, fila); cout << detalles[i];
        }

        rlutil::setColor(rlutil::GREEN);
        dibujarSeparador(15, 105, 23);
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Elegí una opción", 60, 25);

        int tecla = rlutil::getkey();
        if (tecla >= '0' && tecla <= '4') return tecla - '0';
    }
}

/// ==================== NOMBRES DE LOS JUGADORES ====================

void pedirNombres(Jugador jugadores[], int cantidad)
{
    rlutil::cls();
    rlutil::showcursor();
    dibujarVentana(30, 9, 90, 20, "NUEVA PARTIDA", rlutil::GREEN);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Hasta 15 letras. No pueden estar vacíos ni ser iguales.", 60, 11);

    jugadores[0].numero = 1;
    jugadores[1].numero = 2;
    jugadores[0].nombre = pedirNombreValido(1, "", 13);
    if (cantidad == 2) jugadores[1].nombre = pedirNombreValido(2, jugadores[0].nombre, 16);
    rlutil::msleep(700);
    rlutil::hidecursor();
}

/// ==================== FIN DE LA PARTIDA ====================

void mostrarFinDePartida(Jugador jugadores[], int cantidad, int ganador, bool nuevoRecord)
{
    rlutil::cls();
    dibujarVentana(25, 2, 95, 28, "FIN DE LA PARTIDA", rlutil::GREEN);

    // Planilla final: una columna por jugador
    for (int p = 0; p < cantidad; p++)
    {
        rlutil::setColor(colorJugador(jugadores[p].numero));
        escribirCentrado(jugadores[p].nombre, 68 + p * 14, 4);
    }
    for (int j = 0; j < CANTIDAD_DE_JUGADAS; j++)
    {
        rlutil::setColor(rlutil::GREY);
        rlutil::locate(36, 6 + j); cout << nombreJugada(j);
        for (int p = 0; p < cantidad; p++)
        {
            string valor = "-";
            if (jugadores[p].usada[j] == true)
            {
                if (jugadores[p].planilla[j] == 0) valor = "X";
                else valor = to_string(jugadores[p].planilla[j]);
            }
            rlutil::setColor(rlutil::WHITE);
            escribirCentrado(valor, 68 + p * 14, 6 + j);
        }
    }
    rlutil::setColor(rlutil::GREEN);
    dibujarSeparador(25, 95, 17);
    rlutil::locate(36, 18); cout << "TOTAL";
    for (int p = 0; p < cantidad; p++)
    {
        rlutil::setColor(colorJugador(jugadores[p].numero));
        escribirCentrado(to_string(totalDelJugador(jugadores[p])), 68 + p * 14, 18);
    }

    rlutil::setColor(rlutil::GREEN);
    dibujarSeparador(25, 95, 20);
    if (cantidad == 1)
    {
        rlutil::setColor(colorJugador(1));
        escribirCentrado("Terminaste con " + to_string(totalDelJugador(jugadores[0])) + " puntos", 60, 22);
    }
    else if (ganador >= 0)
    {
        rlutil::setColor(colorJugador(jugadores[ganador].numero));
        escribirCentrado("¡GANÓ " + jugadores[ganador].nombre + "!", 60, 22);
    }
    else
    {
        rlutil::setColor(rlutil::GREEN);
        escribirCentrado("¡EMPATE!", 60, 22);
    }
    if (nuevoRecord == true)
    {
        rlutil::setColor(rlutil::LIGHTGREEN);
        escribirCentrado("¡NUEVO RÉCORD DE LA SESIÓN!", 60, 23);
    }
    rlutil::setColor(rlutil::GREEN);
    dibujarSeparador(25, 95, 25);
    esperarTecla(60, 26);
}

/// ==================== REGLAS ====================

void mostrarReglas()
{
    rlutil::cls();
    dibujarVentana(10, 1, 110, 29, "REGLAS", rlutil::GREEN);
    int x = 16;
    rlutil::setColor(rlutil::GREEN);  rlutil::locate(x, 3); cout << "CÓMO SE JUEGA";
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(x, 4); cout << "- Se juegan 10 rondas. En cada turno tirás 5 dados hasta 3 veces.";
    rlutil::locate(x, 5); cout << "- Entre tirada y tirada podés guardar los dados que quieras (teclas 1 a 5).";
    rlutil::locate(x, 6); cout << "- Al final del turno anotás el resultado en una jugada libre de la planilla.";
    rlutil::locate(x, 7); cout << "- Si los dados no forman esa jugada, se tacha (0 puntos). Cada jugada se usa una sola vez.";

    rlutil::setColor(rlutil::GREEN);  rlutil::locate(x, 9); cout << "CUÁNTO VALE CADA JUGADA";
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(x, 10); cout << "- Unos a Seises: la suma de los dados de ese número (ej.: tres cincos = 15).";
    rlutil::locate(x, 11); cout << "- Escalera (1-2-3-4-5 o 2-3-4-5-6) ........ 20 puntos";
    rlutil::locate(x, 12); cout << "- Full (tres iguales + dos iguales) ........ 30 puntos";
    rlutil::locate(x, 13); cout << "- Póker (cuatro iguales) ................... 40 puntos";
    rlutil::locate(x, 14); cout << "- Generala (cinco iguales) ................. 50 puntos";

    rlutil::setColor(rlutil::LIGHTGREEN); rlutil::locate(x, 16); cout << "JUGADAS SERVIDAS";
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(x, 17); cout << "- Si Escalera, Full o Póker salen en la primera tirada, valen 5 puntos más.";
    rlutil::locate(x, 18); cout << "- La GENERALA SERVIDA (en la primera tirada) gana la partida en el acto.";

    rlutil::setColor(rlutil::GREEN);  rlutil::locate(x, 20); cout << "CÓMO SE GANA";
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(x, 21); cout << "- Al completar las 10 rondas gana quien sume más puntos en su planilla.";
    rlutil::locate(x, 22); cout << "- Jugando solo, el objetivo es superar tu propio récord.";

    rlutil::setColor(rlutil::GREEN);
    dibujarSeparador(10, 110, 26);
    esperarTecla(60, 27);
}

/// ==================== RÉCORD ====================

void mostrarRecord(int puntajeRecord, string nombreRecord)
{
    rlutil::cls();
    dibujarVentana(30, 9, 90, 19, "RÉCORD DE LA SESIÓN", rlutil::GREEN);
    if (puntajeRecord > 0)
    {
        rlutil::setColor(rlutil::GREEN);
        escribirCentrado(to_string(puntajeRecord) + " puntos", 60, 12);
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("de " + nombreRecord, 60, 14);
    }
    else
    {
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("Todavía no hay récord.", 60, 12);
        rlutil::setColor(rlutil::DARKGREY);
        escribirCentrado("Jugá una partida para marcar el primero.", 60, 14);
    }
    rlutil::setColor(rlutil::GREEN);
    dibujarSeparador(30, 90, 16);
    esperarTecla(60, 17);
}

/// ==================== PROGRAMA PRINCIPAL ====================

int main()
{
    SetConsoleOutputCP(65001);   // Para que se vean bien los tildes, la ñ y los dibujos de los dados
    srand(time(0));
    rlutil::hidecursor();

    Jugador jugadores[2];
    int puntajeRecord = 0;
    string nombreRecord = "";

    while (true)
    {
        int opcion = menuPrincipal();

        if (opcion == 1 || opcion == 2)
        {
            int cantidad = opcion;
            pedirNombres(jugadores, cantidad);
            int ganador = jugarPartida(jugadores, cantidad);

            // ¿Alguien superó el récord de la sesión?
            bool nuevoRecord = false;
            for (int p = 0; p < cantidad; p++)
            {
                int total = totalDelJugador(jugadores[p]);
                if (total > puntajeRecord)
                {
                    puntajeRecord = total;
                    nombreRecord = jugadores[p].nombre;
                    nuevoRecord = true;
                }
            }
            mostrarFinDePartida(jugadores, cantidad, ganador, nuevoRecord);
        }
        else if (opcion == 3)
        {
            mostrarReglas();
        }
        else if (opcion == 4)
        {
            mostrarRecord(puntajeRecord, nombreRecord);
        }
        else if (opcion == 0)
        {
            rlutil::cls();
            dibujarVentana(15, 9, 105, 19, "", rlutil::GREEN);
            dibujarLogo(11);
            rlutil::setColor(rlutil::LIGHTGREEN);
            escribirCentrado("¡Gracias por jugar!", 60, 17);
            rlutil::setColor(rlutil::WHITE);
            rlutil::locate(1, 21);
            rlutil::showcursor();
            return 0;
        }
    }
}
