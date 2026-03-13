#pragma once

const int TAM_TABLERO = 9;

extern int tableroPiezas[TAM_TABLERO][TAM_TABLERO];

extern int filaSeleccionada;
extern int colSeleccionada;

extern bool piezaSeleccionada;
extern int filaOrigen;
extern int colOrigen;

extern int turnoActual;

void InicializarTablero();
bool MovimientoValido(int filaO, int colO, int filaD, int colD);
void DibujarMovimientosValidos();
