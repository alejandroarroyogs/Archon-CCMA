#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

using ETSIDI::SpriteSequence;


void Interfaz::eligeModo()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/modojuego.png").id);
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

void Interfaz::dibujaFondo()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/deathstar.png").id);
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

void Interfaz::dibujaMenu()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/menu.png").id);
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

void Interfaz::dibujaInstrucciones()
{
    dibujaFondo();
    //reset
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo Star Wars

    // Título
    dibujaTexto(-0.6f, 0.8f, "INSTRUCCIONES DEL JUEGO");

    // Objetivo
    dibujaTexto(-0.9f, 0.55f, "OBJETIVO:");
    dibujaTexto(-0.9f, 0.45f, "Derrota todas las piezas del enemigo.");

    // Tablero
    dibujaTexto(-0.9f, 0.25f, "TABLERO:");
    dibujaTexto(-0.9f, 0.15f, "- Selecciona una pieza y muevela.");
    dibujaTexto(-0.9f, 0.05f, "- Si coincide con una pieza rival, hay combate.");

    // Combate
    dibujaTexto(-0.9f, -0.15f, "COMBATE:");
    dibujaTexto(-0.9f, -0.25f, "- La pieza que sobreviva gana la casilla.");

    // Controles
    dibujaTexto(-0.9f, -0.45f, "CONTROLES:");
    dibujaTexto(-0.9f, -0.55f, "- Flechas: mover cursor.");
    dibujaTexto(-0.9f, -0.65f, "- Enter: seleccionar.");
   


}
void Interfaz::dibujaTexto(float x, float y, const char* texto)
{ 
    glRasterPos2f(x, y);
    while (*texto)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *texto++);
}
