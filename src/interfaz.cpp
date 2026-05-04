#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

void Interfaz::dibujaTexto(float x, float y, const char* texto, int idBoton)
{ 
    // 1. Guardamos la configuración actual de la cámara
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800); // Forzamos 1000x800 píxeles

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    // Forzamos a OpenGL a olvidar la textura del fondo antes de que ETSIDI intente usar la de la fuente.
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D); // ETSIDI la necesita activa para la fuente

    // Modo de mezcla para evitar bordes raros
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // color si pulsado o no:
    if (ratontexto == idBoton && idBoton != -1) {
        glColor3f(1.0f, 0.4f, 0.0f); // Naranja
        ETSIDI::setTextColor(1.0f, 0.4f, 0.0f);
    }
    else {
        glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
        ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    }

    ETSIDI::setFont("fuentes/jedisf.ttf", 55);
    ETSIDI::printxy(texto, x, y, 0);
 // Mejor desactivarlo al salir
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();


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

void Interfaz::gestionRaton(int boton, int estado, int x, int y)
{
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        int mouseCorregidoY = 800 - y; // ¡Importante usar la Y corregida aquí también!

        if (this->estado == MENU) {
            // JUGAR: Mismas coordenadas que el hover
            if (x >= 410 && x <= 590 && mouseCorregidoY >= 380 && mouseCorregidoY <= 440) {
                this->estado = SELEC_MODO;
                ETSIDI::play("bin/laser.wav");
            }
            // INSTRUCCIONES
            else if (x >= 280 && x <= 720 && mouseCorregidoY >= 220 && mouseCorregidoY <= 270) {
                this->estado = INSTRUC;
            }
        }
    }
}
void Interfaz::movimientoRaton(int x, int y)
{
    //Obtenemos el tamaño real de la ventana actual (sea cual sea)
    float anchoReal = glutGet(GLUT_WINDOW_WIDTH);
    float altoReal = glutGet(GLUT_WINDOW_HEIGHT);

    //cambio de escala  en función del tamaño de ventana que usemos
    float mouseVirtualX = (x / anchoReal) * 1000.0f;
    float mouseVirtualY = 800.0f - ((y / altoReal) * 800.0f);


    // primera linea
    if (mouseVirtualX >= 400 && mouseVirtualX <= 600 && mouseVirtualY >= 370 && mouseVirtualY <= 440)
        ratontexto = 0;

    // segunda linea
    else if (mouseVirtualX >= 270 && mouseVirtualX <= 730 && mouseVirtualY >= 210 && mouseVirtualY <= 280)
        ratontexto = 1;

    else
        ratontexto = -1;

    glutPostRedisplay();
}

void Interfaz::dibujaMenu()
{
    dibujaFondo();

    dibujaTexto(180, 600, "ARCHON CONTRATACA", -1);
    dibujaTexto(410, 380, "JUGAR", 0);
    dibujaTexto(280, 220, "INSTRUCCIONES", 1);
}
void Interfaz::eligeModo()
{
    dibujaFondo();
    dibujaTexto(180, 600, "MODO DE JUEGO", -1);
    ETSIDI::setFont("bin/StarJedi.ttf", 40);
    dibujaTexto(410, 380, "JEDI", 0);
    dibujaTexto(280, 220, "JEDI vs SITH", 1);

}

void Interfaz::dibujaInstrucciones()
{
    dibujaFondo();
    //Tengo que redactar mejorlas instrucciones
}

