#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class Chewbacca : public Pieza
{
    ModeloOBJ modelo;
public:
    Chewbacca(int b) : Pieza(b, 200, 20, 15), modelo("recursos/chewbacca.obj") {}

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

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        if (EsAzul()) glColor3f(0.6f, 0.6f, 1.0f);
        else glColor3f(1.0f, 0.6f, 0.6f);

        glScalef(2.0f, 2.0f, 2.0f);
        modelo.dibuja();

        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};