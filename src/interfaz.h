#pragma once
#include <vector>
#include "freeglut.h"
#include "hechizos.h"

class Interfaz
{
    int ratontexto = -1; //-1 ninguno, 0 jugar, 1 instrucc
    enum SubMenu { INICIO_INSTRUC, OBJETIVO, MOVIMIENTO, COMBATE, PODERES, PUNTUACION };
    SubMenu subMenuActual = INICIO_INSTRUC;

public:
    void gestionRaton(int boton, int estdo, int x, int y);
    void movimientoRaton(int x, int y);
    void dibujaFondo();
    void dibujaTexto(float x, float y, const char* texto, int idBoton);
    void eligeModo();
    void dibujaMenu();
    void dibujaInstrucciones();
};


