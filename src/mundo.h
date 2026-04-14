#pragma once
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"

// Estado del juego
enum Estado { MENU, JUGANDO, GAMEOVER, INSTRUC, GAMEOVER, MENU, SELEC_MODO };

class Mundo
{
public:
    Mundo();
    virtual ~Mundo();

    void Inicializar();
    void Dibujar();

    
    Estado estado;
    Interfaz interfaz;

    void tecla(unsigned char key);
    void DibujarBarraVida(float x, float y, float z, int vidaActual, int vidaMax);
    void Timer(int value);

};

