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
    glColor3f(0.0f, 1.0f, 1.0f);

    dibujaTexto(-0.3f, 0.6f, "SELECCION DE MODO");
    dibujaTexto(-0.4f, 0.2f, "1. Jugador vs Jugador");
    dibujaTexto(-0.4f, 0.0f, "2. Jugador vs IA");
    dibujaTexto(-0.4f, -0.2f, "ESC. Volver");
}

void Interfaz::tecladoinicio(unsigned char key, int x, int y)
{
   
    
}

void Interfaz::dibujaFondo()
{
    // Si necesitas que el fondo esté siempre centrado:
   // fondo.setPos(0, 0);

    // Si el fondo sale muy pequeño, puedes ajustar el tamaño aquí:
    // fondo.setSize(20, 15); 

   // fondo.draw(); // Este método es el que pertenece a ETSIDI::Sprite
}

void Interfaz::dibujaMenu()
{
   // dibujaFondo();
    //forzar vista 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

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
    //dibujaFondo();
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
