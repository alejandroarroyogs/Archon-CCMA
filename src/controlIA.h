#pragma once
#include "tablero.h"
#include <vector>
class ControlIA {
	static int evalMov(Tablero& tablero, int fD, int cD);
public:
	struct Mov {
		int f0, c0, fD, cD;
		int punt;
	};
	static void ejecutarturno(Tablero& tablero);
};




