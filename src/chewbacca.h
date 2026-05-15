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

        glDisable(GL_TEXTURE_2D);  
        glEnable(GL_LIGHTING);     
        glEnable(GL_COLOR_MATERIAL);

        glColor3f(0.4f, 0.2f, 0.05f);

        glScalef(3.0f, 3.0f, 3.0f);
        modelo.dibuja();

        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};