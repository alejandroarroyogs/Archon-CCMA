#include "freeglut.h"
#include "mundo.h"
#include "tablero.h"
#include <iostream>

Mundo mundo; // Única definición global de mundo limpia

extern Estado estado; // Traemos el estado real del juego

void OnDraw(void);
void OnTimer(int value);

void OnKeyboardDown(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC para salir

    if (estado == COMBATE) mundo.arena.keyDown(key);
    else mundo.tecla(key);
}

void OnKeyboardUp(unsigned char key, int x, int y) {
    if (estado == COMBATE) mundo.arena.keyUp(key);
    else mundo.teclaLiberada(key);
}

void OnSpecialKeyboardDown(int key, int x, int y) {
    if (estado == COMBATE) mundo.arena.specialKeyDown(key);
}

void OnSpecialKeyboardUp(int key, int x, int y) {
    if (estado == COMBATE) mundo.arena.specialKeyUp(key);
}

void OnMouseClick(int button, int state, int x, int y) {
    mundo.interfaz.gestionRaton(button, state, x, y);
}

void OnPassiveMotion(int x, int y)
{
    mundo.interfaz.movimientoRaton(x, y);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 800);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("ARCHON");
    glutFullScreen();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1000 / 800.0f, 0.1, 150);

    glutDisplayFunc(OnDraw);
    glutTimerFunc(25, OnTimer, 0);
    glutKeyboardFunc(OnKeyboardDown);
    glutKeyboardUpFunc(OnKeyboardUp);
    glutSpecialFunc(OnSpecialKeyboardDown);
    glutSpecialUpFunc(OnSpecialKeyboardUp);
    glutMouseFunc(OnMouseClick);
    glutPassiveMotionFunc(OnPassiveMotion);
    mundo.Inicializar();

    glutMainLoop();

    return 0;
}

void OnDraw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    mundo.Dibujar();

    glutSwapBuffers();
}

void OnTimer(int value)
{
    mundo.Timer(value);
    glutTimerFunc(16, OnTimer, 0);
}