#pragma once
#include "ETSIDI.h"

class Interfaz
{
    int modoJuego;
    int estado;
public:
    void dibujaMenu();
    void dibujaInstrucciones();
    void dibujaTexto(float x, float y, const char* texto);
    void eligeModo();
    void dibujaFondo();
    void gestionRaton(int boton, int estado, int x, int y);
    bool clickEnRectangulo(int mouseX, int mouseY, int x, int y, int ancho, int alto);
};


