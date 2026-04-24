#include "arena.h"
#include "freeglut.h"
//Constructor arena
Arena::Arena()
{
    atacante = nullptr;
    defensor = nullptr;

    flashAtacante = 0;
    flashDefensor = 0;

    cargarFondos();
    elegirFondo();
}
//Destructor arena
Arena::~Arena()
{

}

void Arena::cargarFondos()
{

}

void Arena::elegirFondo()
{}

void Arena::ponMusica()
{
    ETSIDI::playMusica("sonidos/marcha.mp3", true);
}

void Arena::dibuja()
{

}

void Arena::dibujaPlataforma()
{
    glColor3f(0.3f, 0.3f, 0.3f);

    glPushMatrix();
    glTranslatef(0, -2, 0);
    glScalef(6, 0.5, 4);
    glutSolidCube(1);
    glPopMatrix();
}


