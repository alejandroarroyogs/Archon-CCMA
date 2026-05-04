#pragma once
#include "ETSIDI.h"

class Interfaz
{
    int modoJuego;
    int estado;
    int ratontexto = -1; //-1 ninguno, 0 jugar, 1 instrucc
public:
    void dibujaMenu();
    void dibujaInstrucciones();
    void dibujaTexto(float x, float y, const char* texto, int idBoton);
    void eligeModo();
    void dibujaFondo();
    void gestionRaton(int boton, int estdo, int x, int y);
    void movimientoRaton(int x, int y);

};


