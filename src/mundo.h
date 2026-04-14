#pragma once
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"

// Estado del juego
enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, GAMEOVER, SELEC_MODO };

class Mundo
{
public:
    Mundo();
    virtual ~Mundo();

    void Inicializar();
    void Dibujar();

    
    Estado estado;
    Interfaz interfaz;
    Arena arena;
    

    void tecla(unsigned char key);
    void Timer(int value);

};

