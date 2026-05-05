#pragma once
#include "piezas.h" 

// Para que el tablero sepa que existe el objeto global 'mundo'
extern class Mundo mundo;

class Tablero {
    static const int TAM_TABLERO = 9;
    Pieza* casillas[TAM_TABLERO][TAM_TABLERO];

    bool teclasPulsadas[256];
    int cooldownMovimiento;

public:
    Tablero();
    virtual ~Tablero();

    void inicializa();
    void dibuja();

    // Gestión de teclado y movimiento
    void tecla(unsigned char key);
    void teclaLiberada(unsigned char key);
    void actualizarMovimiento();

    static int getTamTablero();

    int filaSeleccionada;
    int colSeleccionada;

    bool piezaSeleccionada;
    int filaOrigen;
    int colOrigen;

    int turnoActual;

    void moverIA();
    int getTurno() { return turnoActual; }
    Pieza* getPiezaEnCursor();
};