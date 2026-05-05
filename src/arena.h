#pragma once
#include "freeglut.h"
#include "hechizos.h"
#include "piezas.h"
#include <ETSIDI.h>
#include <vector>
#include <string>

class Arena
{
	Pieza* atacante;
	Pieza* defensor;

	int flashAtacante;
	int flashDefensor;

public:
	Arena();
	virtual ~Arena();
	
	void inicializa(Pieza* a, Pieza* b);
	void ponMusica();
	void dibuja();
	void dibujaPlataforma();
	void BarraVida();
	void dibujoHechizos();
	
};
