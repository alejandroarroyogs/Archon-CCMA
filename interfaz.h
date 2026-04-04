#pragma once
#include "ETSIDI.h"

class Interfaz
{
private:
    unsigned int fondoID;
public:
    void dibujaFondo();
    void dibujaMenu();
    void dibujaInstrucciones();
    void dibujaTexto(float x, float y, const char* texto);
    void eligeModo();
    void tecladoinicio(unsigned char key, int x, int y);
};

