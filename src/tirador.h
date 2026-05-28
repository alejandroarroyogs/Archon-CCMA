#pragma once
#include "piezas.h"
#include "freeglut.h"
#include "modeloobj.h"
#include "ETSIDI.h"  

class Tirador : public Pieza {

    inline static ModeloOBJ modelo{ "recursos/tirador.obj" };

public:

    Tirador(int b) : Pieza(b, 70 , 50 , 0) {}
    bool MovimientoValido(int filaO, int colO, int filaD, int colD) override {
        int df = abs(filaD - filaO);
        int dc = abs(colD - colO);
        if (df == 0 && dc == 0) return false;
        return ((df + dc) <= 4);
    }

    void Dibujar(float x, float z) override {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);

        if (EsRoja()) glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.6f, 0.0f, 0.2f);

        glDisable(GL_LIGHTING);      
        glEnable(GL_TEXTURE_2D);    
        //glBindTexture(GL_TEXTURE_2D,
         //  ETSIDI::getTexture("recursos/tirador.png").id);

        if (EsAzul()) glColor3f(0.6f, 0.6f, 1.0f);
        else glColor3f(1.0f, 0.6f, 0.6f);

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