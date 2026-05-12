#pragma once

class Tablero;
class Logica
{
private:

    Tablero* tablero;

public:

    Logica(Tablero* t);
    void tecla(unsigned char key);
    void teclaEspecial(int key);
    void teclaLiberada(unsigned char key);
    void actualizarMovimiento();
    
};