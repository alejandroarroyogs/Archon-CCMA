#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "hechizos.h"
#include "arena.h"
#include "jugador.h" // NUEVO

Estado estado = JUGANDO;
int modoJuego = 0;

Mundo::Mundo(){
 
 
    j1 = 0;
    j2 = 0;
    
}

Mundo::~Mundo() {

    // NUEVO - IMPORTANTE: Borrar jugadores para evitar fugas de memoria
    if (j1) delete j1;
    if (j2) delete j2;
}
void Mundo::Inicializar() {
    tablero.inicializa();
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

    glutPostRedisplay();
 
}


void Mundo::Timer(int value) // NUEVO
{
    // LÓGICA DE LA IA:
    // Si estamos jugando, es modo IA, y es el turno del bando 2...
    if (estado == JUGANDO && modoJuego == 2) {
        if (j2 != 0 && j2->esIA() && tablero.turnoActual == 2) {
            tablero.moverIA(); // La función que creamos antes
            glutPostRedisplay();
        }
    }
}

void Mundo::cambiaCiclo()
{
}
