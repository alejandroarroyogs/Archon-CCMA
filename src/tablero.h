#pragma once
#include "piezas.h" // Necesitamos conocer las piezas

class Tablero {
private:
    // La matriz 9x9 de punteros a la clase base Pieza (Polimorfismo perfecto)
    Pieza* casillas[9][9];

public:
    Tablero();          // Constructor
    virtual ~Tablero(); // Destructor (vital para no dejar basura en memoria)

    void inicializa();
    void dibuja();
};