#pragma once
#include "piezas.h" // Necesitamos conocer las piezas

class Tablero {
//private: por ahora nada a ver si funciona
    // uso static para el tamaño del tablero
    static const int TAM_TABLERO = 9;
    Pieza* casillas[TAM_TABLERO][TAM_TABLERO];

public:
    Tablero();          // Constructor
    virtual ~Tablero(); // Destructor (vital para no dejar basura en memoria)

    void inicializa();
    void dibuja();

    int filaSeleccionada;
    int colSeleccionada;

    bool piezaSeleccionada;
    int filaOrigen;
    int colOrigen;

    int turnoActual;
};