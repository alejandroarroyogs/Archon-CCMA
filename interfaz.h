#pragma once
#include "ETSIDI.h"

class Interfaz
{
private:
    ETSIDI::Sprite fondo{ "fondo.png", 0, 0, 16, 12 };
public:
    void dibujaFondo();
    void dibujaMenu();
    void dibujaInstrucciones();
    void dibujaTexto(float x, float y, const char* texto);
    void eligeModo();
    void tecladoinicio(unsigned char key, int x, int y);
};

