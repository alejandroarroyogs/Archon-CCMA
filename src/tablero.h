#pragma once
#include "piezas.h" // Necesitamos conocer las piezas

class Tablero {

    // uso static para el tamaño del tablero
    static const int TAM_TABLERO = 9;
    Pieza* casillas[TAM_TABLERO][TAM_TABLERO];

public:
    Tablero();          // Constructor
    virtual ~Tablero(); // Destructor (vital para no dejar basura en memoria)

    void inicializa();
    void dibuja();
    //acceso a la variable tam_tablero
    static int getTamTablero();

    int filaSeleccionada;
    int colSeleccionada;

    bool piezaSeleccionada;
    int filaOrigen;
    int colOrigen;

    int turnoActual;
};