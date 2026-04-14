#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "arena.h"

Mundo::Mundo() {
    estado =SELEC_MODO; // Ponlo en JUGANDO para probar si se ve el tablero
}

Mundo::~Mundo() {
    LiberarTablero();
}
void Mundo::Inicializar() {
    InicializarTablero();
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
    }
    
};

void Mundo::tecla(unsigned char key)
{
    switch (estado) {
    case MENU:
        interfaz.tecladoinicio(key,0,0);
        break;
    case INSTRUC:
        interfaz.tecladoinicio(key,0,0);
        break;
    case SELEC_MODO:
        interfaz.tecladoinicio(key,0,0);
        break;
    case COMBATE:
        //arena.dibujaCombate();
        break;
    case GAMEOVER:
        break;
    }
 
}


void Mundo::Timer(int value)
{
    
};
