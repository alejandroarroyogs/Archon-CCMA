#include "tablero.h"
#include "freeglut.h"
#include "piezas.h"
#include "mundo.h"      
#include <cmath>

Tablero::Tablero() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            casillas[i][j] = nullptr;
        }
    }
    filaSeleccionada = 4; // Empezamos en el centro del tablero
    colSeleccionada = 4;
    piezaSeleccionada = false;
    filaOrigen = 0;
    colOrigen = 0;
    turnoActual = 1;

    for (int i = 0; i < 256; i++) {
        teclasPulsadas[i] = false;
    }
    cooldownMovimiento = 0;
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
}

void Tablero::inicializa() {
    // 1. Limpieza de seguridad
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            if (casillas[i][j] != nullptr) {
                delete casillas[i][j];
                casillas[i][j] = nullptr;
            }
        }
    }

    // 2. Colocación de piezas (Saliendo como antes)
    // --- EJÉRCITO JEDI / AZUL (Bando 1) ---
    // Fila trasera (Fila 0)
    casillas[0][0] = new Drone(1);
    casillas[0][1] = new CaballeroJedi(1);
    casillas[0][2] = new Tirador(1);
    casillas[0][3] = new Skywalker(1);
    casillas[0][4] = new BabyYoda(1); // Líder en el centro
    casillas[0][5] = new Skywalker(1);
    casillas[0][6] = new Tirador(1);
    casillas[0][7] = new CaballeroJedi(1);
    casillas[0][8] = new Drone(1);

    // Fila delantera (Fila 1)
    for (int j = 0; j < TAM_TABLERO; j++) {
        casillas[1][j] = new Jedi(1);
    }

    // --- EJÉRCITO SITH / ROJO (Bando 2) ---
    // Fila trasera (Fila 8)
    casillas[8][0] = new Drone(2);
    casillas[8][1] = new CaballeroJedi(2);
    casillas[8][2] = new Tirador(2);
    casillas[8][3] = new DarthVader(2);
    casillas[8][4] = new DarthVader(2); // Líder en el centro
    casillas[8][5] = new DarthVader(2);
    casillas[8][6] = new Tirador(2);
    casillas[8][7] = new CaballeroJedi(2);
    casillas[8][8] = new Drone(2);

    // Fila delantera (Fila 7)
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
                glColor3ub(255, 215, 0); // Casillas especiales doradas
            }
            else {
                if ((i + j) % 2 == 0) glColor3ub(60, 60, 60);
                else glColor3ub(180, 180, 180);
            }

            float x = (j - 4) * 2.0f;
            float z = (i - 4) * 2.0f;

            // Dibujo de la baldosa (suelo)
            glBegin(GL_QUADS);
            glVertex3f(x - 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
            glVertex3f(x - 1.0f, 0.0f, z + 1.0f);
            glEnd();

            // CURSOR ROJO ELEVADO
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

            // CASILLA ORIGEN VERDE ELEVADA
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

            // RENDERIZADO DE LA PIEZA
            if (casillas[i][j] != nullptr) {
                glEnable(GL_LIGHTING);
                glPushMatrix();

                glTranslatef(x, 0.1f, z);
                casillas[i][j]->Dibujar(0.0f, 0.0f);

                glPopMatrix();
                glDisable(GL_LIGHTING);
            }
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void Tablero::tecla(unsigned char key) {
    // Registramos la tecla como pulsada para el movimiento WASD
    teclasPulsadas[key] = true;

    // Ejecutamos la actualización de posición inmediatamente al presionar la tecla
    actualizarMovimiento();

    // SELECCIÓN CON ESPACIO
    if (key == ' ') {
        if (!piezaSeleccionada) {
            Pieza* p = casillas[filaSeleccionada][colSeleccionada];
            // Solo seleccionamos si hay pieza y es del bando del turno actual
            if (p != nullptr && p->GetBando() == turnoActual) {
                piezaSeleccionada = true;
                filaOrigen = filaSeleccionada;
                colOrigen = colSeleccionada;
            }
        }
        else {
            Pieza* piezaOrigen = casillas[filaOrigen][colOrigen];
            Pieza* piezaDestino = casillas[filaSeleccionada][colSeleccionada];

            // Caso 1: Casilla de destino vacía -> Mover pieza
            if (piezaDestino == nullptr) {
                if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                    casillas[filaSeleccionada][colSeleccionada] = piezaOrigen;
                    casillas[filaOrigen][colOrigen] = nullptr;
                    piezaSeleccionada = false;
                    turnoActual = (turnoActual == 1) ? 2 : 1; // Cambio de turno
                }
            }
            // Caso 2: Casilla ocupada por enemigo -> Entrar en combate
            else if (piezaDestino->GetBando() != turnoActual) {
                if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                    mundo.arena.inicializa(piezaOrigen, piezaDestino, turnoActual);
                    estado = COMBATE;
                    piezaSeleccionada = false;
                }
            }
            // Caso 3: Pulsar sobre la misma pieza seleccionada -> Cancelar selección
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
    // Si hay cooldown activo, decrementamos y salimos para no mover el cursor a velocidad luz
    if (cooldownMovimiento > 0) {
        cooldownMovimiento--;
        return;
    }

    bool seHaMovido = false;

    // Movimiento con W, A, S, D (soporta mayúsculas y minúsculas)
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
            colSeleccionada++; // En tu diseño 'A' incrementa columna
            seHaMovido = true;
        }
    }
    if (teclasPulsadas['d'] || teclasPulsadas['D']) {
        if (colSeleccionada > 0) {
            colSeleccionada--; // En tu diseño 'D' decrementa columna
            seHaMovido = true;
        }
    }

    if (seHaMovido) {
        cooldownMovimiento = 2; // Cooldown bajo para que se sienta fluido pero controlable
    }
}

int Tablero::getTamTablero() { return TAM_TABLERO; }
Pieza* Tablero::getPiezaEnCursor() { return casillas[filaSeleccionada][colSeleccionada]; }

void Tablero::moverIA() {
    // Tu lógica de IA aquí si la necesitas más adelante
}