#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "hechizos.h"
#include "arena.h"
#include "jugador.h" // NUEVO

Estado estado = MENU;
int modoJuego = 0;

Mundo::Mundo(){
 
    j1 = 0;
    j2 = 0;
    
}

Mundo::~Mundo() {

    // NUEVO - IMPORTANTE: Borrar jugadores para evitar fugas de memoria
    if (j1) delete j1;
    if (j2) delete j2;

    for (Pieza* p : listaPiezas) {
        delete p; // Borramos cada pieza creada con 'new'
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
        arena.ponMusica();
        arena.dibuja();
        break;
    case JUGANDO:
        tablero.dibuja();
        break;
    case GAMEOVER:
        break;
    }
    
};

void Mundo::tecla(unsigned char key)
{
    switch (estado){

    case JUGANDO: // NUEVO
        // Solo dejamos usar el teclado si el turno actual es de un jugador humano
     if ((tablero.turnoActual == 1 && j1 != 0 && !j1->esIA()) ||
        (tablero.turnoActual == 2 && j2 != 0 && !j2->esIA()))
     {
            tablero.tecla(key);
      }
        break;
    }
   
 
}
void Mundo::teclaLiberada(unsigned char key)
{
    if (estado == JUGANDO) {
        tablero.teclaLiberada(key);
    }
}

void Mundo::Timer(int value) // NUEVO
{
    if (estado == JUGANDO) {
        // 1. ACTUALIZACIÓN DEL CURSOR HUMANO (WASD):
        // Esto lee las teclas pulsadas en cada frame para que el movimiento sea continuo y fluido
        tablero.actualizarMovimiento();

        // 2. LÓGICA DE LA IA (Si corresponde):
        if (modoJuego == 2 && j2 != 0 && j2->esIA() && tablero.turnoActual == 2) {
            tablero.moverIA();
        }

        // Forzamos a OpenGL a redibujar la pantalla para ver el movimiento del cursor
        glutPostRedisplay();
    }
}

void Mundo::inicializarPartida()
{

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



