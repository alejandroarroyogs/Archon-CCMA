#pragma once
#include "arena.h"
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "jugador.h" 

// Estado del juego
enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, GAMEOVER, SELEC_MODO };
extern Estado estado;
extern int modoJuego;

class Mundo
{
    jugador* j1;
    jugador* j2;
    std::vector<Pieza*>listaPiezas;
    int scoreJEDI;
    int scoreSITH;
    bool musicaCombateLanzada = false;

public:
    Mundo();
    virtual ~Mundo();
    void Inicializar();
    void Dibujar();
    Interfaz interfaz;
    Tablero tablero;
    Arena arena;
    //
    void tecla(unsigned char key);
    void teclaLiberada(unsigned char key);
    void Timer(int value);
    //scores generales->TODAVíA TENGO QUE MEJORARLO
    void inicializarPartida();
    void calcScore();
    void cambiaCiclo();
    //MOVIMIENTO IAA
    void turnoIA();

};

