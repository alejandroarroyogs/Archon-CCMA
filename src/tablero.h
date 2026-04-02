#pragma once

class Pieza;

const int TAM_TABLERO = 9;

extern Pieza* tableroPiezas[TAM_TABLERO][TAM_TABLERO];

extern int filaSeleccionada;
extern int colSeleccionada;

extern bool piezaSeleccionada;
extern int filaOrigen;
extern int colOrigen;

extern int turnoActual;

extern bool enCombate;
extern Pieza* atacante;
extern Pieza* defensor;

extern int combateFilaOrigen;
extern int combateColOrigen;
extern int combateFilaDestino;
extern int combateColDestino;

extern int tiempoEntreRondas;
extern bool combateResuelto;

extern bool combateIniciado;

extern int flashAtacante;
extern int flashDefensor;

void InicializarTablero();
void LiberarTablero();

bool MovimientoValido(int filaO, int colO, int filaD, int colD);
void DibujarMovimientosValidos();

void ResolverCombate(bool ganaAtacante);

void ResolverCombateAutomatico();

void IniciarCombateAutomatico();
void EjecutarRondaCombate();