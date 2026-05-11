#define _CRT_SECURE_NO_WARNINGS
#include "tablero.h"
#include "freeglut.h"
#include "piezas.h"
#include "mundo.h"      
#include <cmath>
#include "ETSIDI.h"
#include <iostream>

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

    // Inicialización del estado de selección de hechizos
    seleccionandoHechizo = false;
    indiceHechizoSeleccionado = -1;

    // Mensajes de error
    mensajeErrorHechizo = "";
    timerMensajeError = 0;

    // Estado del teleport
    faseTeleportDestino = false;
    filaTeleportOrigen = -1;
    colTeleportOrigen = -1;

    for (int i = 0; i < 256; i++) {
        teclasPulsadas[i] = false;
    }
    cooldownMovimiento = 0;

    // Cargar los 7 hechizos en orden
    hechizosAzules.push_back(new HechizoHeal());
    hechizosAzules.push_back(new HechizoTeleport());
    hechizosAzules.push_back(new HechizoShiftTime());
    hechizosAzules.push_back(new HechizoExchange());
    hechizosAzules.push_back(new HechizoSummon());
    hechizosAzules.push_back(new HechizoRevive());
    hechizosAzules.push_back(new HechizoImprison());

    hechizosRojos.push_back(new HechizoHeal());
    hechizosRojos.push_back(new HechizoTeleport());
    hechizosRojos.push_back(new HechizoShiftTime());
    hechizosRojos.push_back(new HechizoExchange());
    hechizosRojos.push_back(new HechizoSummon());
    hechizosRojos.push_back(new HechizoRevive());
    hechizosRojos.push_back(new HechizoImprison());
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

                // === DIBUJO DE LA CAJA DE CARBONITA SEMITRANSPARENTE ===
                if (casillas[i][j]->estaEncarcelada) {
                    glPushMatrix();
                    glTranslatef(x, 1.2f, z); // La elevamos un poco para que cubra el cuerpo de la pieza

                    glDisable(GL_LIGHTING);
                    glEnable(GL_BLEND);
                    glDepthMask(GL_FALSE); // Evita errores de oclusión con transparencias

                    // 1. CARAS TRANSLÚCIDAS (Azul holograma suave)
                    glColor4f(0.0f, 0.5f, 1.0f, 0.35f); // RGB + 35% de opacidad (Alpha)
                    glPushMatrix();
                    glScalef(1.8f, 2.4f, 1.8f); // Escalamos el cubo para envolver la pieza
                    glutSolidCube(1.0);
                    glPopMatrix();

                    // 2. BORDES SÓLIDOS (Efecto neón brillante)
                    glColor4f(0.0f, 0.8f, 1.0f, 0.9f); // Azul celeste casi opaco
                    glLineWidth(2.0f);
                    glPushMatrix();
                    glScalef(1.8f, 2.4f, 1.8f);
                    glutWireCube(1.0);
                    glPopMatrix();
                    glLineWidth(1.0f);

                    glDepthMask(GL_TRUE); // Restauramos la escritura en el buffer de profundidad
                    glDisable(GL_BLEND);
                    glEnable(GL_LIGHTING);
                    glPopMatrix();
                }

                dibujaBarraVida(x, z, casillas[i][j]->GetVida(), 100);
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

        if (faseTeleportDestino) {
            // FASE 2: Azul Celeste parpadeante
            glColor3f(0.0f * intensidad, 0.8f * intensidad, 1.0f);
            glLineWidth(4.0f);
        }
        else {
            // FASE 1: Amarillo/Dorado parpadeante
            glColor3f(1.0f * intensidad, 0.85f * intensidad, 0.0f);
            glLineWidth(4.0f);
        }
    }
    else if (piezaSeleccionada) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glLineWidth(3.0f);
    }
    else {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(3.0f);
    }

    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glEnd();

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // Dibujo del origen de movimiento estándar
    if (piezaSeleccionada) {
        glPushMatrix();
        float origX = (float)(colOrigen - 4) * 2.0f;
        float origZ = (float)(filaOrigen - 4) * 2.0f;
        glTranslatef(origX, 0.16f, origZ);
        glDisable(GL_LIGHTING);
        glColor3ub(0, 255, 0);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glEnd();
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    // Dibujo de la marca de la pieza que va a saltar por el Salto Hiperespacial
    if (seleccionandoHechizo && faseTeleportDestino) {
        glPushMatrix();
        float origX = (float)(colTeleportOrigen - 4) * 2.0f;
        float origZ = (float)(filaTeleportOrigen - 4) * 2.0f;
        glTranslatef(origX, 0.16f, origZ);
        glDisable(GL_LIGHTING);
        glColor3ub(255, 215, 0); // Amarillo estático
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glEnd();
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    dibujaInterfazHechizos();
}

void Tablero::tecla(unsigned char key) {
    teclasPulsadas[key] = true;
    actualizarMovimiento();

    // --- CASO 1: Estamos apuntando un hechizo ---
    if (seleccionandoHechizo) {
        if (key == 13 || key == ' ') {
            teclasPulsadas[key] = false; // Evitamos repeticiones
            confirmarObjetivoHechizo();
            return;
        }
        if (key == 8 || key == 'q' || key == 'Q') {
            teclasPulsadas[key] = false;
            cancelarSeleccionHechizo();
            return;
        }
    }

    // --- CASO 2: Modo normal de juego ---
    if (!seleccionandoHechizo) {
        if (key >= '1' && key <= '7') {
            int indiceHechizo = key - '1';
            lanzarHechizo(indiceHechizo);
            return;
        }

        if (key == ' ') {
            if (!piezaSeleccionada) {
                Pieza* p = casillas[filaSeleccionada][colSeleccionada];
                if (p != nullptr && p->GetBando() == turnoActual) {
                    // --- NUEVA VALIDACIÓN: ¿Está en carbonita? ---
                    if (p->estaEncarcelada) {
                        mensajeErrorHechizo = "UNIDAD CONGELADA EN CARBONITA";
                        timerMensajeError = 120;
                        std::cout << "[TABLERO] No puedes mover esta pieza. Esta congelada por "
                            << p->turnosPrision << " turnos." << std::endl;
                        return;
                    }
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
                        actualizarTurnosPrision();
                        turnoActual = (turnoActual == 1) ? 2 : 1;
                    }
                }
                else if (piezaDestino->GetBando() != turnoActual) {
                    if (piezaOrigen->MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
                        mundo.arena.inicializa(piezaOrigen, piezaDestino, turnoActual);
                        estado = COMBATE;
                        piezaSeleccionada = false;
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

Pieza* Tablero::getPiezaEnCursor() {
    return casillas[filaSeleccionada][colSeleccionada];
}

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
    if (p != nullptr && p->GetBando() == turnoActual) {
        if (turnoActual == 1 && dynamic_cast<BabyYoda*>(p) != nullptr) esLanzadorValido = true;
        else if (turnoActual == 2 && dynamic_cast<DarthVader*>(p) != nullptr) esLanzadorValido = true;
    }

    if (!esLanzadorValido) {
        mensajeErrorHechizo = "SOLO EL HECHICERO PUEDE LANZAR HECHIZOS";
        timerMensajeError = 120;
        return;
    }

    std::vector<Hechizo*>& mazo = (turnoActual == 1) ? hechizosAzules : hechizosRojos;

    if (indice < 0 || indice >= (int)mazo.size()) return;

    Hechizo* hechizo = mazo[indice];

    if (hechizo->estaUsado()) {
        mensajeErrorHechizo = "ESTE PODER YA FUE AGOTADO";
        timerMensajeError = 100;
        return;
    }

    seleccionandoHechizo = true;
    indiceHechizoSeleccionado = indice;
    faseTeleportDestino = false;

    std::cout << "[HECHIZO] Selector activado para: " << hechizo->getNombre() << std::endl;
}

void Tablero::confirmarObjetivoHechizo() {
    if (!seleccionandoHechizo || indiceHechizoSeleccionado == -1) return;

    std::vector<Hechizo*>& mazo = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
    Hechizo* hechizo = mazo[indiceHechizoSeleccionado];

    // === LOGICA DEL SALTO HIPERESPACIAL (Indice 1) ===
    if (indiceHechizoSeleccionado == 1) {
        Pieza* objetivo = casillas[filaSeleccionada][colSeleccionada];

        // Fase 1: Seleccionar aliado
        if (!faseTeleportDestino) {
            if (objetivo == nullptr) {
                mensajeErrorHechizo = "SELECCIONA UNA PIEZA ALIADA";
                timerMensajeError = 100;
                return;
            }
            if (objetivo->GetBando() != turnoActual) {
                mensajeErrorHechizo = "NO PUEDES MOVER ENEMIGOS";
                timerMensajeError = 100;
                return;
            }

            filaTeleportOrigen = filaSeleccionada;
            colTeleportOrigen = colSeleccionada;
            faseTeleportDestino = true;
            return;
        }
        // Fase 2: Mover a destino vacío
        else {
            if (objetivo != nullptr) {
                mensajeErrorHechizo = "LA CASILLA DEBE ESTAR VACIA";
                timerMensajeError = 100;
                return;
            }

            if (hechizo->aplica(mundo, nullptr)) {
                casillas[filaSeleccionada][colSeleccionada] = casillas[filaTeleportOrigen][colTeleportOrigen];
                casillas[filaTeleportOrigen][colTeleportOrigen] = nullptr;

                hechizo->setUsado(true);
                seleccionandoHechizo = false;
                indiceHechizoSeleccionado = -1;
                faseTeleportDestino = false;
                filaTeleportOrigen = -1;
                colTeleportOrigen = -1;

                actualizarTurnosPrision();
                turnoActual = (turnoActual == 1) ? 2 : 1;
            }
        }
    }
    // === LÓGICA ESTÁNDAR PARA EL RESTO DE HECHIZOS (Curación, etc.) ===
    else {
        Pieza* objetivo = casillas[filaSeleccionada][colSeleccionada];

        if (hechizo->aplica(mundo, objetivo)) {
            hechizo->setUsado(true);
            seleccionandoHechizo = false;
            indiceHechizoSeleccionado = -1;
            turnoActual = (turnoActual == 1) ? 2 : 1;
        }
        else {
            std::cout << "[HECHIZO] Objetivo invalido." << std::endl;
        }
    }
}

void Tablero::cancelarSeleccionHechizo() {
    if (seleccionandoHechizo) {
        seleccionandoHechizo = false;
        indiceHechizoSeleccionado = -1;
        faseTeleportDestino = false;
        filaTeleportOrigen = -1;
        colTeleportOrigen = -1;
        std::cout << "[HECHIZO] Lanzamiento cancelado." << std::endl;
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

    float panelAncho = 230.0f;
    float xInicio = (turnoActual == 1) ? 0.0f : 1000.0f - panelAncho;
    float xTexto = xInicio + 15.0f;
    float xBorde = (turnoActual == 1) ? panelAncho : xInicio;

    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(xInicio, 800.0f); glVertex2f(xInicio + panelAncho, 800.0f);
    glVertex2f(xInicio + panelAncho, 0.0f); glVertex2f(xInicio, 0.0f);
    glEnd();

    if (turnoActual == 1) glColor3f(0.4f, 1.0f, 1.0f);
    else glColor3f(1.0f, 0.2f, 0.2f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(xBorde, 800.0f); glVertex2f(xBorde, 0.0f);
    glEnd();

    // Dibujo del HUD de información del objetivo
    Pieza* pBajoCursor = getPiezaEnCursor();
    if (seleccionandoHechizo && faseTeleportDestino) {
        ETSIDI::setTextColor(0.0f, 0.8f, 1.0f);
        ETSIDI::setFont("fuentes/jedisf.ttf", 20);
        ETSIDI::printxy("DESTINO", (int)xTexto, 750);

        ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
        ETSIDI::setFont("fuentes/jedisf.ttf", 16);
        ETSIDI::printxy("ELIGE CASILLA VACIA", (int)xTexto, 720);
    }
    else if (pBajoCursor != nullptr) {
        ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
        ETSIDI::setFont("fuentes/jedisf.ttf", 20);
        char info[64];
        sprintf(info, "OBJ: %s", (pBajoCursor->GetBando() == turnoActual) ? "ALIADO" : "ENEMIGO");
        ETSIDI::printxy(info, (int)xTexto, 750);

        sprintf(info, "VIDA: %d%%", pBajoCursor->GetVida());
        ETSIDI::printxy(info, (int)xTexto, 720);
    }

    ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    ETSIDI::setFont("fuentes/jedisf.ttf", 26);
    ETSIDI::printxy("PODERES", (int)xTexto, 650);

    std::vector<Hechizo*>& lista = (turnoActual == 1) ? hechizosAzules : hechizosRojos;
    for (int i = 0; i < (int)lista.size(); i++) {
        float yPos = 580.0f - ((float)i * 60.0f);

        if (lista[i]->estaUsado()) ETSIDI::setTextColor(0.5f, 0.5f, 0.5f);
        else (turnoActual == 1) ? ETSIDI::setTextColor(0.4f, 1.0f, 0.0f) : ETSIDI::setTextColor(1.0f, 0.4f, 0.4f);

        ETSIDI::setFont("fuentes/jedisf.ttf", 18);
        char buffer[64];
        sprintf(buffer, "[%d] %s", i + 1, lista[i]->getNombre().c_str());
        ETSIDI::printxy(buffer, (int)xTexto, (int)yPos);

        if (lista[i]->estaUsado()) {
            ETSIDI::setFont("fuentes/jedisf.ttf", 14);
            ETSIDI::printxy("   AGOTADO", (int)xTexto, (int)(yPos - 20.0f));
        }
    }

    // Dibujo del Mensaje de Advertencia Temporal
    if (timerMensajeError > 0 && !mensajeErrorHechizo.empty()) {
        timerMensajeError--;

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 1000, 0, 800);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float factorColor = 0.6f + 0.4f * sinf((float)timerMensajeError * 0.2f);
        ETSIDI::setTextColor(1.0f * factorColor, 0.1f, 0.1f);
        ETSIDI::setFont("fuentes/jedisf.ttf", 30);

        ETSIDI::printxy(mensajeErrorHechizo.c_str(), 180, 730);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
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

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(x - ancho / 2.0f, yBase, z);
    glVertex3f(x + ancho / 2.0f, yBase, z);
    glVertex3f(x + ancho / 2.0f, yBase + alto, z);
    glVertex3f(x - ancho / 2.0f, yBase + alto, z);
    glEnd();

    glDisable(GL_DEPTH_TEST);
    if (porcentaje > 0.0f) {
        if (porcentaje > 0.6f)      glColor3f(0.0f, 1.0f, 0.0f);
        else if (porcentaje > 0.3f) glColor3f(1.0f, 1.0f, 0.0f);
        else                       glColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_QUADS);
        glVertex3f(x - ancho / 2.0f, yBase, z);
        glVertex3f(x - ancho / 2.0f + (ancho * porcentaje), yBase, z);
        glVertex3f(x - ancho / 2.0f + (ancho * porcentaje), yBase + alto, z);
        glVertex3f(x - ancho / 2.0f, yBase + alto, z);
        glEnd();
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x - ancho / 2.0f, yBase, z);
    glVertex3f(x + ancho / 2.0f, yBase, z);
    glVertex3f(x + ancho / 2.0f, yBase + alto, z);
    glVertex3f(x - ancho / 2.0f, yBase + alto, z);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
void Tablero::actualizarTurnosPrision() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
            if (casillas[i][j] != nullptr) {
                casillas[i][j]->reducirCondena();
            }
        }
    }
}