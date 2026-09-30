# Generala

El clásico juego de dados para **1 o 2 jugadores**, en consola, hecho en **C++**.

![Turno de la Generala](https://federicowachenschwan.github.io/portfolio/img/generala/turno.jpg)

## Qué tiene

- 10 rondas con 3 tiradas por turno: se eligen los dados que se guardan y se vuelve a tirar el resto.
- Planilla con las 10 jugadas (del 1 al 6, Escalera, Full, Póker y Generala), bonus por jugada servida y Generala servida que gana en el acto.
- Reglas, récord de la sesión y una interfaz tipo "mesa de casino" con dados dibujados.

## Qué demuestra

- `struct` para modelar a cada jugador y su planilla.
- Separación en módulos: lógica del juego (`generala`) y dibujo en pantalla (`pantalla`).
- Funciones para detectar cada jugada y validaciones de entrada.

## Cómo compilarlo

```
g++ -std=gnu++11 main.cpp generala.cpp pantalla.cpp -o generala
```

Proyecto individual de **Federico Wachenschwan** · [Ver el video en mi portfolio](https://federicowachenschwan.github.io/portfolio/#proyectos)
