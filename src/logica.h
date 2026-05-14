#pragma once

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

};