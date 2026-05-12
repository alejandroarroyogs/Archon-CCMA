#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"

class Tirador : public Pieza {
    ModeloOBJ modelo;
public:
    Tirador(int b) : Pieza(b, 6, 5), modelo("recursos/tirador.obj") {}

    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);
        if (df <= 1 && dc <= 1 && (df != 0 || dc != 0)) return true;
        if ((df == 2 && dc == 0) || (df == 0 && dc == 2)) return true;
        return false;
    }

    void Dibujar(float x, float z) override {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        if (EsRoja()) {
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.6f, 0.0f, 0.2f);
        }
        else {
            glTranslatef(0.6f, 0.0f, 0.2f);
        }

        glDisable(GL_LIGHTING);      
        glEnable(GL_TEXTURE_2D);    
        glColor3f(1.0f, 1.0f, 1.0f);  

        glScalef(3.5f, 3.5f, 3.5f);
        modelo.dibuja();

        glDisable(GL_TEXTURE_2D);    
        glEnable(GL_LIGHTING);

        glPopMatrix();
    }

    void DibujarCombate(float x, float z, bool flash) override {
        Dibujar(x, z);
    }
};