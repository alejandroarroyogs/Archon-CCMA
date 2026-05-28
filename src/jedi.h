#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"


class Jedi : public Pieza {

    inline static ModeloOBJ modelo{ "recursos/jedi.obj" };

public:
    Jedi(int b) : Pieza(b, 50, 20, 0) {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);
        if (df == 0 && dc == 0) return false;
        return (abs(df) <= 2 && abs(dc) <= 2);
    }

    void Dibujar(float x, float z) override {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        if (EsRoja()) glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        if (EsAzul()) glColor3f(0.6f, 0.6f, 1.0f);
        else glColor3f(1.0f, 0.6f, 0.6f);

        glScalef(2.5f, 2.5f, 2.5f);
        modelo.dibuja();

        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
};