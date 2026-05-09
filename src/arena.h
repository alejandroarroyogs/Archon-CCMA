#pragma once
#include "freeglut.h"
#include "hechizos.h"
#include "piezas.h"
#include <vector>
#include <string>

struct Estrella {
	float x, y, z, brillo;
};

class Arena
{
	Pieza* atacante;
	Pieza* defensor;

	int flashAtacante;
	int flashDefensor;
	int turno;

	std::vector<Estrella*> estrellas; //mem dinam

public:
	Arena();
	virtual ~Arena();
	
	void inicializa(Pieza* a, Pieza* b, int turnoInicial);
	void ponMusica();
	void stopMusica();
	void dibuja();
	void dibujaPlataforma();
	void BarraVida();
	void dibujaTexto(float x, float y, const char* texto);
	void setTurno(int t) { turno = t; }
	void inicializaEstrellas(int cantidad);
	void dibujaFondo();
};
