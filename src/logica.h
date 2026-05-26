#pragma once
#include "tablero.h"

class Tablero;
class Logica
{
private:
    Tablero* tablero;
public:
    Logica(Tablero* t);
    void tecla(unsigned char key);
    void teclaLiberada(unsigned char key);
    void actualizarMovimiento();
    int comprobarVictoria(Tablero& tablero);
};