#pragma once
#include "piezas.h" 
#include "hechizos.h"
#include <vector>

extern class Mundo mundo;

class Tablero {
    static const int TAM_TABLERO = 9;
    Pieza* casillas[TAM_TABLERO][TAM_TABLERO];
    bool teclasPulsadas[256];
    int cooldownMovimiento;

    // --- NUEVO: Gestión de Hechizos ---
    std::vector<Hechizo*> hechizosAzules;
    std::vector<Hechizo*> hechizosRojos;

public:
    Tablero();
    virtual ~Tablero();

    void inicializa();
    void dibuja();
    void dibujaInterfazHechizos(); // Para ver la lista en el tablero

    void tecla(unsigned char key);
    void teclaLiberada(unsigned char key);
    void actualizarMovimiento();
    void dibujaBarraVida(float x, float z, int vidaActual, int vidaMax);

    // Lógica de lanzamiento
    void lanzarHechizo(int indice);

    static int getTamTablero();
    int filaSeleccionada, colSeleccionada;
    bool piezaSeleccionada;
    int filaOrigen, colOrigen;
    int turnoActual;

    void moverIA();
    int getTurno() { return turnoActual; }
    Pieza* getPiezaEnCursor();
};