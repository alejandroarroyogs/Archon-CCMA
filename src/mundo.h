#pragma once
#include "arena.h"
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "jugador.h" // AÑADIDO

// Estado del juego
enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, GAMEOVER, SELEC_MODO };
extern Estado estado;
extern int modoJuego;

class Mundo
{
    jugador* j1;
    jugador* j2;
public:
    Mundo();
    virtual ~Mundo();
    void Inicializar();
    void Dibujar();
    Interfaz interfaz;
    Tablero tablero;
    Arena arena;
    

    void tecla(unsigned char key);
    void Timer(int value);
    void cambiaCiclo();

};

