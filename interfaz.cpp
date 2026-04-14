#include "interfaz.h"
#include "freeglut.h"
#include "mundo.h"
#include "ETSIDI.h"
using ETSIDI::SpriteSequence;
Estado estado = MENU;
int modoJuego = 0;

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
}
void Interfaz::tecladoinicio(unsigned char key, int x, int y)
{
    if (estado == SELEC_MODO) {
        switch (key) {
            case'1':
                modoJuego = 1;
                estado = JUGANDO;
                break;
            case '2': 
                modoJuego = 2;
                estado = JUGANDO;
                break;
            case 27: //escape
                estado = MENU;
                break;
        }
    }
    glutPostRedisplay();
}
void Interfaz::eligeModo()
{
    glColor3f(0.0f, 1.0f, 1.0f);

    dibujaTexto(-0.3f, 0.6f, "SELECCION DE MODO");
    dibujaTexto(-0.4f, 0.2f, "1. Jugador vs Jugador");
    dibujaTexto(-0.4f, 0.0f, "2. Jugador vs IA");
    dibujaTexto(-0.4f, -0.2f, "ESC. Volver");
}


void Interfaz::dibujaFondo()
{
    // Si necesitas que el fondo esté siempre centrado:
    fondo.setPos(0, 0);

    // Si el fondo sale muy pequeño, puedes ajustar el tamaño aquí:
    // fondo.setSize(20, 15); 

    fondo.draw(); // Este método es el que pertenece a ETSIDI::Sprite
}

void Interfaz::dibujaMenu()
{
    dibujaFondo();
    // 2. Configuración de texto
    ETSIDI::setTextColor(0, 0, 0);
    ETSIDI::printxy("ARCHON", -0.19, 0.59); // Sombra

    ETSIDI::setTextColor(1, 1, 0); // Amarillo
    ETSIDI::setFont("fuentes/Bitwise.ttf", 24);
    ETSIDI::printxy("ARCHON", -0.2, 0.6);

    // Opciones del menú
    ETSIDI::setTextColor(1, 1, 1);
    ETSIDI::printxy("1. Instrucciones del juego", -0.3, 0.2);
    ETSIDI::printxy("2. Elegir modo de juego", -0.3, 0.0);
    ETSIDI::printxy("ESC. Salir", -0.3, -0.2);
  
}

void Interfaz::dibujaInstrucciones()
{
    // 1. Dibujamos el fondo
    dibujaFondo();

    // 2. Título (Usando ETSIDI para que sea más bonito y consistente)
    ETSIDI::setTextColor(1, 1, 0);
    ETSIDI::printxy("ARCHON - INSTRUCCIONES", -0.2, 0.8);

    // 3. Secciones (Cambiamos a blanco)
    ETSIDI::setTextColor(1, 1, 1);

    // Usamos printxy en lugar de tu dibujaTexto antiguo para que todo 
    // use la misma librería y no haya conflictos de coordenadas.
    ETSIDI::printxy("OBJETIVO:", -0.9, 0.5);
    ETSIDI::printxy("Derrota todas las piezas del enemigo.", -0.9, 0.4);

    ETSIDI::printxy("TABLERO:", -0.9, 0.2);
    ETSIDI::printxy("- Juego por turnos", -0.9, 0.1);
    ETSIDI::printxy("- Selecciona una pieza y muevela", -0.9, 0.0);

    ETSIDI::printxy("COMBATE:", -0.9, -0.2);
    ETSIDI::printxy("- Si dos piezas coinciden, luchan", -0.9, -0.3);
    ETSIDI::printxy("- Gana la que sobreviva", -0.9, -0.4);

    ETSIDI::printxy("CONTROLES:", -0.9, -0.6);
    ETSIDI::printxy("- Flechas: mover", -0.9, -0.7);
    ETSIDI::printxy("- Enter: seleccionar", -0.9, -0.8);
    ETSIDI::printxy("- ESC: volver", -0.9, -0.9);
  
}
