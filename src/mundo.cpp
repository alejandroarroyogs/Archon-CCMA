#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "hechizos.h"
#include "arena.h"
#include "jugador.h" // NUEVO

Estado estado = COMBATE;
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
<<<<<<< HEAD
    inicializarPartida();
=======
    tablero.inicializa();
>>>>>>> parent of c9923c3 (Merge branch 'master' into hechizos)

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

void Mundo::Timer(int value)
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
void Mundo::inicializarPartida()
{
    // Borramos por si acaso ya existían de una partida anterior
    if (j1) delete j1;
    if (j2) delete j2;

    // Creamos dos jugadores humanos (bando, es_IA)
    j1 = new jugador(1, false); // Humano (Jedi)
    j2 = new jugador(2, false); // Humano (Sith)

    // Inicializamos el tablero
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
    // Cambiamos el turno del tablero (si es 1 pasa a 2, si es 2 pasa a 1)
    tablero.turnoActual = (tablero.turnoActual == 1) ? 2 : 1;
}



