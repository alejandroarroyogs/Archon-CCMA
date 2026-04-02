#pragma once
#include "freeglut.h"
#include "codigo/tablero/tablero.h"
#include "codigo/Piezas/piezas.h"
#include "interfaz.h"

class Mundo
{
public:
    Mundo();
    virtual ~Mundo();

    void Inicializar();
    void Dibujar();

    // Estado del juego
    enum Estado { MENU, JUGANDO, GAMEOVER };
    Estado estado;
    Interfaz interfaz;

    void tecla(unsigned char key);
    void DibujarBarraVida(float x, float y, float z, int vidaActual, int vidaMax);
    void Timer(int value);


};

