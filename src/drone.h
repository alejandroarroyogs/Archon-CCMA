#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class Drone : public Pieza
{
    ModeloOBJ modelo;
public:
    Drone(int b) : Pieza(b, 4, 2), modelo("recursos/drone.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override
    {
        int adf = abs(filaD - filaO);
        int adc = abs(colD - colO);

        if (adf == 0 && adc == 0) return false;

        return (adf <= 3 && adc <= 3);
    }

    bool EsVoladora() const override { return true; }

    void Dibujar(float x, float z) override
    {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);

        if (EsRoja()) glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

        glDisable(GL_LIGHTING);
        if (EsAzul()) glColor3f(0.2f, 0.8f, 1.0f);
        else glColor3f(1.0f, 0.4f, 0.4f);

        glScalef(0.03f, 0.03f, 0.03f);
        modelo.dibuja();
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override { Dibujar(x, z); }
}; 
