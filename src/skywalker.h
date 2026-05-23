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
        if (df == 0 && dc == 0) return false;
        return (df <= 5 && dc <= 5); 
    }

    bool EsVoladora() const override { return true; }

    void Dibujar(float x, float z) override
    {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        if (EsRoja()) glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        if (EsAzul()) glColor3f(0.6f, 0.6f, 1.0f);
        else glColor3f(1.0f, 0.6f, 0.6f);

        glScalef(0.7f, 0.7f, 0.7f);
        modelo.dibuja();

        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};