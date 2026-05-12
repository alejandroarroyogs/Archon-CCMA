#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class Chewbacca : public Pieza
{
    ModeloOBJ modelo;
public:
    Chewbacca(int b) : Pieza(b, 22, 4), modelo("recursos/chewbacca.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override
    {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);
        return (df <= 1 && dc <= 1 && (df != 0 || dc != 0));
    }

    void Dibujar(float x, float z) override
    {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);

        if (EsAzul()) glColor3f(0.2f, 0.8f, 1.0f);
        else {
            glColor3f(1.0f, 0.2f, 0.2f);
            glRotatef(180.0f, 0, 1, 0);
        }

        glDisable(GL_LIGHTING);
        glScalef(2.0f, 2.0f, 2.0f);
        modelo.dibuja();
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};