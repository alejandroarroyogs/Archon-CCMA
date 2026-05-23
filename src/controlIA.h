#pragma once
#include "tablero.h"
#include <vector>

enum Dificultad { FACIL, MEDIO, DIFICIL };

class ControlIA {

	static bool PtoPoder(Tablero& tablero, int f, int c);
	static bool rivalCombate(Tablero& tablero, int fD, int cD, int miBando);

public:
	struct Mov {
		int f0, c0, fD, cD;
		int punt;
	};
	static void ejecutarturno(Tablero& tablero, Dificultad nivel);
};

