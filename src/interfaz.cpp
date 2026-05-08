#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

void Interfaz::dibujaTexto(float x, float y, const char* texto, int idBoton)
{ 
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

void Interfaz::gestionRaton(int boton, int estadoRaton, int x, int y)
{
    // Solo actuamos si es el botón izquierdo y se acaba de pulsar (DOWN)
    if (boton == GLUT_LEFT_BUTTON && estadoRaton == GLUT_DOWN)
    {
        ETSIDI::play("sonidos/laser.wav");
        // Si ratontexto es -1, es que el clic ha sido fuera de cualquier botón
        if (ratontexto == -1) return;

        // Navegación basada en el estado actual y el ID del botón (ratontexto)
        if (estado == MENU) {
            if (ratontexto == 0) { // Botón JUGAR
                estado = SELEC_MODO;
            }
            else if (ratontexto == 1) { // Botón INSTRUCCIONES
                estado = INSTRUC;
            }
        }
        else if (estado == SELEC_MODO) {
            if (ratontexto == 0) { 
                modoJuego = 1; //un jugador
                mundo.inicializarPartida();
                estado = JUGANDO;
            }
            else if (ratontexto == 1) {
                modoJuego = 2;  //dos jugadores
                mundo.inicializarPartida();
                estado = JUGANDO;
            }
        }
        else if (estado == INSTRUC) {
            if (ratontexto == 2) { // Botón ATRÁS
                estado = MENU;
            }
        }

        // Importante para que el cambio de pantalla sea instantáneo
        glutPostRedisplay();
    }
}
void Interfaz::movimientoRaton(int x, int y)
{
    float anchoReal = glutGet(GLUT_WINDOW_WIDTH);
    float altoReal = glutGet(GLUT_WINDOW_HEIGHT);

    // Coordenadas virtuales 1000x800
    float mvX = (x / anchoReal) * 1000.0f;
    float mvY = 800.0f - ((y / altoReal) * 800.0f);

    if (estado == MENU) {
        if (mvX >= 400 && mvX <= 600 && mvY >= 370 && mvY <= 440) ratontexto = 0;      // JUGAR
        else if (mvX >= 270 && mvX <= 730 && mvY >= 210 && mvY <= 280) ratontexto = 1; // INSTRUCCIONES
        else ratontexto = -1;
    }
    else if (estado == SELEC_MODO) {
        if (mvX >= 420 && mvX <= 580 && mvY >= 370 && mvY <= 440) ratontexto = 0;      // JEDI
        else if (mvX >= 280 && mvX <= 720 && mvY >= 210 && mvY <= 280) ratontexto = 1; // JEDI VS SITH
        else ratontexto = -1;
    }
    else if (estado == INSTRUC) {
        if (mvX >= 740 && mvX <= 950 && mvY >= 80 && mvY <= 170) ratontexto = 2;      // ATRAS
        else ratontexto = -1;
    }

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
    dibujaTexto(265, 600, "MODO DE JUEGO", -1);
    dibujaTexto(430, 380, "JEDI", 0);
    dibujaTexto(285, 220, "JEDI vs SITH", 1);

}

void Interfaz::dibujaInstrucciones()
{
   /* dibujaFondo();

    //Tengo que redactar mejorlas instrucciones
    dibujaTexto(350, 650, "INSTRUCCIONES", -1);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1); // Blanco
    ETSIDI::setTextColor(1, 1, 1);

    ETSIDI::setFont("fuentes/jedisf.ttf", 25);

    // Instrucciones breves
    dibujaTexto(100, 450, "- OBJETIVO: Domina los 5 Puntos de Poder.", -1);
    dibujaTexto(100, 400, "- MOVIMIENTO: Clic para elegir, FLECHAS para mover.", -1);
    dibujaTexto(100, 350, "- COMBATE: FLECHAS para mover, ESPACIO para atacar.", -1);
    dibujaTexto(100, 300, "- HECHIZOS: Usa teclas 1-7 para poderes unicos.", -1);
    
    dibujaTexto(800, 720, "ATRAS", 2);*/
    dibujaFondo();

    // Título (Usamos dibujaTexto porque es un título)
    dibujaTexto(260, 650, "INSTRUCCIONES", -1);

    // Configuramos la vista para el texto plano
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1); // Blanco
    ETSIDI::setTextColor(1, 1, 1);

    // Fuente más pequeña para que quepa la frase entera
    ETSIDI::setFont("fuentes/jedisf.ttf", 25);

    // Dibujamos con printxy directamente para evitar el conflicto de matrices
    ETSIDI::printxy("- OBJETIVO: Domina los 5 Puntos de Poder.", 100, 450);
    ETSIDI::printxy("- MOVIMIENTO: Clic para elegir, FLECHAS para mover.", 100, 400);
    ETSIDI::printxy("- COMBATE: FLECHAS para mover, ESPACIO para atacar.", 100, 350);
    ETSIDI::printxy("- HECHIZOS: Usa teclas 1-7 para poderes unicos.", 100, 300);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // El botón ATRÁS (Abajo a la derecha para que no estorbe)
    dibujaTexto(750, 100, "ATRAS", 2);

}
