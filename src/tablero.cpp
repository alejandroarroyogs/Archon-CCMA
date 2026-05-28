#define _CRT_SECURE_NO_WARNINGS
#include "tablero.h"
#include "freeglut.h"
#include "piezas.h"
#include "mundo.h"      
#include <cmath>
#include "ETSIDI.h"
#include <iostream>
#include <utility> // Necesario para guardar las casillas libres (pares de coordenadas)
#include <queue>

#include "jedi.h"
#include "tirador.h"
#include "babyyoda.h"
#include "darthvader.h"
#include "caballerojedi.h"
#include "skywalker.h"
#include "drone.h"
#include "chewbacca.h"

// =========================================================================
// VARIABLES ESTÁTICAS PARA EL HECHIZO "REVIVIR" (NIGROMANTE)
// Se declaran aquí para no tener que ensuciar ni modificar tablero.h
// =========================================================================
static bool faseReviveMenu = false;
static int indiceMenuRevive = 0;
static bool faseReviveCasilla = false;
static Pieza* piezaARevivir = nullptr;
static std::vector<Pieza*> cementerioActual;
static std::vector<std::pair<int, int>> casillasLibresRevive;

// =========================================================================

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
    turnoGlobal = 0;

    seleccionandoHechizo = false;
    indiceHechizoSeleccionado = -1;
    bloqueoCuracion = false;

    mensajeErrorHechizo = "";
    timerMensajeError = 0;

    faseTeleportDestino = false;
    filaTeleportOrigen = -1;
    colTeleportOrigen = -1;

    faseExchangeSegunda = false;
    filaExchangeOrigen = -1;
    colExchangeOrigen = -1;

    for (int i = 0; i < 256; i++) {
        teclasPulsadas[i] = false;
    }
    cooldownMovimiento = 0;

    // Reseteamos el estado de Revivir por seguridad al arrancar
    faseReviveMenu = false;
    indiceMenuRevive = 0;
    faseReviveCasilla = false;
    piezaARevivir = nullptr;

    // CARGA DE LOS 7 HECHIZOS DEFINITIVOS (Mapeados a las teclas 1-7)
    // CARGA DE LOS 6 HECHIZOS DEFINITIVOS (Mapeados a las teclas 1-6)
    hechizosAzules.push_back(new HechizoHeal());        // [0] Tecla 1: Curación de la Fuerza
    hechizosAzules.push_back(new HechizoTeleport());    // [1] Tecla 2: Salto Hiperespacial
    hechizosAzules.push_back(new HechizoShiftTime());   // [2] Tecla 3: Alteración Cronológica
    hechizosAzules.push_back(new HechizoExchange());    // [3] Tecla 4: Confusión Mental
    hechizosAzules.push_back(new HechizoImprison());    // [4] Tecla 5: Bloqueo Carbonita
    hechizosAzules.push_back(new HechizoRevive());      // [5] Tecla 6: Resurrección (NUEVO ORDEN)

    hechizosRojos.push_back(new HechizoHeal());
    hechizosRojos.push_back(new HechizoTeleport());
    hechizosRojos.push_back(new HechizoShiftTime());
    hechizosRojos.push_back(new HechizoExchange());
    hechizosRojos.push_back(new HechizoImprison());
    hechizosRojos.push_back(new HechizoRevive());
}

Tablero::~Tablero() {

    for (auto h : hechizosAzules) delete h;
    for (auto h : hechizosRojos) delete h;
}

void Tablero::inicializa() {
    extern Mundo mundo;

    // 1. Limpiamos TODAS las piezas (vivas y muertas) de la partida anterior
    for (Pieza* p : mundo.listaPiezas) {
        delete p;
    }
    mundo.listaPiezas.clear();

    // 2. Vaciamos la matriz lógica del tablero
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            casillas[i][j] = nullptr;
        }
    }

    // 3. Creación de piezas (TU CÓDIGO ORIGINAL)
    casillas[0][0] = new Drone(1);
    casillas[0][1] = new Chewbacca(1);
    casillas[0][2] = new Tirador(1);
    casillas[0][3] = new CaballeroJedi(1);
    casillas[0][4] = new BabyYoda(1);
    casillas[0][5] = new Skywalker(1);
    casillas[0][6] = new Tirador(1);
    casillas[0][7] = new Chewbacca(1);
    casillas[0][8] = new Drone(1);

    for (int j = 0; j < TAM_TABLERO; j++) {
        casillas[1][j] = new Jedi(1);
    }

    casillas[8][0] = new Drone(2);
    casillas[8][1] = new Chewbacca(2);
    casillas[8][2] = new Tirador(2);
    casillas[8][3] = new Skywalker(2);
    casillas[8][4] = new DarthVader(2);
    casillas[8][5] = new CaballeroJedi(2);
    casillas[8][6] = new Tirador(2);
    casillas[8][7] = new Chewbacca(2);
    casillas[8][8] = new Drone(2);

    for (int j = 0; j < TAM_TABLERO; j++) {
        casillas[7][j] = new Jedi(2);
    }

    // 4. ¡LA CLAVE! Registramos todas las piezas creadas en la lista global
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            if (casillas[i][j] != nullptr) {
                mundo.listaPiezas.push_back(casillas[i][j]);
            }
        }
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
    gluLookAt(0.0, 22.0, -28.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat luz_posicion[] = { 0.0f, 30.0f, 0.0f, 1.0f };
    GLfloat luz_difusa[] = { 1.2f, 1.2f, 1.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luz_posicion);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glDisable(GL_LIGHTING);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int tipoMapa[9][9] = {
                {0, 0, 0, 1, 2, 1, 0, 0, 0},
                {0, 0, 1, 0, 1, 0, 1, 0, 0},
                {0, 1, 0, 0, 1, 0, 0, 1, 0},
                {1, 0, 0, 0, 1, 0, 0, 0, 1},
                {2, 1, 1, 1, 2, 1, 1, 1, 2},
                {1, 0, 0, 0, 1, 0, 0, 0, 1},
                {0, 1, 0, 0, 1, 0, 0, 1, 0},
                {0, 0, 1, 0, 1, 0, 1, 0, 0},
                {0, 0, 0, 1, 2, 1, 0, 0, 0}
            };

            bool esCasillaLuz = ((i + j) % 2 == 0);

            if (tipoMapa[i][j] == 2) glColor3ub(255, 215, 0);
            else if (tipoMapa[i][j] == 1) {
                if (turnoActual == 1) glColor3ub(20, 100, 220);
                else glColor3ub(200, 30, 30);
            }
            else {
                if (esCasillaLuz) glColor3ub(200, 200, 200);
                else glColor3ub(50, 50, 50);
            }

            float x = (float)(j - 4) * 2.0f;
            float z = (float)(i - 4) * 2.0f;

            glBegin(GL_QUADS);
            glVertex3f(x - 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
            glVertex3f(x - 1.0f, 0.0f, z + 1.0f);
            glEnd();

            if (casillas[i][j] != nullptr) {
                glEnable(GL_LIGHTING);
                glPushMatrix();
                glTranslatef(x, 0.1f, z);
                casillas[i][j]->Dibujar(0.0f, 0.0f);
                glPopMatrix();

                if (casillas[i][j]->estaEncarcelada) {
                    glPushMatrix();
                    glTranslatef(x, 1.2f, z);
                    glDisable(GL_LIGHTING); glEnable(GL_BLEND); glDepthMask(GL_FALSE);
                    glColor4f(0.0f, 0.5f, 1.0f, 0.35f);
                    glPushMatrix(); glScalef(1.8f, 2.4f, 1.8f); glutSolidCube(1.0); glPopMatrix();
                    glColor4f(0.0f, 0.8f, 1.0f, 0.9f); glLineWidth(2.0f);
                    glPushMatrix(); glScalef(1.8f, 2.4f, 1.8f); glutWireCube(1.0); glPopMatrix();
                    glLineWidth(1.0f); glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
                    glPopMatrix();
                }

                dibujaBarraVida(x, z, casillas[i][j]->GetVida(), casillas[i][j]->GetVidaMax());
                dibujaMovimientoValido();
                glDisable(GL_LIGHTING);
            }
        }
        
    }

    // === DIBUJO DEL CURSOR DE SELECCIÓN ===
    glPushMatrix();
    float posX = (float)(colSeleccionada - 4) * 2.0f;
    float posZ = (float)(filaSeleccionada - 4) * 2.0f;
    glTranslatef(posX, 0.16f, posZ);
    glDisable(GL_LIGHTING);

    if (seleccionandoHechizo) {
        float t = (float)glutGet(GLUT_ELAPSED_TIME) / 150.0f;
        float intensidad = 0.5f + 0.5f * sinf(t);
        if (faseTeleportDestino || faseExchangeSegunda || faseReviveCasilla) {
            glColor3f(0.0f * intensidad, 0.8f * intensidad, 1.0f); // Azul Celeste para elegir casilla
        }
        else {
            glColor3f(1.0f * intensidad, 0.85f * intensidad, 0.0f); // Amarillo dorado
        }
        glLineWidth(4.0f);
    }
    else if (piezaSeleccionada) { glColor3f(0.0f, 1.0f, 0.0f); glLineWidth(3.0f); }
    else { glColor3f(1.0f, 0.0f, 0.0f); glLineWidth(3.0f); }

    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.0f, 0.0f, -1.0f); glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
    glEnd();
    glLineWidth(1.0f); glEnable(GL_LIGHTING); glPopMatrix();

    // Dibujo del origen regular (Verde)
    if (piezaSeleccionada) {
        glPushMatrix();
        glTranslatef((float)(colOrigen - 4) * 2.0f, 0.16f, (float)(filaOrigen - 4) * 2.0f);
        glDisable(GL_LIGHTING); glColor3ub(0, 255, 0); glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-1.0f, 0.0f, -1.0f); glVertex3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
        glEnd();
        glLineWidth(1.0f); glEnable(GL_LIGHTING); glPopMatrix();
    }

    // Dibujo de marca de dos fases (Teleport o Exchange)
    if (seleccionandoHechizo && (faseTeleportDestino || faseExchangeSegunda)) {
        glPushMatrix();
        float oX = faseTeleportDestino ? (float)(colTeleportOrigen - 4) * 2.0f : (float)(colExchangeOrigen - 4) * 2.0f;
        float oZ = faseTeleportDestino ? (float)(filaTeleportOrigen - 4) * 2.0f : (float)(filaExchangeOrigen - 4) * 2.0f;
        glTranslatef(oX, 0.16f, oZ);
        glDisable(GL_LIGHTING); glColor3ub(255, 215, 0); glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-1.0f, 0.0f, -1.0f); glVertex3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
        glEnd();
        glLineWidth(1.0f); glEnable(GL_LIGHTING); glPopMatrix();
    }

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glEnable(GL_LIGHTING);

    dibujaInterfazHechizos();
}

void Tablero::tecla(unsigned char key) {
    teclasPulsadas[key] = true;
    actualizarMovimiento();

    if (seleccionandoHechizo) {
        if (key == 13 || key == ' ') {
            teclasPulsadas[key] = false;
            confirmarObjetivoHechizo();
            return;
        }
        if (key == 8 || key == 'q' || key == 'Q') {
            teclasPulsadas[key] = false;
            cancelarSeleccionHechizo();
            return;
        }
    }

    if (!seleccionandoHechizo) {
        // PERMITIMOS TECLAS DEL 1 AL 7 PARA ACTIVAR LOS PODERES
        if (key >= '1' && key <= '6') {
            int indiceHechizo = key - '1';
            lanzarHechizo(indiceHechizo);
            return;
        }

        if (key == ' ') {
            if (!piezaSeleccionada) {
                Pieza* p = casillas[filaSeleccionada][colSeleccionada];
                if (p != nullptr && p->GetBando() == turnoActual) {
                    if (p->estaEncarcelada) {
                        mensajeErrorHechizo = "UNIDAD CONGELADA EN CARBONITA";
                        timerMensajeError = 120;
                        return;
                    }
                    piezaSeleccionada = true;
                    filaOrigen = filaSeleccionada; colOrigen = colSeleccionada;
                }
            }
            else {
                Pieza* piezaOrigen = casillas[filaOrigen][colOrigen];
                Pieza* piezaDestino = casillas[filaSeleccionada][colSeleccionada];

                if (piezaDestino == nullptr) {
                    if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                        if (CaminoLibre(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                            casillas[filaSeleccionada][colSeleccionada] = piezaOrigen;
                            casillas[filaOrigen][colOrigen] = nullptr;
                            piezaSeleccionada = false;
                            avanzarTurno();
                        }
                        else {
                            mensajeErrorHechizo = "CAMINO BLOQUEADO POR OTRA PIEZA";
                            timerMensajeError = 120;
                        }
                    }
                    else {
                        mensajeErrorHechizo = "MOVIMIENTO FUERA DE RANGO";
                        timerMensajeError = 120;
                    }
                }
                else if (piezaDestino->GetBando() != turnoActual) {
                    if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                        if (CaminoLibre(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                            mundo.arena.inicializa(piezaOrigen, piezaDestino, turnoActual);
                            estado = COMBATE;
                            piezaSeleccionada = false;
                        }
                        else {
                            mensajeErrorHechizo = "CAMINO BLOQUEADO POR OTRA PIEZA";
                            timerMensajeError = 120;
                        }
                    }
                    else {
                        mensajeErrorHechizo = "OBJETIVO FUERA DE RANGO";
                        timerMensajeError = 120;
                    }
                }
                else if (filaSeleccionada == filaOrigen && colSeleccionada == colOrigen) {
                    piezaSeleccionada = false;
                }
            }
        }
    }
    glutPostRedisplay();
}

void Tablero::teclaLiberada(unsigned char key) {
    teclasPulsadas[key] = false;
}

void Tablero::actualizarMovimiento() {
    if (cooldownMovimiento > 0) { cooldownMovimiento--; return; }
    bool seHaMovido = false;

    // Si estamos en la fase de elegir del CEMENTERIO (Menú Lateral), el movimiento es vertical
    if (faseReviveMenu) {
        if (teclasPulsadas['w'] || teclasPulsadas['W']) {
            if (indiceMenuRevive > 0) { indiceMenuRevive--; seHaMovido = true; }
        }
        if (teclasPulsadas['s'] || teclasPulsadas['S']) {
            if (indiceMenuRevive < (int)cementerioActual.size() - 1) { indiceMenuRevive++; seHaMovido = true; }
        }
    }
    // Si no, movemos el cursor 3D del tablero normalmente
    else {
        if (teclasPulsadas['w'] || teclasPulsadas['W']) { if (filaSeleccionada < TAM_TABLERO - 1) { filaSeleccionada++; seHaMovido = true; } }
        if (teclasPulsadas['s'] || teclasPulsadas['S']) { if (filaSeleccionada > 0) { filaSeleccionada--; seHaMovido = true; } }
        if (teclasPulsadas['a'] || teclasPulsadas['A']) { if (colSeleccionada < TAM_TABLERO - 1) { colSeleccionada++; seHaMovido = true; } }
        if (teclasPulsadas['d'] || teclasPulsadas['D']) { if (colSeleccionada > 0) { colSeleccionada--; seHaMovido = true; } }
    }

    if (seHaMovido) cooldownMovimiento = 2;
}

Pieza* Tablero::getPiezaEnCursor() { return casillas[filaSeleccionada][colSeleccionada]; }

bool Tablero::liderEstaVivo(int bando) {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            Pieza* p = casillas[i][j];
            if (p != nullptr && p->GetBando() == bando) {
                if (bando == 1 && dynamic_cast<BabyYoda*>(p) != nullptr) return true;
                if (bando == 2 && dynamic_cast<DarthVader*>(p) != nullptr) return true;
            }
        }
    }
    return false;
}

void Tablero::lanzarHechizo(int indice) {
    Pieza* p = casillas[filaSeleccionada][colSeleccionada];
    bool esLanzadorValido = false;

    // Todos los hechizos se lanzan situados sobre el Hechicero
    if (p != nullptr && p->GetBando() == turnoActual) {
        if (turnoActual == 1 && dynamic_cast<BabyYoda*>(p) != nullptr) esLanzadorValido = true;
        else if (turnoActual == 2 && dynamic_cast<DarthVader*>(p) != nullptr) esLanzadorValido = true;
    }

    if (!esLanzadorValido) {
        mensajeErrorHechizo = "SOLO EL HECHICERO PUEDE LANZAR HECHIZOS";
        timerMensajeError = 120; return;
    }

    std::vector<Hechizo*>& mazo = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
    if (indice < 0 || indice >= (int)mazo.size()) return;

    Hechizo* hechizo = mazo[indice];
    if (hechizo->estaUsado()) {
        mensajeErrorHechizo = "ESTE PODER YA FUE AGOTADO";
        timerMensajeError = 100; return;
    }

    // === LÓGICA ESPECIAL PARA REVIVIR (ÍNDICE 5) ===
    if (indice == 5) {
        int fHechicero = filaSeleccionada;
        int cHechicero = colSeleccionada;

        // Comprobar huecos libres a su alrededor (N, S, E, O)
        casillasLibresRevive.clear();
        int df[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };
        for (int k = 0; k < 4; k++) {
            int nf = fHechicero + df[k];
            int nc = cHechicero + dc[k];
            if (nf >= 0 && nf < TAM_TABLERO && nc >= 0 && nc < TAM_TABLERO && casillas[nf][nc] == nullptr) {
                casillasLibresRevive.push_back({ nf, nc });
            }
        }

        if (casillasLibresRevive.empty()) {
            mensajeErrorHechizo = "NO HAY ESPACIO ADYACENTE AL HECHICERO";
            timerMensajeError = 120; return;
        }

        // Cargar el cementerio con las tropas aliadas muertas
        cementerioActual.clear();
        for (Pieza* pz : mundo.listaPiezas) {
            if (pz->GetBando() == turnoActual && !pz->EstaViva()) {
                cementerioActual.push_back(pz);
            }
        }

        if (cementerioActual.empty()) {
            mensajeErrorHechizo = "CEMENTERIO VACIO";
            timerMensajeError = 120; return;
        }

        // Iniciamos el modo Nigromante
        seleccionandoHechizo = true;
        indiceHechizoSeleccionado = indice;
        faseReviveMenu = true;
        indiceMenuRevive = 0;
        faseReviveCasilla = false;
        piezaARevivir = nullptr;
        return;
    }

    // Flujo normal para los demás hechizos
    seleccionandoHechizo = true;
    indiceHechizoSeleccionado = indice;
    faseTeleportDestino = false;
    faseExchangeSegunda = false;
}

void Tablero::confirmarObjetivoHechizo() {
    if (!seleccionandoHechizo || indiceHechizoSeleccionado == -1) return;

    std::vector<Hechizo*>& mazo = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
    Hechizo* hechizo = mazo[indiceHechizoSeleccionado];
    Pieza* objetivo = casillas[filaSeleccionada][colSeleccionada];

    // Salto Hiperespacial (Índice 1)
    if (indiceHechizoSeleccionado == 1) {
        if (!faseTeleportDestino) {
            if (objetivo == nullptr || objetivo->GetBando() != turnoActual) {
                mensajeErrorHechizo = "SELECCIONA UNA PIEZA ALIADA"; timerMensajeError = 100; return;
            }
            filaTeleportOrigen = filaSeleccionada; colTeleportOrigen = colSeleccionada;
            faseTeleportDestino = true; return;
        }
        else {
            if (objetivo != nullptr) {
                mensajeErrorHechizo = "LA CASILLA DEBE ESTAR VACIA"; timerMensajeError = 100; return;
            }
            if (hechizo->aplica(mundo, nullptr)) {
                casillas[filaSeleccionada][colSeleccionada] = casillas[filaTeleportOrigen][colTeleportOrigen];
                casillas[filaTeleportOrigen][colTeleportOrigen] = nullptr;
                hechizo->setUsado(true); seleccionandoHechizo = false;
                indiceHechizoSeleccionado = -1; faseTeleportDestino = false;
                actualizarTurnosPrision(); turnoActual = (turnoActual == 1) ? 2 : 1;
            }
        }
    }
    // Confusión Mental (Índice 3)
    else if (indiceHechizoSeleccionado == 3) {
        if (!faseExchangeSegunda) {
            if (objetivo == nullptr) {
                mensajeErrorHechizo = "SELECCIONA UNA PIEZA VALIDA"; timerMensajeError = 100; return;
            }
            filaExchangeOrigen = filaSeleccionada; colExchangeOrigen = colSeleccionada;
            faseExchangeSegunda = true;
            hechizo->aplica(mundo, objetivo); return;
        }
        else {
            if (objetivo == nullptr) {
                mensajeErrorHechizo = "SELECCIONA LA SEGUNDA PIEZA"; timerMensajeError = 100; return;
            }
            if (filaSeleccionada == filaExchangeOrigen && colSeleccionada == colExchangeOrigen) {
                mensajeErrorHechizo = "NO PUEDE SER LA MISMA PIEZA"; timerMensajeError = 100; return;
            }
            if (hechizo->aplica(mundo, objetivo)) {
                hechizo->setUsado(true); seleccionandoHechizo = false;
                indiceHechizoSeleccionado = -1; faseExchangeSegunda = false;
                avanzarTurno();
            }
        }
    }
    // === RESOLUCIÓN DE RESURRECCIÓN (AHORA Índice 5) ===
    else if (indiceHechizoSeleccionado == 5) {
        if (faseReviveMenu) {
            piezaARevivir = cementerioActual[indiceMenuRevive];

            if (casillasLibresRevive.size() == 1) {
                piezaARevivir->SetVida(piezaARevivir->GetVidaMax());
                casillas[casillasLibresRevive[0].first][casillasLibresRevive[0].second] = piezaARevivir;

                hechizo->setUsado(true);
                cancelarSeleccionHechizo();
                avanzarTurno();
            }
            else {
                faseReviveMenu = false;
                faseReviveCasilla = true;
                filaSeleccionada = casillasLibresRevive[0].first;
                colSeleccionada = casillasLibresRevive[0].second;
            }
            return;
        }
        else if (faseReviveCasilla) {
            bool casillaAdyacenteValida = false;
            for (auto& par : casillasLibresRevive) {
                if (par.first == filaSeleccionada && par.second == colSeleccionada) {
                    casillaAdyacenteValida = true; break;
                }
            }

            if (!casillaAdyacenteValida) {
                mensajeErrorHechizo = "SELECCIONA UN HUECO ADYACENTE LUMINOSO";
                timerMensajeError = 120; return;
            }

            piezaARevivir->SetVida(piezaARevivir->GetVidaMax());
            casillas[filaSeleccionada][colSeleccionada] = piezaARevivir;

            hechizo->setUsado(true); // Se marca como usado automáticamente el hechizo actual
            cancelarSeleccionHechizo();
            avanzarTurno();
            return;
        }
    }
    // Directos (Heal, Imprison, ShiftTime, etc)
    else {
        if (hechizo->aplica(mundo, objetivo)) {
            hechizo->setUsado(true); seleccionandoHechizo = false;
            indiceHechizoSeleccionado = -1;
            avanzarTurno();
        }
    }
}

void Tablero::cancelarSeleccionHechizo() {
    if (seleccionandoHechizo) {
        seleccionandoHechizo = false; indiceHechizoSeleccionado = -1;
        faseTeleportDestino = false; filaTeleportOrigen = -1; colTeleportOrigen = -1;
        faseExchangeSegunda = false; filaExchangeOrigen = -1; colExchangeOrigen = -1;

        // Limpiamos la magia de Nigromante
        faseReviveMenu = false;
        faseReviveCasilla = false;
        indiceMenuRevive = 0;
        piezaARevivir = nullptr;
    }
}

void Tablero::actualizarTurnosPrision() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            if (casillas[i][j] != nullptr) casillas[i][j]->reducirCondena();
        }
    }
}

void Tablero::dibujaInterfazHechizos() {
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 1000, 0, 800);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    float panelAncho = 230.0f;
    float xInicio = (turnoActual == 1) ? 0.0f : 1000.0f - panelAncho;
    float xTexto = xInicio + 15.0f;
    float xBorde = (turnoActual == 1) ? panelAncho : xInicio;

    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(xInicio, 800.0f); glVertex2f(xInicio + panelAncho, 800.0f);
    glVertex2f(xInicio + panelAncho, 0.0f); glVertex2f(xInicio, 0.0f);
    glEnd();

    if (turnoActual == 1) glColor3f(0.4f, 1.0f, 1.0f); else glColor3f(1.0f, 0.2f, 0.2f);
    glLineWidth(4.0f); glBegin(GL_LINES); glVertex2f(xBorde, 800.0f); glVertex2f(xBorde, 0.0f); glEnd();

    if (seleccionandoHechizo && faseTeleportDestino) {
        ETSIDI::setTextColor(0.0f, 0.8f, 1.0f); ETSIDI::setFont("fuentes/jedisf.ttf", 20);
        ETSIDI::printxy("DESTINO", (int)xTexto, 750);
        ETSIDI::setTextColor(1.0f, 1.0f, 1.0f); ETSIDI::setFont("fuentes/jedisf.ttf", 16);
        ETSIDI::printxy("ELIGE CASILLA VACIA", (int)xTexto, 720);
    }
    else if (seleccionandoHechizo && faseExchangeSegunda) {
        ETSIDI::setTextColor(0.0f, 0.8f, 1.0f); ETSIDI::setFont("fuentes/jedisf.ttf", 19);
        ETSIDI::printxy("FASE: INTERCAMBIO", (int)xTexto, 750);
        ETSIDI::setTextColor(1.0f, 1.0f, 1.0f); ETSIDI::setFont("fuentes/jedisf.ttf", 16);
        ETSIDI::printxy("ELIGE SEGUNDA PIEZA", (int)xTexto, 720);
    }
    else {
        Pieza* pBajoCursor = getPiezaEnCursor();
        if (pBajoCursor != nullptr) {
            ETSIDI::setTextColor(1.0f, 1.0f, 1.0f); ETSIDI::setFont("fuentes/jedisf.ttf", 20);
            char info[64];

            sprintf(info, "OBJ: %s", (pBajoCursor->GetBando() == turnoActual) ? "ALIADO" : "ENEMIGO");
            ETSIDI::printxy(info, (int)xTexto, 765);

            int porcentajeReal = (pBajoCursor->GetVida() * 100) / pBajoCursor->GetVidaMax();
            sprintf(info, "VIDA: %d%%", porcentajeReal);
            ETSIDI::printxy(info, (int)xTexto, 740);

            sprintf(info, "(%d / %d)", pBajoCursor->GetVida(), pBajoCursor->GetVidaMax());
            ETSIDI::setTextColor(0.7f, 0.7f, 0.7f);
            ETSIDI::setFont("fuentes/jedisf.ttf", 16);
            ETSIDI::printxy(info, (int)xTexto, 720);

            ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
            ETSIDI::setFont("fuentes/jedisf.ttf", 20);

            const char* tipoTexto = "TERRESTRE";
            int rangoMov = 3;

            if (dynamic_cast<BabyYoda*>(pBajoCursor) != nullptr || dynamic_cast<DarthVader*>(pBajoCursor) != nullptr) {
                tipoTexto = "HECHICERO"; rangoMov = 3;
            }
            else if (dynamic_cast<Skywalker*>(pBajoCursor) != nullptr) {
                tipoTexto = "VOLADORA"; rangoMov = 5;
            }
            else if (dynamic_cast<CaballeroJedi*>(pBajoCursor) != nullptr || dynamic_cast<Tirador*>(pBajoCursor) != nullptr) {
                tipoTexto = (dynamic_cast<CaballeroJedi*>(pBajoCursor) != nullptr) ? "VOLADORA" : "TERRESTRE";
                rangoMov = 4;
            }
            else if (dynamic_cast<Drone*>(pBajoCursor) != nullptr) {
                tipoTexto = "VOLADORA"; rangoMov = 3;
            }
            else {
                tipoTexto = "TERRESTRE"; rangoMov = 3;
            }

            sprintf(info, "TIPO: %s", tipoTexto);
            ETSIDI::printxy(info, (int)xTexto, 695);

            sprintf(info, "RANGO: %d", rangoMov);
            ETSIDI::printxy(info, (int)xTexto, 675);
        }

        ETSIDI::setTextColor(1.0f, 1.0f, 0.0f); ETSIDI::setFont("fuentes/jedisf.ttf", 26);
        ETSIDI::printxy("PODERES", (int)xTexto, 645);

        std::vector<Hechizo*>& lista = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
        for (int i = 0; i < (int)lista.size(); i++) {
            float yPos = 575.0f - ((float)i * 60.0f);
            if (lista[i]->estaUsado()) ETSIDI::setTextColor(0.5f, 0.5f, 0.5f);
            else (turnoActual == 1) ? ETSIDI::setTextColor(0.4f, 1.0f, 0.0f) : ETSIDI::setTextColor(1.0f, 0.4f, 0.4f);

            ETSIDI::setFont("fuentes/jedisf.ttf", 18);
            char buffer[64]; sprintf(buffer, "[%d] %s", i + 1, lista[i]->getNombre().c_str());
            ETSIDI::printxy(buffer, (int)xTexto, (int)yPos);
            if (lista[i]->estaUsado()) {
                ETSIDI::setFont("fuentes/jedisf.ttf", 14); ETSIDI::printxy("   AGOTADO", (int)xTexto, (int)(yPos - 20.0f));
            }
        }

        // ====================================================================================
        // === DIBUJADO DE LA FASE REVIVIR (MENÚ LATERAL DEL CEMENTERIO Y CUADRADOS VERDES) ===
        // ====================================================================================
        if (seleccionandoHechizo && indiceHechizoSeleccionado == 6) {

            // Fase de Listado: Dibujamos en el lado opuesto de la pantalla
            if (faseReviveMenu) {
                float xCem = (turnoActual == 1) ? 720.0f : 50.0f; // Abre a la derecha si eres luz, a la izq si eres oscuridad

                // Fondo oscuro transparente para el menú lateral
                glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
                glDisable(GL_TEXTURE_2D);
                glBegin(GL_QUADS);
                glVertex2f(xCem - 50.0f, 700.0f); glVertex2f(xCem + 230.0f, 700.0f);
                glVertex2f(xCem + 230.0f, 100.0f); glVertex2f(xCem - 50.0f, 100.0f);
                glEnd();
                glEnable(GL_TEXTURE_2D);

                ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
                ETSIDI::setFont("fuentes/jedisf.ttf", 26);
                ETSIDI::printxy("CEMENTERIO", (int)xCem, 645);

                for (size_t i = 0; i < cementerioActual.size(); i++) {
                    float yPos = 580.0f - (i * 35.0f);

                    if ((int)i == indiceMenuRevive) {
                        ETSIDI::setTextColor(1.0f, 1.0f, 0.0f); // Cursor
                        ETSIDI::printxy(">>", (int)(xCem - 35), (int)yPos);
                    }
                    else {
                        ETSIDI::setTextColor(0.6f, 0.6f, 0.6f); // No seleccionados
                    }

                    std::string nombreP = "Desconocido";
                    if (dynamic_cast<Jedi*>(cementerioActual[i])) nombreP = "Jedi";
                    else if (dynamic_cast<CaballeroJedi*>(cementerioActual[i])) nombreP = "Caballero Jedi";
                    else if (dynamic_cast<Skywalker*>(cementerioActual[i])) nombreP = "Skywalker";
                    else if (dynamic_cast<Chewbacca*>(cementerioActual[i])) nombreP = "Chewbacca";
                    else if (dynamic_cast<Tirador*>(cementerioActual[i])) nombreP = "Tirador";
                    else if (dynamic_cast<Drone*>(cementerioActual[i])) nombreP = "Drone";
                    else if (dynamic_cast<BabyYoda*>(cementerioActual[i])) nombreP = "Baby Yoda";
                    else if (dynamic_cast<DarthVader*>(cementerioActual[i])) nombreP = "Darth Vader";

                    char buf[64];
                    sprintf(buf, "%s (ATQ: %d)", nombreP.c_str(), cementerioActual[i]->GetAtaque());
                    ETSIDI::setFont("fuentes/jedisf.ttf", 18);
                    ETSIDI::printxy(buf, (int)xCem, (int)yPos);
                }

                ETSIDI::setTextColor(0.0f, 1.0f, 1.0f);
                ETSIDI::setFont("fuentes/jedisf.ttf", 16);
                ETSIDI::printxy("W/S: Elegir   ESPACIO: Invocar", (int)xCem - 20, 150);
            }
            // Fase de Casilla: Dibujamos los cuadrados verdes en el suelo 3D
            else if (faseReviveCasilla) {
                // Hay que suspender el HUD 2D un momento para pintar en la proyección 3D del tablero
                glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);

                glDisable(GL_LIGHTING);
                glEnable(GL_BLEND);
                glColor4f(0.2f, 1.0f, 0.2f, 0.4f); // Verde translúcido
                for (auto& par : casillasLibresRevive) {
                    glPushMatrix();
                    glTranslatef((par.second - 4) * 2.0f, 0.01f, (par.first - 4) * 2.0f);
                    glBegin(GL_QUADS);
                    glVertex3f(-0.9f, 0, -0.9f);
                    glVertex3f(0.9f, 0, -0.9f);
                    glVertex3f(0.9f, 0, 0.9f);
                    glVertex3f(-0.9f, 0, 0.9f);
                    glEnd();
                    glPopMatrix();
                }
                glEnable(GL_LIGHTING);

                // Reabrimos el HUD 2D para que el resto del código no casque
                glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 1000, 0, 800);
                glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
            }
        }

        if (timerMensajeError > 0 && !mensajeErrorHechizo.empty()) {
            timerMensajeError--;
            float factorColor = 0.6f + 0.4f * sinf((float)timerMensajeError * 0.2f);
            ETSIDI::setTextColor(1.0f * factorColor, 0.1f, 0.1f); ETSIDI::setFont("fuentes/jedisf.ttf", 30);
            ETSIDI::printxy(mensajeErrorHechizo.c_str(), 180, 730);
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
    float ancho = 1.6f; float alto = 0.4f; float yBase = 3.5f;
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(x - ancho / 2.0f, yBase, z); glVertex3f(x + ancho / 2.0f, yBase, z);
    glVertex3f(x + ancho / 2.0f, yBase + alto, z); glVertex3f(x - ancho / 2.0f, yBase + alto, z);
    glEnd();

    glDisable(GL_DEPTH_TEST);
    if (porcentaje > 0.0f) {
        if (porcentaje > 0.6f) glColor3f(0.0f, 1.0f, 0.0f);
        else if (porcentaje > 0.3f) glColor3f(1.0f, 1.0f, 0.0f);
        else glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex3f(x - ancho / 2.0f, yBase, z); glVertex3f(x - ancho / 2.0f + (ancho * porcentaje), yBase, z);
        glVertex3f(x - ancho / 2.0f + (ancho * porcentaje), yBase + alto, z); glVertex3f(x - ancho / 2.0f, yBase + alto, z);
        glEnd();
    }
    glColor3f(1.0f, 1.0f, 1.0f); glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x - ancho / 2.0f, yBase, z); glVertex3f(x + ancho / 2.0f, yBase, z);
    glVertex3f(x + ancho / 2.0f, yBase + alto, z); glVertex3f(x - ancho / 2.0f, yBase + alto, z);
    glEnd();
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}

bool Tablero::CaminoLibre(int f0, int c0, int fD, int cD) {
    Pieza* p = casillas[f0][c0];
    if (p == nullptr) return false;

    // 1. Asignamos el rango máximo a cada tipo de pieza para esculpir el rombo correcto
    int rangoMaximo = 3;
    if (dynamic_cast<Skywalker*>(p) != nullptr) rangoMaximo = 5;
    else if (dynamic_cast<Tirador*>(p) != nullptr) rangoMaximo = 4;
    else if (dynamic_cast<CaballeroJedi*>(p) != nullptr) rangoMaximo = 4;
    else if (dynamic_cast<Jedi*>(p) != nullptr) rangoMaximo = 3; // El Jedi tiene menos movilidad
    // Drone, Chewbacca, BabyYoda y DarthVader se quedan con el rango 3 por defecto

    // 2. Verificamos si la pieza tiene la habilidad de volar/teletransportarse
    bool esVoladorOMago = (p->EsVoladora() || dynamic_cast<BabyYoda*>(p) != nullptr || dynamic_cast<DarthVader*>(p) != nullptr);

    // 3. BFS para TODAS las piezas. Esto garantiza el rango en forma de Rombo (Manhattan)
    struct Nodo { int f, c, dist; };
    std::queue<Nodo> q;
    bool visitado[9][9] = { false };

    q.push({ f0, c0, 0 });
    visitado[f0][c0] = true;

    int dirF[] = { -1, 1, 0, 0 };
    int dirC[] = { 0, 0, -1, 1 };

    while (!q.empty()) {
        Nodo actual = q.front();
        q.pop();

        if (actual.f == fD && actual.c == cD) {
            return true; // Se encontró ruta válida
        }

        // Expandir solo si estamos dentro del límite de pasos de la unidad
        if (actual.dist < rangoMaximo) {
            for (int i = 0; i < 4; i++) {
                int nf = actual.f + dirF[i];
                int nc = actual.c + dirC[i];

                if (nf >= 0 && nf < TAM_TABLERO && nc >= 0 && nc < TAM_TABLERO) {
                    if (!visitado[nf][nc]) {
                        // LA MAGIA ESTÁ AQUÍ: 
                        // Si vuela, puede pasar por encima de la casilla sin importar quién esté.
                        // Si camina, la casilla debe estar vacía (o ser el enemigo al que vamos a atacar).
                        if (esVoladorOMago || casillas[nf][nc] == nullptr || (nf == fD && nc == cD)) {
                            visitado[nf][nc] = true;
                            q.push({ nf, nc, actual.dist + 1 });
                        }
                    }
                }
            }
        }
    }
    return false;
}

void Tablero::avanzarTurno()
{
    turnoActual = (turnoActual == 1) ? 2 : 1;
    turnoGlobal++;
    actualizarTurnosPrision();

    int tipoMapa[9][9] = {
        {0, 0, 0, 1, 2, 1, 0, 0, 0}, {0, 0, 1, 0, 1, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0, 0, 1, 0},
        {1, 0, 0, 0, 1, 0, 0, 0, 1}, {2, 1, 1, 1, 2, 1, 1, 1, 2}, {1, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 1, 0, 0, 1, 0, 0, 1, 0}, {0, 0, 1, 0, 1, 0, 1, 0, 0}, {0, 0, 0, 1, 2, 1, 0, 0, 0}
    };

    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            Pieza* p = casillas[i][j];

            if (p != nullptr && p->EstaViva() && p->GetBando() == turnoActual) {

                if (bloqueoCuracion) continue;

                bool curar = false;

                if (tipoMapa[i][j] == 2) {
                    curar = true;
                }
                else if (tipoMapa[i][j] == 1) {
                    curar = true;
                }
                else {
                    bool esLuz = ((i + j) % 2 == 0);
                    if (p->EsAzul() && esLuz) curar = true;
                    if (p->EsRoja() && !esLuz) curar = true;
                }

                if (curar) {
                    p->recuperarVida(15);
                }
            }
        }
    }
    bloqueoCuracion = false;
}

void Tablero::dibujaMovimientoValido()
{
    // Si no hay ninguna pieza seleccionada, no hay nada que calcular ni dibujarif 
    if (!piezaSeleccionada) return;

    Pieza* piezaOrigen = casillas[filaOrigen][colOrigen];
    if (piezaOrigen == nullptr) return;

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            // Saltamos la propia casilla donde está la pieza
            if (i == filaOrigen && j == colOrigen) continue;

            if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, i, j)) {

                // 2. Validar que el camino no esté obstruido (a menos que vuele)
                if (CaminoLibre(filaOrigen, colOrigen, i, j)) {

                    Pieza* piezaDestino = casillas[i][j];

                    // 3. Determinar el color según lo que haya en la casilla destino
                    if (piezaDestino == nullptr) {
                        // Casilla vacía: Movimiento normal (El verde brillante del primer código)
                        glColor4f(0.0f, 1.0f, 0.0f, 0.25f);
                    }
                    else if (piezaDestino->GetBando() != turnoActual) {
                        // ¡Casilla con enemigo! Si se la puede comer, cambia a Rojo brillante
                        glColor4f(1.0f, 0.0f, 0.0f, 0.35f);
                    }
                    else {
                        // Si es una pieza aliada, no te la puedes comer ni mover ahí
                        continue;
                    }

                    float x = (float)(j - 4) * 2.0f;
                    float z = (float)(i - 4) * 2.0f;

                    glPushMatrix();
                    glTranslatef(x, 0.02f, z); // Un pelín elevado sobre el suelo (0.02f) para evitar Z-fighting
                    glBegin(GL_QUADS);
                    glVertex3f(-0.9f, 0.0f, -0.9f);
                    glVertex3f(0.9f, 0.0f, -0.9f);
                    glVertex3f(0.9f, 0.0f, 0.9f);
                    glVertex3f(-0.9f, 0.0f, 0.9f);
                    glEnd();
                    glPopMatrix();
                }
            }
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
