#include "interfaz.h"
#include "freeglut.h"

void Interfaz::dibujaTexto(float x, float y, const char* texto)
{
    //nopodemos romper el tablero 3D de después
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Establecemos un sistema de coordenadas de -1 a 1

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 2. Dibujamos el texto
    glRasterPos2f(x, y);
    while (texto && *texto) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto++);
    }
    // 3. Restauramos las matrices para que el tablero 3D se vea bien
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
};


void Interfaz::dibujaMenu()
{


    // Color verde
    glColor3f(0.0f, 1.0f, 0.0f);

    // Título
    dibujaTexto(-0.2f, 0.6f, "ARCHON");

    // Opciones
    dibujaTexto(-0.3f, 0.2f, "1. Jugar");
    dibujaTexto(-0.3f, 0.0f, "2. Instrucciones");
    dibujaTexto(-0.3f, -0.2f, "ESC. Salir");
    glutSwapBuffers();


}

void Interfaz::dibujaInstrucciones()
{
    glClear(GL_COLOR_BUFFER_BIT);

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

    glFlush();
    glutSwapBuffers();
}
