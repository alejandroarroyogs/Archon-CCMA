#include "arena.h"
#include "freeglut.h"


void Arena::CargaCombate(Pieza* a, Pieza* d, int fA, int fD)
{
    atacante = a;
    defensor = d;
    flashAtacante = fA;
    flashDefensor = fD;
}

void Arena::dibujaCombate()
{
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glColor3f(0.25f, 0.08f, 0.08f);
    glBegin(GL_QUADS);
    glVertex3f(-8.0f, -1.0f, -4.0f); glVertex3f(8.0f, -1.0f, -4.0f);
    glVertex3f(8.0f, -1.0f, 4.0f); glVertex3f(-8.0f, -1.0f, 4.0f);
    glEnd();

    if (atacante != 0) {
        atacante->DibujarCombate(-3.0f, 0.0f, flashAtacante > 0);
        dibujaBarraVida(-3.0f, 2.4f, 0.0f, atacante->GetVida(), 15);
    }
    if (defensor != 0) {
        defensor->DibujarCombate(3.0f, 0.0f, flashDefensor > 0);
        dibujaBarraVida(3.0f, 2.4f, 0.0f, defensor->GetVida(), 15);
    }
    glutSwapBuffers();
    return;
}
void Arena::tiempoCombate()
{

    if (enCombate && combateIniciado)
    {
        if (tiempoEntreRondas > 0)
        {
            tiempoEntreRondas--;
        }
        else
        {
            if (!combateResuelto)
                EjecutarRondaCombate();
        }
    }

    if (flashAtacante > 0)
        flashAtacante--;

    if (flashDefensor > 0)
        flashDefensor--;

    glutPostRedisplay();
}
void Arena::EjecutarRondaCombate()
{
    if (!enCombate)
        return;

    if (atacante == 0 || defensor == 0)
        return;

    if (combateResuelto)
        return;

    defensor->RecibirDanio(atacante->GetDanio());
    flashDefensor = 8;

    if (!defensor->EstaViva())
    {
        combateResuelto = true;
        ResolverCombate(true);
        return;
    }

    atacante->RecibirDanio(defensor->GetDanio());
    flashAtacante = 8;

    if (!atacante->EstaViva())
    {
        combateResuelto = true;
        ResolverCombate(false);
        return;
    }

    tiempoEntreRondas = 20;
}
void Arena::ResolverCombate()
{
    if (!enCombate)
        return;

    if (ganaAtacante)
    {
        if (tableroPiezas[combateFilaDestino][combateColDestino] != 0)
        {
            delete tableroPiezas[combateFilaDestino][combateColDestino];
            tableroPiezas[combateFilaDestino][combateColDestino] = 0;
        }

        tableroPiezas[combateFilaDestino][combateColDestino] = tableroPiezas[combateFilaOrigen][combateColOrigen];
        tableroPiezas[combateFilaOrigen][combateColOrigen] = 0;
    }
    else
    {
        if (tableroPiezas[combateFilaOrigen][combateColOrigen] != 0)
        {
            delete tableroPiezas[combateFilaOrigen][combateColOrigen];
            tableroPiezas[combateFilaOrigen][combateColOrigen] = 0;
        }
    }

    piezaSeleccionada = false;

    if (turnoActual == 1)
        turnoActual = 2;
    else
        turnoActual = 1;

    enCombate = false;
    atacante = 0;
    defensor = 0;

    combateFilaOrigen = -1;
    combateColOrigen = -1;
    combateFilaDestino = -1;
    combateColDestino = -1;

    tiempoEntreRondas = 0;
    combateResuelto = false;

    combateIniciado = false;

    flashAtacante = 0;
    flashDefensor = 0;
}
void Arena::dibujaBarraVida(float x, float y, float z, int vidaActual, int vidaMax)
{
    float anchoTotal = 4.0f;
    float alto = 0.35f;
    float porcentaje = (vidaMax > 0) ? (float)vidaActual / (float)vidaMax : 0.0f;
    if (porcentaje < 0.0f) porcentaje = 0.0f;
    if (porcentaje > 1.0f) porcentaje = 1.0f;

    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-anchoTotal / 2.0f, 0.0f, 0.0f);
    glVertex3f(anchoTotal / 2.0f, 0.0f, 0.0f);
    glVertex3f(anchoTotal / 2.0f, alto, 0.0f);
    glVertex3f(-anchoTotal / 2.0f, alto, 0.0f);
    glEnd();

    if (porcentaje > 0.5f) glColor3f(0.1f, 0.9f, 0.1f);
    else if (porcentaje > 0.25f) glColor3f(0.95f, 0.75f, 0.1f);
    else glColor3f(0.9f, 0.1f, 0.1f);

    float anchoVida = anchoTotal * porcentaje;
    glBegin(GL_QUADS);
    glVertex3f(-anchoTotal / 2.0f, 0.0f, 0.01f);
    glVertex3f(-anchoTotal / 2.0f + anchoVida, 0.0f, 0.01f);
    glVertex3f(-anchoTotal / 2.0f + anchoVida, alto, 0.01f);
    glVertex3f(-anchoTotal / 2.0f, alto, 0.01f);
    glEnd();
    glPopMatrix();
};
