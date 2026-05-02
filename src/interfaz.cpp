#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

void Interfaz::dibujaTexto(float x, float y, const char* texto)
{ 
    //Usaré la fuente Starjedi.ttx
    ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    ETSIDI::setFont("bin/Starjedi.ttf", 24);
    ETSIDI::printxy(texto, x, y);
   
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
    dibujaFondo();
    dibujaTexto(0, 5, "ARCHON CONTRATACA");
    ETSIDI::setFont("bin/StarJedi.ttf", 18);
    dibujaTexto(0, 0, "JUGAR");
    dibujaTexto(0, -2, "INSTRUCCIONES");
}
void Interfaz::eligeModo()
{
    dibujaFondo();
    dibujaTexto(0, 5, "MODO DE JUEG=");
    ETSIDI::setFont("bin/StarJedi.ttf", 18);
    dibujaTexto(0, 0, "JEDI");
    dibujaTexto(0, -2, "JEDI vs SITH");

}

void Interfaz::dibujaInstrucciones()
{
    dibujaFondo();
    //Tengo que redactar mejorlas instrucciones
}

