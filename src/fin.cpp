#include "fin.h"
#include "ETSIDI.h"
#include "freeglut.h"


void Fin::escribe(float x, float y, const char* texto)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Ajusta 800x600 a la resolución de tu ventana
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    ETSIDI::setFont("fuentes/jedisf.ttf", 55);
    ETSIDI::printxy(texto, x, y, 0);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void Fin::dibuja()
{
    dibujaFondo();
    if (ganador == 1) {
        escribe(100, 500, "THE FORCE IS STRONG WITH YOU");
        escribe(100, 400, "VICTORIA");
    }
    else {
        escribe(100, 500, "YOU LACK FAITH");
        escribe(100, 400, "GAME OVER");
    }
    escribe(100, 300, "SCORE: 5000");
}

void Fin::dibujaFondo()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/end.png").id);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, 1);
    glTexCoord2f(0, 0); glVertex2f(-1, 1);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void Fin::ponMusica()
{
    if (ganador==1 )  ETSIDI::playMusica("sonidos/hope.mp3", true);
    else  ETSIDI::playMusica("sonidos/marcha.mp3", true);
}

void Fin::stopMusica()
{
    ETSIDI::stopMusica();
}
