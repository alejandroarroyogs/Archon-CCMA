#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "hechizos.h"
#include "arena.h"
#include "jugador.h"

Estado estado = JUGANDO;
int modoJuego = 0;

Mundo::Mundo() {
    j1 = 0;
    j2 = 0;
}

Mundo::~Mundo() {
    if (j1) delete j1;
    if (j2) delete j2;

    for (Pieza* p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
}

void Mundo::Inicializar() {
    inicializarPartida();
}

void Mundo::Dibujar()
{
    switch (estado) {
    case MENU:
        interfaz.dibujaMenu();
        break;
    case INSTRUC:
        interfaz.dibujaInstrucciones();
        break;
    case SELEC_MODO:
        interfaz.eligeModo();
        break;
    case COMBATE:
        // Dibujamos la arena que ya fue inicializada correctamente desde tablero.cpp
        arena.setTurno(tablero.turnoActual);
        arena.ponMusica();
        arena.dibuja();
        break;
    case JUGANDO:
        tablero.dibuja();
        break;
    case GAMEOVER:
        break;
    }
}

void Mundo::tecla(unsigned char key)
{
    switch (estado) {
    case JUGANDO:
        // Verificamos si el jugador actual es humano antes de pasar la tecla
        if ((tablero.turnoActual == 1 && j1 != 0 && !j1->esIA()) ||
            (tablero.turnoActual == 2 && j2 != 0 && !j2->esIA()))
        {
            // Ahora tablero.tecla ya gestiona internamente los hechizos (1-9)
            tablero.tecla(key);
        }
        break;

    case COMBATE:
        
        break;
    }
}
void Mundo::teclaLiberada(unsigned char key)
{
    if (estado == JUGANDO) {
        tablero.teclaLiberada(key);
    }
}

void Mundo::Timer(int value)
{
    if (estado == JUGANDO) {
        tablero.actualizarMovimiento();

        if (modoJuego == 2 && j2 != 0 && j2->esIA() && tablero.turnoActual == 2) {
            tablero.moverIA();
        }

        glutPostRedisplay();
    }
}

void Mundo::inicializarPartida()
{
    if (j1) delete j1;
    if (j2) delete j2;

    j1 = new jugador(1, false); // Humano (Jedi)
    j2 = new jugador(2, false); // Humano (Sith)

    tablero.inicializa();
}

void Mundo::calcScore()
{
    scoreJEDI = 0;
    scoreSITH = 0;

    for (Pieza* p : listaPiezas) {
        if (p->EstaViva()) {
            if (p->EsAzul()) scoreJEDI += p->GetVida();
            else scoreSITH += p->GetVida();
        }
    }
}

void Mundo::cambiaCiclo()
{
    tablero.turnoActual = (tablero.turnoActual == 1) ? 2 : 1;
}