#pragma once
#include "freeglut.h"
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

	std::vector<std::string> fondos;
	int fondoActual;

public:
	Arena();
	virtual ~Arena();
	
	//void inicializa(Pieza* a, Pieza* b);
	void cargarFondos();
	void elegirFondo();
	void ponMusica();
	void dibuja();
	void dibujaFondo();
	void dibujaPlataforma();
	void dibujaUI();
	void BarraVida();
	
};
