#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "arena.h"
#include "jugador.h" // NUEVO

Mundo::Mundo() {
    estado =MENU;
    // NUEVO
    modoJuego = 0; 
    j1 = 0;
    j2 = 0;
  // extern int modoJuego; // En lugar de tener la variable flotando globalmente, se convierte modoJuego en un atributo privado de la clase Mundo.
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
    switch (estado)
    {
    case MENU:
        if (key == '1') estado = SELEC_MODO;
        else if (key == '2') estado = INSTRUC;
        break;

    case INSTRUC:
        if (key == 13) estado = SELEC_MODO;
        break;

    case SELEC_MODO: // NUEVO 
        if (j1) delete j1;
        if (j2) delete j2;

        if (key == '1') { // Jugador vs Jugador
            modoJuego = 1;
            j1 = new jugador(1, false); // Humano
            j2 = new jugador(2, false); // Humano
            estado = JUGANDO;
        }
        if (key == '2') { // Jugador vs IA
            modoJuego = 2;
            j1 = new jugador(1, false); // Humano
            j2 = new jugador(2, true);  // IA
            estado = JUGANDO;
        }
        break;

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
};
