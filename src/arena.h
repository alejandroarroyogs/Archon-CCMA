#pragma once
#include "freeglut.h"
#include "piezas.h"

class Arena
{
private:
	Pieza* atacante;
	Pieza* defensor;
	int flashAtacante;
	int flashDefensor;
	void dibujaBarraVida(float x, float y, float z, int vidaActual, int vidaMax);

public:
	void CargaCombate(Pieza* a, Pieza* d, int fA, int fD);
	void dibujaCombate();
	void tiempoCombate();
	void EjecutarRondaCombate();
	void ResolverCombate();
};

bool enCombate = false;
Pieza* atacante = 0;
Pieza* defensor = 0;

int combateFilaOrigen = -1;
int combateColOrigen = -1;
int combateFilaDestino = -1;
int combateColDestino = -1;

int tiempoEntreRondas = 0;
bool combateResuelto = false;

bool combateIniciado = false;

int flashAtacante = 0;
int flashDefensor = 0;