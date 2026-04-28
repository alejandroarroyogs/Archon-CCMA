#pragma once
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "jugador.h" // AÑADIDO

// Estado del juego
enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, GAMEOVER, SELEC_MODO };

class Mundo
{
    Estado estado;
    int modoJuego;

    // AÑADIDO: Punteros a los jugadores
    jugador* j1;
    jugador* j2;

public:
    Mundo();
    virtual ~Mundo();

    void Inicializar();
    void Dibujar();
    Interfaz interfaz;
    Tablero tablero;
    

    void tecla(unsigned char key);
    void Timer(int value);

};

