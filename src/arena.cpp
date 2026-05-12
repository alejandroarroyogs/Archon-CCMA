#include "arena.h"
#include "piezas.h"
#include "tablero.h"
#include "ETSIDI.h"
#include <cmath>
#include"drone.h"
#include "babyyoda.h"
#include "caballerojedi.h"
#include "chewbacca.h"
#include "darthvader.h"
#include "skywalker.h"

Arena::Arena()
{
    atacante = nullptr;
    defensor = nullptr;

    flashAtacante = 0;
    flashDefensor = 0;
   
    if (estrellas.empty()) {
        for (int i = 0; i < 200; i++) {
            Estrella* e = new Estrella;
            e->x = (float)(rand() % 40 - 20);
            e->y = (float)(rand() % 30 - 15);
            e->z = (float)-(rand() % 10);
            e->brillo = (float)(rand() % 100) / 100.0f;
            estrellas.push_back(e);
        }
    }
}

Arena::~Arena() {
    for (auto e : estrellas) {
        delete e;
    }
    estrellas.clear();
}

void Arena::inicializa(Pieza* a, Pieza* b, int turnoInicial)
{
    atacante = a;
    defensor = b;
    turno = turnoInicial;


    // --- DAÑO DE PRUEBA: Empiezan dañados al 50% ---
    if (atacante != nullptr) {
        atacante->SetVida(50);
    }
    if (defensor != nullptr) {
        defensor->SetVida(50);
    }
    
}

void Arena::ponMusica()
{
    ETSIDI::playMusica("sonidos/marcha.mp3", true);
}

void Arena::stopMusica()
{
    ETSIDI::playMusica("sonidos/marcha.mp3", false);
}

void Arena::dibuja()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // Seguridad para pruebas
    if (atacante == nullptr || defensor == nullptr) {
        static Drone dronePruebaJedi(1);
        static Drone dronePruebaSith(2);
        atacante = &dronePruebaJedi;
        defensor = &dronePruebaSith;
    }

    // 1. FONDO (Estrellas)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -15, 15);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    dibujaFondo();
    glEnable(GL_DEPTH_TEST);

    // 2. ESCENA 3D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1000.0 / 800.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 6.0, 18.0, 0.0, -1.5, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    dibujaPlataforma();

    // --- ENFRENTAMIENTO CON OFFSETS INDIVIDUALES ---
    if (atacante != nullptr && defensor != nullptr) {
        float xA = -4.0f; // Posicion Atacante
        float xD = 4.0f;  // Posicion Defensor

        // -----------------------------------------------------------
        // AJUSTA ESTOS VALORES SI NO SE MIRAN DE FRENTE:
        // Si el Atacante mira a la cámara, prueba 90 o -90.
        // Si el Atacante mira atrás, prueba 180.
        float offsetAtacante = 180.0f;
        float offsetDefensor = 0.0f;
        // -----------------------------------------------------------

        // A) ATACANTE (Lado izquierdo)
        glPushMatrix();
        glTranslatef(xA, -3.7f, 0.0f);
        // Calculamos ángulo hacia el defensor
        float angA = atan2(xD - xA, 0.0f) * 180.0f / 3.14159f;
        glRotatef(angA + offsetAtacante, 0.0f, 1.0f, 0.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        atacante->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();

        // B) DEFENSOR (Lado derecho)
        glPushMatrix();
        glTranslatef(xD, -3.7f, 0.0f);
        // Calculamos ángulo hacia el atacante
        float angD = atan2(xA - xD, 0.0f) * 180.0f / 3.14159f;
        glRotatef(angD + offsetDefensor, 0.0f, 1.0f, 0.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        defensor->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();
    }
}void Arena::dibujaPlataforma()
{
    GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glPushMatrix();
    glTranslatef(0, -4, 0);
    glScalef(15, 0.6, 8);
    glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    // 2. BORDE DE NEÓN 
    glDisable(GL_LIGHTING);

    // --- RESTAURAMOS EL COLOR DEL TURNO ---
    if (turno == 1) glColor3f(0.4f, 1.0f, 0.0f); // Verde Jedi
    else glColor3f(1.0f, 0.0f, 0.0f);            // Rojo Sith

    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-7.5, -3.6, 4.0);
    glVertex3f(7.5, -3.6, 4.0);
    glVertex3f(7.5, -3.6, -4.0);
    glVertex3f(-7.5, -3.6, -4.0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void Arena::BarraVida()
{
    if (!atacante || !defensor) return;

    int vidaAtacante = atacante->GetVida();
    int vidaDefensor = defensor->GetVida();

    if (vidaAtacante < 0) vidaAtacante = 0;
    if (vidaAtacante > 100) vidaAtacante = 100;
    if (vidaDefensor < 0) vidaDefensor = 0;
    if (vidaDefensor > 100) vidaDefensor = 100;

    float vidaA = (float)vidaAtacante / 100.0f;
    float vidaD = (float)vidaDefensor / 100.0f;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1.0f, 1.0f, 1.0f);

    // Barra Atacante
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(50, 720, 350, 740);

    if (vidaA > 0.0f) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(50, 720, 50 + (300.0f * vidaA), 740);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(50, 720);
    glVertex2f(350, 720);
    glVertex2f(350, 740);
    glVertex2f(50, 740);
    glEnd();

    // Barra Defensor
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(650, 720, 950, 740);

    if (vidaD > 0.0f) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(650, 720, 650 + (300.0f * vidaD), 740);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(650, 720);
    glVertex2f(950, 720);
    glVertex2f(950, 740);
    glVertex2f(650, 740);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void Arena::dibujaTexto(float x, float y, const char* texto)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Mantenemos el color configurado previamente en dibujoHechizos
    ETSIDI::setFont("fuentes/jedisf.ttf", 24);
    ETSIDI::printxy(texto, x, y);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void Arena::inicializaEstrellas(int cantidad)
{
    for (auto e : estrellas) delete e;
    estrellas.clear();

    for (int i = 0; i < cantidad; i++) {
        Estrella* e = new Estrella();
        e->x = (rand() % 2000 - 1000) / 100.0f;
        e->y = (rand() % 2000 - 1000) / 100.0f;
        e->brillo = (rand() % 100) / 100.0f;
        estrellas.push_back(e);
    }
}

void Arena::dibujaFondo()
{
    glDisable(GL_LIGHTING);
    glPointSize(4.0f);

    glBegin(GL_POINTS);
    for (auto e : estrellas) {
        glColor3f(e->brillo * 0.6f, e->brillo * 0.6f, e->brillo * 1.0f);
        glVertex2f(e->x, e->y);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}