#pragma once
#include <string>

/// ==================== LÓGICA Y PANTALLAS DEL JUEGO ====================

const int CANTIDAD_DE_DADOS = 5;
const int CANTIDAD_DE_JUGADAS = 10;   // Unos a Seises, Escalera, Full, Póker y Generala
const int CANTIDAD_DE_RONDAS = 10;    // Una ronda por cada jugada de la planilla
const int TIRADAS_POR_TURNO = 3;

// Datos de cada jugador
struct Jugador
{
    std::string nombre;
    int numero;                          // 1 o 2 (define su color)
    int planilla[CANTIDAD_DE_JUGADAS];   // Puntos anotados en cada jugada
    bool usada[CANTIDAD_DE_JUGADAS];     // true si esa jugada ya se anotó o se tachó
};

// Nombre de cada jugada de la planilla (índice 0 a 9)
std::string nombreJugada(int jugada);

// Tira los dados que no están guardados
void tirarDados(int dados[], bool guardados[]);

// Cuenta cuántos dados salieron con cada número (cantidad[1] = cuántos unos, etc.)
void contarDados(int dados[], int cantidad[]);

// Detecta las jugadas especiales
bool esEscalera(int dados[]);
bool esFull(int dados[]);
bool esPoker(int dados[]);
bool esGenerala(int dados[]);

// Puntos que vale anotar esos dados en una jugada. "servida" = salió en la primera tirada (suma 5 de bonus)
int puntosDeLaJugada(int jugada, int dados[], bool servida);

// Total de la planilla de un jugador
int totalDelJugador(Jugador jugador);

// Juega una partida completa. Devuelve el índice del ganador (-1 si empataron)
int jugarPartida(Jugador jugadores[], int cantidadDeJugadores);

// Pide el nombre de un jugador hasta que sea válido (no vacío, hasta 15 letras, distinto del otro)
std::string pedirNombreValido(int numero, std::string nombreOtro, int fila);
