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

        if (df == 0 && dc == 0) return false;
        return (df <= 3 && dc <= 3);
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