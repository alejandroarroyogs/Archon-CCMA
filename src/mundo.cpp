#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "arena.h"

Mundo::Mundo() {
    estado = JUGANDO; // Ponlo en JUGANDO para probar si se ve el tablero
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
        arena.dibujaCombate();
        break;
    case GAMEOVER:

    }
    
};

void Mundo::tecla(unsigned char key)
{
 
}


void Mundo::Timer(int value)
{
    
};
