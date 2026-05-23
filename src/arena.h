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
private:
	Pieza* atacante;
	Pieza* defensor;

	int flashAtacante;
	int flashDefensor;
	int turno;

	std::vector<Estrella*> estrellas;

	// Posiciones en la plataforma 3D
	float xA, zA;
	float xD, zD;

	// Velocidad de movimiento
	float velocidad;

	// Control de colisiones
	int cooldownDanio;

	//Origen y destino del movimiento
	int fOrigen, cOrigen, fDestino, cDestino;

public:
	// Arrays para detectar teclas mantenidas
	bool teclas[256];
	bool teclasEspeciales[256];

	Arena();
	virtual ~Arena();

	void inicializa(Pieza* a, Pieza* b, int turnoInicial);
	void actualiza();

	// Funciones de teclado para el input continuo
	void keyDown(unsigned char key);
	void keyUp(unsigned char key);
	void specialKeyDown(int key);
	void specialKeyUp(int key);

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