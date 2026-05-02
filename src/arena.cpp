#include "arena.h"
#include "piezas.h"

//Constructor arena
Arena::Arena()
{
    atacante = nullptr;
    defensor = nullptr;

    flashAtacante = 0;
    flashDefensor = 0;

}
//Destructor arena
Arena::~Arena() {}

void Arena::inicializa(Pieza* a, Pieza* b)
{
    atacante = a; //pieza del mismo color que la casilla
    defensor = b; //otra pieza

}

void Arena::ponMusica()
{
    ETSIDI::playMusica("sonidos/marcha.mp3", true);
}

void Arena::dibuja()
{
    //No recuerdo bien si toca buffer o no, cuando funcione bien
    //ya se verá
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    dibujaPlataforma();

    if (atacante) {
        glPushMatrix();
        glTranslatef(-2.0f, -1.0f, 0.0f);
       // atacante->Dibujar(); se llama a la función que sea de dibujar piezas
        glPopMatrix();
    }
    if (defensor) {
        glPushMatrix();
        glTranslatef(2.0f, 1.0f, 0.0f);
       // defensor->Dibujar(); se llama aquí a la función que dibuje la pieza
        glPopMatrix();
    }

    

}

void Arena::dibujaPlataforma()
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0, -2, 0);
    glScalef(8, 0.5, 5);  // un poco más grande
    glutSolidCube(1);
    glPopMatrix();
}

void Arena::BarraVida()
{
    if (!atacante || !defensor) return;

    float vidaA = (float)atacante->getVida() / atacante->getVidaMax(); 
    // se llama a la vida de uno
    float vidaD = (float)defensor->getVida() / defensor->getVidaMax();
    //se llama a la vida de dos

    //
    glPushMatrix();
    glTranslatef(-3.5f, 2.5f, 0.0f);

    // vida
    glColor3f(1.0f - vidaA, vidaA, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(2 * vidaA, 0, 0);
    glVertex3f(2 * vidaA, 0.3f, 0);
    glVertex3f(0, 0.3f, 0);
    glEnd();

    glPopMatrix();

    //Comprobar funcionamiento cuando estén las piezas hechas.
    glPushMatrix();
    glTranslatef(1.5f, 2.5f, 0.0f);

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(2, 0, 0);
    glVertex3f(2, 0.3f, 0);
    glVertex3f(0, 0.3f, 0);
    glEnd();

    glColor3f(1.0f - vidaD, vidaD, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(2 * vidaD, 0, 0);
    glVertex3f(2 * vidaD, 0.3f, 0);
    glVertex3f(0, 0.3f, 0);
    glEnd();

    glPopMatrix();
}

void Arena::dibujoHechizos()
{
    // Aquí añadiré un display de los hechizos disponibles de la pieza.
    //Me cuesta entender todavía el que pase por turnos

}


