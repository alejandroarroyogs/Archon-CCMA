#pragma once
#include "ETSIDI.h"
#include "freeglut.h"


class Fin
{
public:
	void escribe(float x, float y, const char* texto);
	void dibuja();
	void dibujaFondo();
	void ponMusica();
	void stopMusica();
};

