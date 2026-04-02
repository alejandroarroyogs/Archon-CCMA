#pragma once
#include "freeglut.h"
#include "codigo/tablero/tablero.h"
#include "codigo/Piezas/piezas.h"
#include "interfaz.h"
#include "arena.h"

class Mundo
{
public:
    Mundo();
    virtual ~Mundo();

    void Inicializar();
    void Dibujar();
    void DibujarJuego();
    void tecla(unsigned char key);
    void Timer(int value);

private:
    // Estado del juego
    enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, SELEC_MODO, GAMEOVER };
    Estado estado;
    Interfaz interfaz;
    Arena arena;

    

};

