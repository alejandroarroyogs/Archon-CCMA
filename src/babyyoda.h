#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class BabyYoda : public Pieza
{
    ModeloOBJ modelo;

public:
    BabyYoda(int b) : Pieza(b, 25, 6), modelo("recursos/babyyoda.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override
    {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);

        // Se mueve hasta 2 casillas (df y dc <= 2) y evita quedarse en el sitio
        if (df <= 2 && dc <= 2 && (df != 0 || dc != 0))
            return true;

        return false;
    }

    void Dibujar(float x, float z) override
    {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);

        glDisable(GL_LIGHTING);

        glColor3f(0.2f, 0.5f, 0.2f);

        glScalef(1.0f, 1.0f, 1.0f);

        modelo.dibuja();

        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override
    {
        Dibujar(x, z);
    }
};