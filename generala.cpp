#include <iostream>
#include <string>
#include <cstdlib>
#include "generala.h"
#include "pantalla.h"
#include "rlutil.h"
using namespace std;

/// ==================== POSICIONES DE LA PANTALLA DEL TURNO ====================
/// Recuadro de la columna 2 a la 119 y de la fila 1 a la 29.
/// Panel izquierdo (dados y controles): columnas 3 a 77, centro 40.
/// Panel derecho (planilla): columnas 80 a 118, centro 99.
const int CENTRO_IZQ = 40;
const int CENTRO_DER = 99;
const int FILA_DADOS = 8;
const int X_PRIMER_DADO = 12;   // 5 dados de 9 columnas con 3 de espacio: 12, 24, 36, 48, 60

/// ==================== LÓGICA DEL JUEGO ====================

string nombreJugada(int jugada)
{
    string nombres[CANTIDAD_DE_JUGADAS] = { "Unos", "Doses", "Treses", "Cuatros", "Cincos", "Seises",
                                            "Escalera", "Full", "Póker", "Generala" };
    return nombres[jugada];
}

void tirarDados(int dados[], bool guardados[])
{
    for (int i = 0; i < CANTIDAD_DE_DADOS; i++)
    {
        if (guardados[i] == false)
        {
            dados[i] = (rand() % 6) + 1;
        }
    }
}

void contarDados(int dados[], int cantidad[])
{
    for (int n = 0; n <= 6; n++)
    {
        cantidad[n] = 0;
    }
    for (int i = 0; i < CANTIDAD_DE_DADOS; i++)
    {
        cantidad[dados[i]]++;
    }
}

bool esEscalera(int dados[])
{
    // Escalera: 1-2-3-4-5 o 2-3-4-5-6 (cinco números distintos seguidos)
    int cantidad[7];
    contarDados(dados, cantidad);
    bool menor = cantidad[1] == 1 && cantidad[2] == 1 && cantidad[3] == 1 && cantidad[4] == 1 && cantidad[5] == 1;
    bool mayor = cantidad[2] == 1 && cantidad[3] == 1 && cantidad[4] == 1 && cantidad[5] == 1 && cantidad[6] == 1;
    return menor || mayor;
}

bool esFull(int dados[])
{
    // Full: tres dados iguales y otros dos iguales entre sí
    int cantidad[7];
    contarDados(dados, cantidad);
    bool hayTres = false, hayDos = false;
    for (int n = 1; n <= 6; n++)
    {
        if (cantidad[n] == 3) hayTres = true;
        if (cantidad[n] == 2) hayDos = true;
    }
    return hayTres && hayDos;
}

bool esPoker(int dados[])
{
    // Póker: cuatro dados iguales
    int cantidad[7];
    contarDados(dados, cantidad);
    for (int n = 1; n <= 6; n++)
    {
        if (cantidad[n] == 4) return true;
    }
    return false;
}

bool esGenerala(int dados[])
{
    // Generala: los cinco dados iguales
    for (int i = 1; i < CANTIDAD_DE_DADOS; i++)
    {
        if (dados[i] != dados[0]) return false;
    }
    return true;
}

int puntosDeLaJugada(int jugada, int dados[], bool servida)
{
    int bonus = 0;
    if (servida == true) bonus = 5;

    if (jugada <= 5)   // Unos a Seises: se suman los dados de ese número
    {
        int numero = jugada + 1;
        int suma = 0;
        for (int i = 0; i < CANTIDAD_DE_DADOS; i++)
        {
            if (dados[i] == numero) suma += numero;
        }
        return suma;
    }
    if (jugada == 6 && esEscalera(dados)) return 20 + bonus;
    if (jugada == 7 && esFull(dados)) return 30 + bonus;
    if (jugada == 8 && esPoker(dados)) return 40 + bonus;
    if (jugada == 9 && esGenerala(dados)) return 50;
    return 0;   // Si no se formó la jugada, anotarla es tacharla (0 puntos)
}

int totalDelJugador(Jugador jugador)
{
    int total = 0;
    for (int j = 0; j < CANTIDAD_DE_JUGADAS; j++)
    {
        if (jugador.usada[j] == true) total += jugador.planilla[j];
    }
    return total;
}

// Nombre de la mejor jugada especial que formaron los dados (para mostrarlo como ayuda)
string jugadaDetectada(int dados[])
{
    if (esGenerala(dados)) return "¡GENERALA!";
    if (esPoker(dados)) return "Póker";
    if (esFull(dados)) return "Full";
    if (esEscalera(dados)) return "Escalera";
    return "";
}

/// ==================== DIBUJO DE LA PANTALLA DEL TURNO ====================

void dibujarDadosDelTurno(int dados[], bool guardados[], int color)
{
    for (int i = 0; i < CANTIDAD_DE_DADOS; i++)
    {
        int x = X_PRIMER_DADO + i * 12;

        // Los dados son blancos; los guardados se pintan de verde y llevan un cartel abajo
        if (guardados[i] == true) dibujarDado(dados[i], x, FILA_DADOS, rlutil::LIGHTGREEN);
        else dibujarDado(dados[i], x, FILA_DADOS, rlutil::WHITE);

        rlutil::setColor(color);
        rlutil::locate(x + 3, FILA_DADOS + 5);
        cout << "[" << i + 1 << "]";

        rlutil::locate(x, FILA_DADOS + 6);
        if (guardados[i] == true)
        {
            rlutil::setColor(rlutil::LIGHTGREEN);
            cout << "GUARDADO ";
        }
        else
        {
            cout << "         ";
        }
    }
}

// Planilla de puntos (panel derecho). Si "elegida" es >= 0, se marca esa fila y se muestran los puntos posibles
void dibujarPlanilla(Jugador jugadores[], int cantidad, int actual, int dados[], bool servida, int elegida)
{
    rlutil::setColor(rlutil::YELLOW);
    escribirCentrado("PLANILLA", CENTRO_DER, 3);

    // Encabezado con los nombres (hasta 9 letras para que entren)
    for (int p = 0; p < cantidad; p++)
    {
        rlutil::setColor(colorJugador(jugadores[p].numero));
        escribirCentrado(jugadores[p].nombre.substr(0, 9), 103 + p * 10, 5);
    }

    for (int j = 0; j < CANTIDAD_DE_JUGADAS; j++)
    {
        int fila = 7 + j;
        limpiarFila(80, 118, fila);

        // Nombre de la jugada (con una flecha si es la que se está eligiendo)
        rlutil::locate(81, fila);
        if (j == elegida)
        {
            rlutil::setColor(rlutil::YELLOW);
            cout << "► " << nombreJugada(j);
        }
        else
        {
            rlutil::setColor(rlutil::GREY);
            cout << "  " << nombreJugada(j);
        }

        // Valor de cada jugador
        for (int p = 0; p < cantidad; p++)
        {
            string valor;
            if (jugadores[p].usada[j] == true)
            {
                rlutil::setColor(rlutil::WHITE);
                if (jugadores[p].planilla[j] == 0) valor = "X";
                else valor = to_string(jugadores[p].planilla[j]);
            }
            else if (p == actual && elegida >= 0)
            {
                // Mientras elige: muestra en gris cuánto valdría anotar ahí
                rlutil::setColor(rlutil::DARKGREY);
                if (j == elegida) rlutil::setColor(rlutil::YELLOW);
                valor = to_string(puntosDeLaJugada(j, dados, servida));
            }
            else
            {
                rlutil::setColor(rlutil::DARKGREY);
                valor = "·";
            }
            escribirCentrado(valor, 103 + p * 10, fila);
        }
    }

    // Totales
    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(81, 18);
    cout << "  TOTAL";
    for (int p = 0; p < cantidad; p++)
    {
        rlutil::setColor(colorJugador(jugadores[p].numero));
        escribirCentrado(to_string(totalDelJugador(jugadores[p])) + " ", 103 + p * 10, 18);
    }
}

void dibujarMarcoDelTurno(Jugador jugadores[], int cantidad, int actual, int ronda, int tirada)
{
    int color = colorJugador(jugadores[actual].numero);
    rlutil::cls();
    // La banda del título va del color del jugador y el marco de la mesa, en verde
    dibujarVentana(2, 1, 119, 29, "RONDA " + to_string(ronda) + " DE " + to_string(CANTIDAD_DE_RONDAS), color);
    rlutil::setColor(rlutil::GREEN);
    dibujarRecuadro(2, 1, 119, 29);

    // División vertical entre los dados y la planilla
    for (int f = 3; f <= 28; f++)
    {
        rlutil::locate(79, f);
        cout << "│";
    }
    rlutil::locate(80, 17);
    for (int i = 80; i <= 118; i++) cout << "─";

    // Encabezado del turno
    rlutil::setColor(color);
    escribirCentrado("TURNO DE " + jugadores[actual].nombre, CENTRO_IZQ, 3);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Tirada " + to_string(tirada) + " de " + to_string(TIRADAS_POR_TURNO) +
                     "   ·   Puntos: " + to_string(totalDelJugador(jugadores[actual])), CENTRO_IZQ, 4);
}

void dibujarAyuda(int tirada, bool eligiendo)
{
    for (int f = 19; f <= 24; f++) limpiarFila(3, 77, f);
    rlutil::setColor(rlutil::DARKGREY);
    dibujarRecuadroSimple(14, 19, 66, 24);

    if (eligiendo == true)
    {
        rlutil::setColor(rlutil::WHITE);
        escribirCentrado("↑ ↓   Elegir la jugada de la planilla", CENTRO_IZQ, 21);
        escribirCentrado("ENTER   Anotar (si no se formó, se tacha)", CENTRO_IZQ, 22);
        return;
    }

    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(18, 20); cout << "[1] a [5]   Guardar o soltar un dado";
    rlutil::locate(18, 21);
    if (tirada < TIRADAS_POR_TURNO) cout << "[T]         Tirar los dados que no guardaste";
    else { rlutil::setColor(rlutil::DARKGREY); cout << "[T]         Ya usaste las 3 tiradas"; }
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(18, 22); cout << "[A]         Anotar en la planilla";
    rlutil::setColor(rlutil::DARKGREY);
    rlutil::locate(18, 23); cout << "Te quedan " << TIRADAS_POR_TURNO - tirada << " tirada(s)";
}

void mostrarMensaje(string texto, int color)
{
    limpiarFila(3, 77, 26);
    rlutil::setColor(color);
    escribirCentrado(texto, CENTRO_IZQ, 26);
}

// Animación corta de los dados rodando (solo cambian los que no están guardados)
void animarTirada(int dados[], bool guardados[], int color)
{
    int falsos[CANTIDAD_DE_DADOS];
    for (int vuelta = 0; vuelta < 6; vuelta++)
    {
        for (int i = 0; i < CANTIDAD_DE_DADOS; i++)
        {
            if (guardados[i] == true) falsos[i] = dados[i];
            else falsos[i] = (rand() % 6) + 1;
        }
        dibujarDadosDelTurno(falsos, guardados, color);
        rlutil::msleep(70);
    }
}

/// ==================== UN TURNO ====================
/// Devuelve true si el jugador sacó Generala servida (gana al instante)

bool jugarTurno(Jugador jugadores[], int cantidad, int actual, int ronda)
{
    int color = colorJugador(jugadores[actual].numero);
    int dados[CANTIDAD_DE_DADOS] = { 1, 1, 1, 1, 1 };
    bool guardados[CANTIDAD_DE_DADOS] = { false, false, false, false, false };
    int tirada = 1;

    // Primera tirada: los 5 dados
    dibujarMarcoDelTurno(jugadores, cantidad, actual, ronda, tirada);
    dibujarPlanilla(jugadores, cantidad, actual, dados, false, -1);
    dibujarAyuda(tirada, false);
    tirarDados(dados, guardados);
    animarTirada(dados, guardados, color);
    dibujarDadosDelTurno(dados, guardados, color);

    // Generala servida: gana la partida en el acto
    if (esGenerala(dados) == true)
    {
        mostrarMensaje("¡GENERALA SERVIDA! " + jugadores[actual].nombre + " gana la partida", rlutil::LIGHTGREEN);
        rlutil::msleep(1500);
        esperarTecla(CENTRO_IZQ, 27);
        return true;
    }

    // Elegir qué dados guardar y volver a tirar (hasta 3 tiradas)
    bool anotar = false;
    while (anotar == false)
    {
        string detectada = jugadaDetectada(dados);
        if (detectada != "") mostrarMensaje("Formaste: " + detectada, rlutil::LIGHTGREEN);

        if (tirada == TIRADAS_POR_TURNO)
        {
            anotar = true;   // Después de la tercera tirada hay que anotar sí o sí
            break;
        }

        int tecla = rlutil::getkey();
        limpiarFila(3, 77, 26);

        if (tecla >= '1' && tecla <= '5')
        {
            int d = tecla - '1';
            guardados[d] = !guardados[d];
            dibujarDadosDelTurno(dados, guardados, color);
        }
        else if (tecla == 't' || tecla == 'T')
        {
            bool todosGuardados = true;
            for (int i = 0; i < CANTIDAD_DE_DADOS; i++) if (guardados[i] == false) todosGuardados = false;

            if (todosGuardados == true)
            {
                mostrarMensaje("Guardaste todos los dados: soltá alguno o anotá", rlutil::YELLOW);
            }
            else
            {
                tirada++;
                rlutil::setColor(rlutil::DARKGREY);
                limpiarFila(3, 77, 4);
                escribirCentrado("Tirada " + to_string(tirada) + " de " + to_string(TIRADAS_POR_TURNO) +
                                 "   ·   Puntos: " + to_string(totalDelJugador(jugadores[actual])), CENTRO_IZQ, 4);
                tirarDados(dados, guardados);
                animarTirada(dados, guardados, color);
                dibujarDadosDelTurno(dados, guardados, color);
                dibujarAyuda(tirada, false);
            }
        }
        else if (tecla == 'a' || tecla == 'A')
        {
            anotar = true;
        }
    }

    /// ---------- ANOTAR EN LA PLANILLA ----------
    bool servida = (tirada == 1);
    dibujarAyuda(tirada, true);

    // Arranca en la jugada libre que más puntos da
    int elegida = -1, mejor = -1;
    for (int j = 0; j < CANTIDAD_DE_JUGADAS; j++)
    {
        if (jugadores[actual].usada[j] == false && puntosDeLaJugada(j, dados, servida) > mejor)
        {
            mejor = puntosDeLaJugada(j, dados, servida);
            elegida = j;
        }
    }

    while (true)
    {
        dibujarPlanilla(jugadores, cantidad, actual, dados, servida, elegida);
        int puntos = puntosDeLaJugada(elegida, dados, servida);
        if (puntos > 0) mostrarMensaje("Anotar " + nombreJugada(elegida) + ": " + to_string(puntos) + " puntos", rlutil::LIGHTGREEN);
        else mostrarMensaje("Tachar " + nombreJugada(elegida) + " (0 puntos)", rlutil::YELLOW);

        int tecla = rlutil::getkey();
        if (tecla == rlutil::KEY_UP || tecla == rlutil::KEY_DOWN)
        {
            // Busca la siguiente jugada libre hacia arriba o hacia abajo
            int paso = 1;
            if (tecla == rlutil::KEY_UP) paso = -1;
            int j = elegida;
            for (int k = 0; k < CANTIDAD_DE_JUGADAS; k++)
            {
                j = (j + paso + CANTIDAD_DE_JUGADAS) % CANTIDAD_DE_JUGADAS;
                if (jugadores[actual].usada[j] == false) { elegida = j; break; }
            }
        }
        else if (tecla == rlutil::KEY_ENTER)
        {
            jugadores[actual].planilla[elegida] = puntos;
            jugadores[actual].usada[elegida] = true;
            dibujarPlanilla(jugadores, cantidad, actual, dados, servida, -1);
            rlutil::msleep(900);
            return false;
        }
    }
}

/// ==================== PARTIDA COMPLETA ====================

// Pantalla que anuncia de quién es el turno (solo cuando juegan dos)
void anunciarTurno(Jugador jugador, int ronda)
{
    int color = colorJugador(jugador.numero);
    rlutil::cls();
    dibujarVentana(30, 10, 90, 18, "RONDA " + to_string(ronda) + " DE " + to_string(CANTIDAD_DE_RONDAS), color);
    rlutil::setColor(color);
    escribirCentrado("Turno de " + jugador.nombre, 60, 13);
    rlutil::setColor(rlutil::DARKGREY);
    escribirCentrado("Lleva " + to_string(totalDelJugador(jugador)) + " puntos", 60, 14);
    esperarTecla(60, 16);
}

int jugarPartida(Jugador jugadores[], int cantidadDeJugadores)
{
    for (int p = 0; p < cantidadDeJugadores; p++)
    {
        for (int j = 0; j < CANTIDAD_DE_JUGADAS; j++)
        {
            jugadores[p].planilla[j] = 0;
            jugadores[p].usada[j] = false;
        }
    }

    for (int ronda = 1; ronda <= CANTIDAD_DE_RONDAS; ronda++)
    {
        for (int p = 0; p < cantidadDeJugadores; p++)
        {
            if (cantidadDeJugadores == 2) anunciarTurno(jugadores[p], ronda);
            bool servida = jugarTurno(jugadores, cantidadDeJugadores, p, ronda);
            if (servida == true) return p;   // Generala servida: gana ese jugador
        }
    }

    if (cantidadDeJugadores == 1) return 0;
    int t1 = totalDelJugador(jugadores[0]), t2 = totalDelJugador(jugadores[1]);
    if (t1 > t2) return 0;
    if (t2 > t1) return 1;
    return -1;
}

/// ==================== NOMBRES ====================

string pedirNombreValido(int numero, string nombreOtro, int fila)
{
    string nombre;
    while (true)
    {
        limpiarFila(31, 89, fila);
        rlutil::setColor(colorJugador(numero));
        rlutil::locate(40, fila);
        cout << "Nombre del JUGADOR " << numero << ": ";
        getline(cin, nombre);

        int cantLugares = nombre.length();
        int letras = 0;
        for (int i = 0; i < cantLugares; i++) if (nombre[i] != ' ') letras++;

        limpiarFila(31, 89, fila + 1);
        rlutil::setColor(rlutil::YELLOW);
        if (letras == 0) escribirCentrado("El nombre no puede estar vacío.", 60, fila + 1);
        else if (largoTexto(nombre) > 15) escribirCentrado("Puede tener hasta 15 letras.", 60, fila + 1);
        else if (nombre == nombreOtro) escribirCentrado("Ese nombre ya lo eligió el jugador 1.", 60, fila + 1);
        else
        {
            rlutil::setColor(rlutil::LIGHTGREEN);
            escribirCentrado("¡Listo, " + nombre + "!", 60, fila + 1);
            return nombre;
        }
    }
}
