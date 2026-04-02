#include "../../freeglut.h"
#include "piezas.h"

// ====================
// CLASE BASE PIEZA
// ====================

Pieza::Pieza(int b, int v, int d)
{
    bando = b;
    vida = v;
    danio = d;
}

Pieza::~Pieza()
{
}

int Pieza::GetBando()
{
    return bando;
}

int Pieza::GetVida()
{
    return vida;
}

int Pieza::GetDanio()
{
    return danio;
}

void Pieza::SetVida(int v)
{
    vida = v;
}

void Pieza::RecibirDanio(int d)
{
    vida = vida - d;
    if (vida < 0)
        vida = 0;
}

bool Pieza::EstaViva()
{
    return vida > 0;
}

bool Pieza::EsAzul()
{
    return bando == 1;
}

bool Pieza::EsRoja()
{
    return bando == 2;
}

// ====================
// SOLDADO
// ====================

Soldado::Soldado(int b) : Pieza(b, 10, 4)
{
}

bool Soldado::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    if (df == 0 && dc == 0)
        return false;

    int absDf = df < 0 ? -df : df;
    int absDc = dc < 0 ? -dc : dc;

    if (absDf <= 1 && absDc <= 1)
        return true;

    return false;
}

void Soldado::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f);
    else
        glColor3f(1.0f, 0.3f, 0.3f);

    glutSolidCube(1.2f);

    glPopMatrix();
}

void Soldado::DibujarCombate(float x, float z, bool flash)
{
    glPushMatrix();
    glTranslatef(x, 0.8f, z);

    if (flash)
        glColor3f(1.0f, 1.0f, 1.0f);
    else if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f);
    else
        glColor3f(1.0f, 0.3f, 0.3f);

    glutSolidCube(1.2f);
    glPopMatrix();
}

// ====================
// MAGO
// ====================

Mago::Mago(int b) : Pieza(b, 8, 5)
{
}

bool Mago::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    if (df == 0 && dc == 0)
        return false;

    int absDf = df < 0 ? -df : df;
    int absDc = dc < 0 ? -dc : dc;

    if (absDf <= 2 && absDc <= 2)
        return true;

    return false;
}

void Mago::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.0f, 0.4f, 1.0f);
    else
        glColor3f(0.8f, 0.0f, 0.8f);

    glutSolidSphere(0.7f, 20, 20);

    glPopMatrix();
}

void Mago::DibujarCombate(float x, float z, bool flash)
{
    glPushMatrix();
    glTranslatef(x, 0.8f, z);

    if (flash)
        glColor3f(1.0f, 1.0f, 1.0f);
    else if (EsAzul())
        glColor3f(0.0f, 0.4f, 1.0f);
    else
        glColor3f(0.8f, 0.0f, 0.8f);

    glutSolidSphere(0.7f, 20, 20);
    glPopMatrix();
}

// ====================
// ARQUERO
// ====================

Arquero::Arquero(int b) : Pieza(b, 7, 4) {}

bool Arquero::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    int absDf = df < 0 ? -df : df;
    int absDc = dc < 0 ? -dc : dc;

    // Solo recto (tipo torre) hasta 3 casillas
    if ((df == 0 || dc == 0) && (absDf <= 3 && absDc <= 3))
        return true;

    return false;
}

void Arquero::Dibujar(float x, float z)
{
    glPushMatrix();
    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.0f, 1.0f, 0.5f);
    else
        glColor3f(1.0f, 0.5f, 0.0f);

    glutSolidCone(0.7f, 1.5f, 20, 20);

    glPopMatrix();
}

void Arquero::DibujarCombate(float x, float z, bool flash)
{
    glPushMatrix();
    glTranslatef(x, 0.8f, z);

    if (flash)
        glColor3f(1.0f, 1.0f, 1.0f);
    else if (EsAzul())
        glColor3f(0.0f, 1.0f, 0.5f);
    else
        glColor3f(1.0f, 0.5f, 0.0f);

    glutSolidCone(0.7f, 1.5f, 20, 20);
    glPopMatrix();
}

// ====================
// CABALLERO
// ====================

Caballero::Caballero(int b) : Pieza(b, 12, 3) {}

bool Caballero::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    int absDf = df < 0 ? -df : df;
    int absDc = dc < 0 ? -dc : dc;

    // Movimiento en L
    if ((absDf == 2 && absDc == 1) || (absDf == 1 && absDc == 2))
        return true;

    return false;
}

void Caballero::Dibujar(float x, float z)
{
    glPushMatrix();
    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.3f, 0.3f, 1.0f);
    else
        glColor3f(1.0f, 0.3f, 0.3f);

    glutSolidTeapot(0.6f);

    glPopMatrix();
}

void Caballero::DibujarCombate(float x, float z, bool flash)
{
    glPushMatrix();
    glTranslatef(x, 0.8f, z);

    if (flash)
        glColor3f(1.0f, 1.0f, 1.0f);
    else if (EsAzul())
        glColor3f(0.3f, 0.3f, 1.0f);
    else
        glColor3f(1.0f, 0.3f, 0.3f);

    glutSolidTeapot(0.6f);
    glPopMatrix();
}

// ====================
// REY
// ====================

Rey::Rey(int b) : Pieza(b, 15, 4) {}

bool Rey::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    int absDf = df < 0 ? -df : df;
    int absDc = dc < 0 ? -dc : dc;

    if (absDf <= 1 && absDc <= 1)
        return true;

    return false;
}

void Rey::Dibujar(float x, float z)
{
    glPushMatrix();
    glTranslatef(x, 1.0f, z);

    if (EsAzul())
        glColor3f(1.0f, 1.0f, 0.0f);
    else
        glColor3f(1.0f, 0.0f, 0.0f);

    glutSolidOctahedron();

    glPopMatrix();
}

void Rey::DibujarCombate(float x, float z, bool flash)
{
    glPushMatrix();
    glTranslatef(x, 1.0f, z);

    if (flash)
        glColor3f(1.0f, 1.0f, 1.0f);
    else if (EsAzul())
        glColor3f(1.0f, 1.0f, 0.0f);
    else
        glColor3f(1.0f, 0.0f, 0.0f);

    glutSolidOctahedron();
    glPopMatrix();
}

// ====================
// AUXILIARES
// ====================

bool EsAzul(Pieza* pieza)
{
    if (pieza == 0)
        return false;

    return pieza->EsAzul();
}

bool EsRoja(Pieza* pieza)
{
    if (pieza == 0)
        return false;

    return pieza->EsRoja();
}