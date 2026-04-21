#include "freeglut.h"
#include "mundo.h"
#include "tablero.h"

Mundo mundo; //centralizamos la información en este objeto

// Callbacks
void OnDraw(void);
void OnTimer(int value);

void OnKeyboardDown(unsigned char key, int x, int y) {
    mundo.tecla(key);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 800);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("ARCHON");

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1000 / 800.0f, 0.1, 150);

    

    glutDisplayFunc(OnDraw);
    glutTimerFunc(25, OnTimer, 0);
    glutKeyboardFunc(OnKeyboardDown);

    glutMainLoop();



    return 0;
}

void OnDraw(void)
{
    //Borrado de la pantalla y reseteo de la matriz de transformacion	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   //forzar vista 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    mundo.Dibujar();

    //no borrar esta linea ni poner nada despues
    glutSwapBuffers();
};

void OnTimer(int value)
{
    mundo.Timer(value);
    glutTimerFunc(16, OnTimer, 0);
}