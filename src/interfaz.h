#pragma once
#include "ETSIDI.h"

class Interfaz
{
    //ETSIDI::Sprite fondo{ "imagenes/fondo.png" };
    int modoJuego;
    int estado;
public:
    void dibujaFondo();
    void dibujaMenu();
    void dibujaInstrucciones();
    void dibujaTexto(float x, float y, const char* texto);
    void eligeModo();
};

