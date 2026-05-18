#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class Jedi : public Pieza {
    ModeloOBJ modelo;
public:
    Jedi(int b) : Pieza(b, 5, 2), modelo("recursos/jedi.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);
        if (df == 0 && dc == 0) return false;
        return (abs(df) <= 3 && abs(dc) <= 3);
    }

    void Dibujar(float x, float z) override {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        if (EsRoja()) glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glDisable(GL_LIGHTING);
        if (EsAzul()) glColor3f(0.2f, 0.8f, 1.0f);
        else glColor3f(1.0f, 0.2f, 0.2f);
        glScalef(2.5f, 2.5f, 2.5f);
        modelo.dibuja();
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        if (EsAzul()) glColor3f(0.2f, 0.8f, 1.0f);
        else {
            glColor3f(1.0f, 0.2f, 0.2f);
            glRotatef(180.0f, 0, 1, 0);
        }
        modelo.dibuja();
        glPopMatrix();
    }
};