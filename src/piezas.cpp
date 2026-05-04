#include "freeglut.h"
#include "piezas.h"
#include <cmath>

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

// JEDI 

Jedi::Jedi(int b) : Pieza(b, 5, 2), modelo("recursos/jedi.obj")
{
}

bool Jedi::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    if (df == 0 && dc == 0)
        return false;


    if (abs(df) <= 1 && abs(dc) <= 1)
        return true;

    return false;
}
void Jedi::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.0f, z);

    glDisable(GL_LIGHTING);

    if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f); // Azul claro brillante
    else
        glColor3f(1.0f, 0.2f, 0.2f); // Rojo brillante

    glScalef(2.5f, 2.5f, 2.5f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void Jedi::DibujarCombate(float x, float z, bool flash)
{
    glPushMatrix();

    glTranslatef(x, 0.0f, z);

    if (flash)
        glColor3f(1.0f, 1.0f, 1.0f);
    else if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f);
    else
        glColor3f(1.0f, 0.2f, 0.2f);

    modelo.dibuja();

    glPopMatrix();
}


//Baby yoda
BabyYoda::BabyYoda(int b) : Pieza(b, 25, 6), modelo("recursos/babyyoda.obj")
{
}

//2 casillas en cualquier direccion
bool BabyYoda::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = abs(filaD - filaO);
    int dc = abs(colD - colO);


    if (df <= 2 && dc <= 2 && (df != 0 || dc != 0))
        return true;

    return false;
}

void BabyYoda::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.0f, z);

    glDisable(GL_LIGHTING);

   /* if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f);
    else
        glColor3f(0.2f, 1.0f, 0.2f); */

    glColor3f(0.0f, 0.5f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void BabyYoda::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}

//Darth vader
DarthVader::DarthVader(int b) : Pieza(b, 30, 7)
{
}
//Movimiento estilo reina ajedrez
bool DarthVader::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;


    if (df == 0 || dc == 0)
        return true;

    if (abs(df) == abs(dc))
        return true;

    return false;
}

void DarthVader::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);

    glColor3f(0.1f, 0.1f, 0.1f);

    glutSolidCone(0.6, 1.2, 20, 20);

    glPopMatrix();
}
void DarthVader::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}

//Caballero jedi/sith 

CaballeroJedi::CaballeroJedi(int b) : Pieza(b, 10, 4)
{
}
//Se mueve recto en todas las direcciones de dos en dos
bool CaballeroJedi::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;


    if (df == 0 && abs(dc) <= 2 && dc != 0)
        return true;


    if (dc == 0 && abs(df) <= 2 && df != 0)
        return true;

    return false;
}
//Forma toroide
void CaballeroJedi::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.0f, 0.0f, 1.0f);
    else
        glColor3f(1.0f, 0.0f, 0.0f);


    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.2, 0.5, 20, 20);

    glPopMatrix();
}
void CaballeroJedi::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}

//Tirador
Tirador::Tirador(int b) : Pieza(b, 6, 5)
{
}
//Se mueve  casilla en cualquier direccion o dos en linea recta
bool Tirador::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = abs(filaD - filaO);
    int dc = abs(colD - colO);


    if (df <= 1 && dc <= 1 && (df != 0 || dc != 0))
        return true;


    if ((df == 2 && dc == 0) || (df == 0 && dc == 2))
        return true;

    return false;
}

//Mezcla de cubo y cono

void Tirador::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.3f, 0.3f, 1.0f);
    else
        glColor3f(1.0f, 0.3f, 0.3f);


    glutSolidCube(0.6);


    glTranslatef(0, 0.5f, 0);
    glutSolidCone(0.3, 0.8, 20, 20);

    glPopMatrix();
}

void Tirador::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}

//Skywalker/ kylo ren
Skywalker::Skywalker(int b) : Pieza(b, 12, 5)
{
}
//se mueve 2 casillas en cualuiqe direccion
bool Skywalker::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = abs(filaD - filaO);
    int dc = abs(colD - colO);


    if (df <= 2 && dc <= 2 && (df != 0 || dc != 0))
        return true;

    return false;
}

void Skywalker::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);

    if (EsAzul())
        glColor3f(0.5f, 0.5f, 1.0f);
    else
        glColor3f(1.0f, 0.5f, 0.5f);


    glutSolidCube(0.8);


    glTranslatef(0, 0.6f, 0);
    glutSolidSphere(0.3, 20, 20);

    glPopMatrix();
}

void Skywalker::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}


//drones

Drone::Drone(int b) : Pieza(b, 4, 2)
{
}
//· casillas en cualquier direccion
bool Drone::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = filaD - filaO;
    int dc = colD - colO;

    int adf = abs(df);
    int adc = abs(dc);


    if ((df == 0 && adc <= 3 && dc != 0) ||
        (dc == 0 && adf <= 3 && df != 0))
        return true;


    if (adf == adc && adf <= 3)
        return true;

    return false;
}

void Drone::Dibujar(float x, float z)
{
    glPushMatrix();


    glTranslatef(x, 1.2f, z);

    if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f);
    else
        glColor3f(1.0f, 0.4f, 0.4f);


    glutSolidSphere(0.3, 20, 20);


    glColor3f(0.8f, 0.8f, 0.8f);

    glPushMatrix();
    glScalef(1.5f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.1f, 0.1f, 1.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}
void Drone::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}
//Chewbacca/ Jabba the hut

Chewbacca::Chewbacca(int b) : Pieza(b, 22, 4)
{
}

//se mueve 1 casilla en cualquier direccion
bool Chewbacca::MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int df = abs(filaD - filaO);
    int dc = abs(colD - colO);


    if (df <= 1 && dc <= 1 && (df != 0 || dc != 0))
        return true;

    return false;
}
void Chewbacca::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.8f, z);


    if (EsAzul())
        glColor3f(0.6f, 0.4f, 0.2f);
    else
        glColor3f(0.4f, 0.2f, 0.1f);


    glScalef(1.2f, 1.4f, 1.2f);
    glutSolidCube(1.0f);

    glPopMatrix();
}

void Chewbacca::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}





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
   