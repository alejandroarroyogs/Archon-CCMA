#pragma once
#include "piezas.h"
#include "freeglut.h"

class Skywalker : public Pieza
{
    ModeloOBJ modelo;
public:
    Skywalker(int b) : Pieza(b, 12, 5), modelo("recursos/skywalker.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override
    {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);
        return (df <= 2 && dc <= 2 && (df != 0 || dc != 0));
    }

    void Dibujar(float x, float z) override
    {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);

        glDisable(GL_LIGHTING);
        if (EsAzul()) glColor3f(0.2f, 0.8f, 1.0f);
        else {
            glColor3f(1.0f, 0.2f, 0.2f);
            glRotatef(180.0f, 0, 1, 0);
        }

        glScalef(0.7f, 0.7f, 0.7f);
        modelo.dibuja();
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};