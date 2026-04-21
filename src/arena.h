#pragma once
#include "freeglut.h"
#include "piezas.h"

class Arena
{
	Pieza* atacante;
	Pieza* defensor;
	int flashAtacante;
	int flashDefensor;
	void dibujaBarraVida(float x, float y, float z, int vidaActual, int vidaMax);

public:
	Arena();
	virtual ~Arena();
	void CargaCombate(Pieza* a, Pieza* d, int fA, int fD);
	void dibujaCombate();
	void tiempoCombate();
	void EjecutarRondaCombate();
	void ResolverCombate();
};
