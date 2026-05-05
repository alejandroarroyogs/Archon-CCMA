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

	std::vector<Hechizo*> hechizosAzules;
	std::vector<Hechizo*> hechizosRojos;
	std::vector<Estrella*> estrellas; //mem dinam

public:
	Arena();
	virtual ~Arena();
	
	void inicializa(Pieza* a, Pieza* b, int turnoInicial);
	void ponMusica();
	void dibuja();
	void dibujaPlataforma();
	void BarraVida();
	void dibujoHechizos();
	void dibujaTexto(float x, float y, const char* texto);
	void lanzarHechizo(int indice);
	void setTurno(int t) { turno = t; }
	void inicializaEstrellas(int cantidad);
	void dibujaFondo();
};
