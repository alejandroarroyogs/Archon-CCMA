#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

using ETSIDI::SpriteSequence;

extern int estado;
void Interfaz::dibujaTexto(float x, float y, const char* texto)
{
    glRasterPos2f(x, y);
    while (*texto)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto++);
}

void Interfaz::eligeModo()
{
    dibujaFondo();
    // Vista 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Asegurar estados correctos
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(0.0f, 1.0f, 1.0f);
    dibujaTexto(-0.3f, 0.6f, "SELECCION DE MODO");
    dibujaTexto(-0.4f, 0.2f, "1. Jugador vs Jugador");
    dibujaTexto(-0.4f, 0.0f, "2. Jugador vs IA");
    dibujaTexto(-0.4f, -0.2f, "ESC. Volver");
}

void Interfaz::dibujaFondo()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/estrella.png").id);
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
    dibujaFondo();
    // Vista 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Asegurar estados correctos
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    // Color verde
    glColor3f(0.0f, 1.0f, 0.0f);
    // Título
    dibujaTexto(-0.2f, 0.6f, "ARCHON");
    // Opciones
    dibujaTexto(-0.3f, 0.2f, "1. Jugar");
    dibujaTexto(-0.3f, 0.0f, "2. Instrucciones");
    dibujaTexto(-0.3f, -0.2f, "ESC. Salir");

}

void Interfaz::dibujaInstrucciones()
{
    dibujaFondo();

   //forzar vista 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Título
    dibujaTexto(-0.2f, 0.8f, "ARCHON - INSTRUCCIONES");

    // Secciones
    dibujaTexto(-0.9f, 0.5f, "OBJETIVO:");
    dibujaTexto(-0.9f, 0.4f, "Derrota todas las piezas del enemigo.");

    dibujaTexto(-0.9f, 0.2f, "TABLERO:");
    dibujaTexto(-0.9f, 0.1f, "- Juego por turnos");
    dibujaTexto(-0.9f, 0.0f, "- Selecciona una pieza y muevela");

    dibujaTexto(-0.9f, -0.2f, "COMBATE:");
    dibujaTexto(-0.9f, -0.3f, "- Si dos piezas coinciden, luchan");
    dibujaTexto(-0.9f, -0.4f, "- Gana la que sobreviva");

    dibujaTexto(-0.9f, -0.6f, "CONTROLES:");
    dibujaTexto(-0.9f, -0.7f, "- Flechas: mover");
    dibujaTexto(-0.9f, -0.8f, "- Enter: seleccionar");
    dibujaTexto(-0.9f, -0.9f, "- ESC: volver");

}
