#include "../../freeglut.h"
#include "piezas.h"

bool EsAzul(int pieza)
{
    return pieza == SOLDADO_AZUL || pieza == MAGO_AZUL;
}

bool EsRoja(int pieza)
{
    return pieza == SOLDADO_ROJO || pieza == MAGO_ROJO;
}

void DibujarPieza(float x, float z, float r, float g, float b, int tipo)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);
    glColor3f(r, g, b);

    if (tipo == 1) // soldado
    {
        glutSolidCube(1.2f);
    }
    else if (tipo == 2) // mago
    {
        glutSolidSphere(0.7f, 20, 20);
    }

    glPopMatrix();
}