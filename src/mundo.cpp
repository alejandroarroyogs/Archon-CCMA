#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "arena.h"

Mundo::Mundo() {
    estado =MENU;// Ponlo en JUGANDO para probar si se ve el tablero
    modoJuego = 0;
}

Mundo::~Mundo() {
    
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
       //arena.dibujaCombate();
        break;
    case GAMEOVER:
        break;
    case JUGANDO:
        tablero.dibuja();
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

    case SELEC_MODO:
        if (key == '1') { modoJuego = 1; estado = JUGANDO; }
        if (key == '2') { modoJuego = 2; estado = JUGANDO; }
        break;
    }

    glutPostRedisplay();
 
}


void Mundo::Timer(int value)
{
    
};
