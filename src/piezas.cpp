#include "freeglut.h"
#include "piezas.h"
#include <cmath>
#include <iostream>

Pieza::Pieza(int b, int v, int d) : bando(b), vida(v), vidaMax(v), danio(d) {}
Pieza::~Pieza() {}

int Pieza::GetBando() { return bando; }
int Pieza::GetVida() { return vida; }
int Pieza::GetDanio() { return danio; }
void Pieza::SetVida(int v) { vida = v; }
void Pieza::RecibirDanio(int d) {
    vida -= d;
    if (vida < 0) vida = 0;
}
void Pieza::recuperarVida(int cantidad) {
    vida += cantidad;
    if (vida > vidaMax) vida = vidaMax;
}
bool Pieza::EstaViva() { return vida > 0; }
bool Pieza::EsAzul() { return bando == 1; }
bool Pieza::EsRoja() { return bando == 2; }

bool EsAzul(Pieza* pieza) { return (pieza != nullptr) && pieza->EsAzul(); }
bool EsRoja(Pieza* pieza) { return (pieza != nullptr) && pieza->EsRoja(); }

/*

Pieza::Pieza(int b, int v, int d)
{
    bando = b;
    vida = 50;
    vidaMax = 100; // La vida máxima al empezar es la vida completa
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
void Pieza::recuperarVida(int cantidad) {
    vida += cantidad;
    if (vida > vidaMax) vida = vidaMax;
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


//////////
// JEDI // 
//////////

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

    if (EsRoja()) {
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    }

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

     if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f); // Azul claro brillante
    else {
        glColor3f(1.0f, 0.2f, 0.2f); // Rojo brillante
        glRotatef(180.0f, 0, 1, 0);
    }

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

    /*if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f);
    else
        glColor3f(0.2f, 1.0f, 0.2f);*/
/*

    glColor3f(0.2f, 0.5f, 0.2f);

    
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
DarthVader::DarthVader(int b) : Pieza(b, 30, 7), modelo("recursos/darthvader.obj")
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

    glTranslatef(x, 0.0f, z);

    
    glDisable(GL_LIGHTING);

    
    glColor3f(0.05f, 0.05f, 0.05f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    
    glScalef(0.15f, 0.15f, 0.15f);
    modelo.dibuja();

    glEnable(GL_LIGHTING);

    glPopMatrix();
}
void DarthVader::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}
    
//Caballero jedi/sith 

CaballeroJedi::CaballeroJedi(int b) : Pieza(b, 10, 4), modelo("recursos/caballerojedi.obj")
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

    glTranslatef(x, 0.0f, z);


    if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f); // Azul claro brillante
    else {
        glColor3f(1.0f, 0.2f, 0.2f); // Rojo brillante
        glRotatef(180.0f, 0, 1, 0);
    }

    glDisable(GL_LIGHTING);

   

    glScalef(0.005f, 0.005f, 0.005f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);

    glPopMatrix();
}
void CaballeroJedi::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}
/////////////
// TIRADOR //
/////////////

Tirador::Tirador(int b) : Pieza(b, 6, 5), modelo("recursos/tirador.obj")
{
    std::cout << "Intentando cargar tirador.obj..." << std::endl;
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

void Tirador::Dibujar(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.0f, z);

    if (EsRoja()) {
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
   
        glTranslatef(0.6f, 0.0f, 0.2f);
    }
    else {
        
        glTranslatef(0.6f, 0.0f, 0.2f);
    }

    glDisable(GL_LIGHTING);
    if (EsAzul()) glColor3f(0.3f, 0.3f, 1.0f);
    else glColor3f(1.0f, 0.2f, 0.2f);

    glScalef(3.5f, 3.5f, 3.5f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Tirador::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}

//Skywalker/ kylo ren
Skywalker::Skywalker(int b) : Pieza(b, 12, 5), modelo("recursos/skywalker.obj")
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

    glTranslatef(x, 0.0f, z);

    

    glDisable(GL_LIGHTING);

    if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f); // Azul claro brillante
    else {
        glColor3f(1.0f, 0.2f, 0.2f); // Rojo brillante
        glRotatef(180.0f, 0, 1, 0);
    }

    glScalef(0.7f, 0.7f, 0.7f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void Skywalker::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}


//drones

Drone::Drone(int b) : Pieza(b, 4, 2), modelo("recursos/drone.obj")
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

    glTranslatef(x, 0.0f, z);

    if (EsRoja()) {
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    }

    glDisable(GL_LIGHTING);
    if (EsAzul()) glColor3f(0.2f, 0.8f, 1.0f);
    else glColor3f(1.0f, 0.4f, 0.4f);

    glScalef(0.03f, 0.03f, 0.03f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);
    glPopMatrix();

}
void Drone::DibujarCombate(float x, float z, bool flash)
{
    Dibujar(x, z);
}
//Chewbacca/ Jabba the hut

Chewbacca::Chewbacca(int b) : Pieza(b, 22, 4), modelo("recursos/chewbacca.obj")
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

    glTranslatef(x, 0.0f, z);

    
    if (EsAzul())
        glColor3f(0.2f, 0.8f, 1.0f); // Azul claro brillante
    else {
        glColor3f(1.0f, 0.2f, 0.2f); // Rojo brillante
        glRotatef(180.0f, 0, 1, 0);
    }

    glDisable(GL_LIGHTING);

   

    // Escala (ajusta según modelo)
    glScalef(2.0f, 2.0f, 2.0f);

    modelo.dibuja();

    glEnable(GL_LIGHTING);

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
*/