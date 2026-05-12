#define _CRT_SECURE_NO_WARNINGS
#include "tablero.h"
#include "freeglut.h"
#include "piezas.h"
#include "mundo.h"      
#include <cmath>
#include "ETSIDI.h"
#include "jedi.h"
#include "tirador.h"
#include "babyyoda.h"
#include "darthvader.h"
#include "caballerojedi.h"
#include "skywalker.h"
#include "drone.h"
#include "chewbacca.h"

Tablero::Tablero() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            casillas[i][j] = nullptr;
        }
    }
    filaSeleccionada = 4;
    colSeleccionada = 4;
    piezaSeleccionada = false;
    filaOrigen = 0;
    colOrigen = 0;
    turnoActual = 1;

    for (int i = 0; i < 256; i++) {
        teclasPulsadas[i] = false;
    }
    cooldownMovimiento = 0;
    hechizosAzules.push_back(new HechizoHeal());
    hechizosRojos.push_back(new HechizoHeal());
}

Tablero::~Tablero() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            if (casillas[i][j] != nullptr) {
                delete casillas[i][j];
                casillas[i][j] = nullptr;
            }
        }
    }
    for (auto h : hechizosAzules) delete h;
    for (auto h : hechizosRojos) delete h;
}

void Tablero::inicializa() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            if (casillas[i][j] != nullptr) {
                delete casillas[i][j];
                casillas[i][j] = nullptr;
            }
        }
    }

    // --- EJÉRCITO JEDI / AZUL (Bando 1) ---
    casillas[0][0] = new Drone(1);
    casillas[0][1] = new CaballeroJedi(1);
    casillas[0][2] = new Tirador(1);
    casillas[0][3] = new Skywalker(1);
    casillas[0][4] = new BabyYoda(1);
    casillas[0][5] = new Skywalker(1);
    casillas[0][6] = new Tirador(1);
    casillas[0][7] = new CaballeroJedi(1);
    casillas[0][8] = new Drone(1);

    for (int j = 0; j < TAM_TABLERO; j++) {
        casillas[1][j] = new Jedi(1);
    }

    // --- EJÉRCITO SITH / ROJO (Bando 2) ---
    casillas[8][0] = new Drone(2);
    casillas[8][1] = new CaballeroJedi(2);
    casillas[8][2] = new Tirador(2);
    casillas[8][3] = new DarthVader(2);
    casillas[8][4] = new DarthVader(2);
    casillas[8][5] = new DarthVader(2);
    casillas[8][6] = new Tirador(2);
    casillas[8][7] = new CaballeroJedi(2);
    casillas[8][8] = new Drone(2);

    for (int j = 0; j < TAM_TABLERO; j++) {
        casillas[7][j] = new Jedi(2);
    }
}

void Tablero::dibuja() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(55.0, 1000.0 / 800.0, 0.1, 150.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    gluLookAt(
        0.0, 22.0, -28.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat luz_posicion[] = { 0.0f, 30.0f, 0.0f, 1.0f };
    GLfloat luz_difusa[] = { 1.2f, 1.2f, 1.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);

    glDisable(GL_LIGHTING);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            bool esEsquina5x5 = (i == 2 || i == 6) && (j == 2 || j == 6);
            bool esCentroAbsoluto = (i == 4 && j == 4);

            if (esEsquina5x5 || esCentroAbsoluto) {
                glColor3ub(255, 215, 0);
            }
            else {
                if ((i + j) % 2 == 0) glColor3ub(60, 60, 60);
                else glColor3ub(180, 180, 180);
            }

            float x = (j - 4) * 2.0f;
            float z = (i - 4) * 2.0f;

            glBegin(GL_QUADS);
            glVertex3f(x - 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
            glVertex3f(x - 1.0f, 0.0f, z + 1.0f);
            glEnd();

            if (i == filaSeleccionada && j == colSeleccionada) {
                glColor3ub(255, 0, 0);
                glLineWidth(4.0f);
                glBegin(GL_LINE_LOOP);
                glVertex3f(x - 1.0f, 0.15f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.15f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.15f, z + 1.0f);
                glVertex3f(x - 1.0f, 0.15f, z + 1.0f);
                glEnd();
            }

            if (piezaSeleccionada && i == filaOrigen && j == colOrigen) {
                glColor3ub(0, 255, 0);
                glLineWidth(4.0f);
                glBegin(GL_LINE_LOOP);
                glVertex3f(x - 1.0f, 0.15f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.15f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.15f, z + 1.0f);
                glVertex3f(x - 1.0f, 0.15f, z + 1.0f);
                glEnd();
            }

            if (casillas[i][j] != nullptr) {
                glEnable(GL_LIGHTING);
                glPushMatrix();
                glTranslatef(x, 0.1f, z);
                casillas[i][j]->Dibujar(0.0f, 0.0f);
                glPopMatrix();
                dibujaBarraVida(x, z, casillas[i][j]->GetVida(), 100);
                glDisable(GL_LIGHTING);
               
            }
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    dibujaInterfazHechizos();
}

void Tablero::tecla(unsigned char key) {
    teclasPulsadas[key] = true;
    actualizarMovimiento();

    if (key >= '1' && key <= '9') {
        lanzarHechizo(key - '1'); // Esta es la nueva función que creamos en Tablero
    }
    else {
        teclasPulsadas[key] = true;
    }
    if (key == ' ') {
        if (!piezaSeleccionada) {
            Pieza* p = casillas[filaSeleccionada][colSeleccionada];
            if (p != nullptr && p->GetBando() == turnoActual) {
                piezaSeleccionada = true;
                filaOrigen = filaSeleccionada;
                colOrigen = colSeleccionada;
            }
        }
        else {
            Pieza* piezaOrigen = casillas[filaOrigen][colOrigen];
            Pieza* piezaDestino = casillas[filaSeleccionada][colSeleccionada];

            if (piezaDestino == nullptr) {
                if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                    casillas[filaSeleccionada][colSeleccionada] = piezaOrigen;
                    casillas[filaOrigen][colOrigen] = nullptr;
                    piezaSeleccionada = false;
                    turnoActual = (turnoActual == 1) ? 2 : 1;
                }
            }
            // --- ¡AQUÍ ESTÁ LA TRANSFERENCIA DE COMBATE INTEGRADA! ---
            else if (piezaDestino->GetBando() != turnoActual) {
                if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                    // Sincronizamos la arena pasándole los combatientes reales
                    mundo.arena.inicializa(piezaOrigen, piezaDestino, turnoActual);
                    // Cambiamos el estado a COMBATE
                    estado = COMBATE;
                    piezaSeleccionada = false;
                }
            }
            else if (filaSeleccionada == filaOrigen && colSeleccionada == colOrigen) {
                piezaSeleccionada = false;
            }
        }
    }

    glutPostRedisplay();
}

void Tablero::teclaLiberada(unsigned char key) {
    teclasPulsadas[key] = false;
}

void Tablero::actualizarMovimiento() {
    if (cooldownMovimiento > 0) {
        cooldownMovimiento--;
        return;
    }

    bool seHaMovido = false;

    if (teclasPulsadas['w'] || teclasPulsadas['W']) {
        if (filaSeleccionada < TAM_TABLERO - 1) {
            filaSeleccionada++;
            seHaMovido = true;
        }
    }
    if (teclasPulsadas['s'] || teclasPulsadas['S']) {
        if (filaSeleccionada > 0) {
            filaSeleccionada--;
            seHaMovido = true;
        }
    }
    if (teclasPulsadas['a'] || teclasPulsadas['A']) {
        if (colSeleccionada < TAM_TABLERO - 1) {
            colSeleccionada++;
            seHaMovido = true;
        }
    }
    if (teclasPulsadas['d'] || teclasPulsadas['D']) {
        if (colSeleccionada > 0) {
            colSeleccionada--;
            seHaMovido = true;
        }
    }

    if (seHaMovido) {
        cooldownMovimiento = 2;
    }
}

int Tablero::getTamTablero() { return TAM_TABLERO; }
Pieza* Tablero::getPiezaEnCursor() { return casillas[filaSeleccionada][colSeleccionada]; }


void Tablero::lanzarHechizo(int indice) {
    std::vector<Hechizo*>& lista = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
    if (indice < 0 || indice >= (int)lista.size()) return;

    Hechizo* h = lista[indice];
    if (h->estaUsado()) return;

    Pieza* objetivo = getPiezaEnCursor();
    if (objetivo != nullptr && objetivo->GetBando() == turnoActual) {
        // Solo el líder debería poder lanzarlo (puedes añadir esa condición aquí)
        if (h->aplica(mundo, objetivo)) {
            h->setUsado(true);
            ETSIDI::play("sonidos/curar.wav");
        }
    }
}


void Tablero::dibujaInterfazHechizos() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float panelAncho = 230.0f; // Un poco más ancho para albergar letras grandes
    float xInicio = (turnoActual == 1) ? 0 : 1000 - panelAncho;
    float xTexto = xInicio + 15;
    float xBorde = (turnoActual == 1) ? panelAncho : xInicio;

    // Fondo Panel (Más opaco para mejorar contraste)
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(xInicio, 800); glVertex2f(xInicio + panelAncho, 800);
    glVertex2f(xInicio + panelAncho, 0); glVertex2f(xInicio, 0);
    glEnd();

    // Borde Neón
    if (turnoActual == 1) glColor3f(0.4f, 1.0f, 1.0f);
    else glColor3f(1.0f, 0.2f, 0.2f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(xBorde, 800); glVertex2f(xBorde, 0);
    glEnd();

    // INFO OBJETIVO (Texto Blanco)
    Pieza* pBajoCursor = getPiezaEnCursor();
    if (pBajoCursor != nullptr) {
        ETSIDI::setTextColor(1, 1, 1);
        ETSIDI::setFont("fuentes/jedisf.ttf", 20); // Tamaño aumentado
        char info[64];
        sprintf(info, "OBJ: %s", (pBajoCursor->GetBando() == turnoActual) ? "ALIADO" : "ENEMIGO");
        ETSIDI::printxy(info, xTexto, 750);

        sprintf(info, "VIDA: %d%%", pBajoCursor->GetVida());
        ETSIDI::printxy(info, xTexto, 720);
    }

    // TÍTULO PODERES
    ETSIDI::setTextColor(1, 1, 0);
    ETSIDI::setFont("fuentes/jedisf.ttf", 26); // Título grande
    ETSIDI::printxy("PODERES", xTexto, 650);

    // LISTADO DE HECHIZOS
    std::vector<Hechizo*>& lista = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
    for (int i = 0; i < (int)lista.size(); i++) {
        float yPos = 580 - (i * 60); // Más separación para evitar solape de letras grandes

        if (lista[i]->estaUsado()) ETSIDI::setTextColor(0.5, 0.5, 0.5);//amarillo título
        else (turnoActual == 1) ? ETSIDI::setTextColor(0.4, 1.0, 0.0) : ETSIDI::setTextColor(1, 0.4, 0.4); //he cambiado a verde por jedi

        ETSIDI::setFont("fuentes/jedisf.ttf", 18); // Letra de hechizo legible
        char buffer[64];
        sprintf(buffer, "[%d] %s", i + 1, lista[i]->getNombre().c_str());
        ETSIDI::printxy(buffer, xTexto, yPos);

        if (lista[i]->estaUsado()) {
            ETSIDI::setFont("fuentes/jedisf.ttf", 14);
            ETSIDI::printxy("   AGOTADO", xTexto, yPos - 20);
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}


void Tablero::dibujaBarraVida(float x, float z, int vidaActual, int vidaMax) {
    float porcentaje = (float)vidaActual / (float)vidaMax;
    float ancho = 1.6f;
    float alto = 0.4f;
    float yBase = 3.5f;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 1. Fondo negro (Este sí respeta la profundidad del mundo)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(x - ancho / 2, yBase, z);
    glVertex3f(x + ancho / 2, yBase, z);
    glVertex3f(x + ancho / 2, yBase + alto, z);
    glVertex3f(x - ancho / 2, yBase + alto, z);
    glEnd();

    // 2. Relleno de color (Desactivamos profundidad para que se pegue al fondo negro)
    glDisable(GL_DEPTH_TEST);
    if (porcentaje > 0) {
        if (porcentaje > 0.6)      glColor3f(0.0f, 1.0f, 0.0f); // Verde
        else if (porcentaje > 0.3) glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
        else                       glColor3f(1.0f, 0.0f, 0.0f); // Rojo

        glBegin(GL_QUADS);
        glVertex3f(x - ancho / 2, yBase, z);
        glVertex3f(x - ancho / 2 + (ancho * porcentaje), yBase, z);
        glVertex3f(x - ancho / 2 + (ancho * porcentaje), yBase + alto, z);
        glVertex3f(x - ancho / 2, yBase + alto, z);
        glEnd();
    }

    // 3. Borde blanco
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x - ancho / 2, yBase, z);
    glVertex3f(x + ancho / 2, yBase, z);
    glVertex3f(x + ancho / 2, yBase + alto, z);
    glVertex3f(x - ancho / 2, yBase + alto, z);
    glEnd();

    glEnable(GL_DEPTH_TEST); // Reactivamos profundidad para el resto del juego
    glEnable(GL_LIGHTING);
}