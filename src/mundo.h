#pragma once
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"

// Estado del juego
enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, GAMEOVER, SELEC_MODO };

class Mundo
{
private:
    Estado estado;
    int modoJuego;

public:
    Mundo();
    virtual ~Mundo();

    void Inicializar();
    void Dibujar();
    Interfaz interfaz;
    

    void tecla(unsigned char key);
    void Timer(int value);

};

