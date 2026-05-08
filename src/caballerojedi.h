#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class CaballeroJedi : public Pieza
{
    ModeloOBJ modelo;
public:
    CaballeroJedi(int b) : Pieza(b, 10, 4), modelo("recursos/caballerojedi.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override
    {
        int df = filaD - filaO;
        int dc = colD - colO;

        // Se mueve recto (horizontal o vertical) hasta 2 casillas
        if (df == 0 && abs(dc) <= 2 && dc != 0) return true;
        if (dc == 0 && abs(df) <= 2 && df != 0) return true;

        return false;
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
        glScalef(0.005f, 0.005f, 0.005f);
        modelo.dibuja();
        glEnable(GL_LIGHTING);

        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};