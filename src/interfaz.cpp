#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

void Interfaz::dibujaTexto(float x, float y, const char* texto)
{ 
    glEnable(GL_TEXTURE_2D);
    //Usaré la fuente Starjedi.ttx
    ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    ETSIDI::setFont("bin/starjedi.ttf", 24);
    ETSIDI::printxy(texto, x, y);
    glDisable(GL_TEXTURE_2D); // Mejor desactivarlo al salir
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

bool Interfaz::clickEnRectangulo(int mouseX, int mouseY, int x, int y, int ancho, int alto)
{
    if (mouseX >= x && mouseX <= (x + ancho) && mouseY >= y && mouseY <= (y + alto)) {
        return true;
    }
    return false;
}

void Interfaz::gestionRaton(int boton, int estado, int x, int y)
{
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        //en el menu principal:
        if (this->estado == MENU) {
            if (clickEnRectangulo(x, y, 300, 250, 200, 50)) {
                this->estado = SELEC_MODO; // Pasamos a la siguiente interfaz
                ETSIDI::play("bin/click.wav");
            }
        }
        else if (clickEnRectangulo(x, y, 300, 350, 200, 50)) {
            this->estado = INSTRUC;
        }
    }
    //seleccion de modo
    else if(this->estado==SELEC_MODO){
        if (clickEnRectangulo(x, y, 100, 300, 250, 100)) {
            this->modoJuego = 1; // 1 jugador
            this->estado = JUGANDO;
        }
        else if (clickEnRectangulo(x, y, 450, 300, 250, 100)) {
            this->modoJuego = 2; // 2 jugadores
            this->estado = JUGANDO;
        }
    }
    //si estamos en instrucciones
    else if (this->estado == INSTRUC) {
        // Un botón de "VOLVER" en la esquina
        if (clickEnRectangulo(x, y, 10, 10, 100, 50)) {
            this->estado = MENU;
        }
    }
}


void Interfaz::dibujaMenu()
{
    dibujaFondo();
    dibujaTexto(0, 5, "ARCHON CONTRATACA");
    ETSIDI::setFont("bin/starjedi.ttf", 18);
    dibujaTexto(0, 0, "JUGAR");
    dibujaTexto(0, -2, "INSTRUCCIONES");
}
void Interfaz::eligeModo()
{
    dibujaFondo();
    dibujaTexto(0, 5, "MODO DE JUEGO");
    ETSIDI::setFont("bin/StarJedi.ttf", 18);
    dibujaTexto(-4, 0, "JEDI");
    dibujaTexto(4, -2, "JEDI vs SITH");

}

void Interfaz::dibujaInstrucciones()
{
    dibujaFondo();
    //Tengo que redactar mejorlas instrucciones
}

