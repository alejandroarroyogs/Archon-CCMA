#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"

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
    
};

void Mundo::tecla(unsigned char key)
{
 
};

void Mundo::Timer(int value)
{
    
};
