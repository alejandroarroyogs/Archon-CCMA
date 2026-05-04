#pragma once
#include <vector>
#include "ETSIDI.h"
#include "freeglut.h"
#include "hechizos.h"


class Interfaz
{
    int modoJuego;
    int estado;
public:
    void dibujaMenu();
    void dibujaInstrucciones();
    void dibujaHechizos(std::vector<Hechizo*>& lista, int turno);
    void dibujaTexto(float x, float y, const char* texto);
};


