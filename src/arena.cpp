#define _CRT_SECURE_NO_WARNINGS

#include "arena.h"
#include "piezas.h"
#include "tablero.h"
#include "ETSIDI.h"
#include <cmath>
#include "mundo.h"

#include "jedi.h"
#include "tirador.h"
#include "babyyoda.h"
#include "darthvader.h"
#include "caballerojedi.h"
#include "skywalker.h"
#include "drone.h"
#include "chewbacca.h"

Arena::Arena()
{
    atacante = nullptr;
    defensor = nullptr;
    flashAtacante = 0;
    flashDefensor = 0;

    velocidad = 0.08f;
    cooldownDanio = 0;
    for (int i = 0; i < 256; i++) {
        teclas[i] = false;
        teclasEspeciales[i] = false;
	}

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

    // GUARDAMOS LAS COORDENADAS FIJAS DEL TABLERO
    extern Mundo mundo;
    fOrigen = mundo.tablero.filaOrigen;
    cOrigen = mundo.tablero.colOrigen;
    fDestino = mundo.tablero.filaSeleccionada;
    cDestino = mundo.tablero.colSeleccionada;

    // Posiciones iniciales opuestas
    xA = -5.0f; zA = 0.0f;
    xD = 5.0f;  zD = 0.0f;
    cooldownDanio = 0;

    // Esto evita que los personajes se muevan solos por arrastrar teclas del combate anterior
    for (int i = 0; i < 256; i++) {
        teclas[i] = false;
        teclasEspeciales[i] = false;
    }

    contadorTregua = 200; // 3s cuenta atrás + 2s tregua (ticks x 25ms)

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

    if (atacante != nullptr && defensor != nullptr) {

        // Lógica "Street Fighter": Se miran fijamente en el eje X ignorando la profundidad
        bool atacanteIzquierda = (xA <= xD);

        // A) ATACANTE
        glPushMatrix();
        glTranslatef(xA, -3.7f, zA);
        float angA = 0.0f;
        if (atacanteIzquierda) angA = atacante->EsAzul() ? 90.0f : -90.0f; // Mira a la Derecha (+X)
        else angA = atacante->EsAzul() ? -90.0f : 90.0f;                   // Mira a la Izquierda (-X)
        glRotatef(angA, 0.0f, 1.0f, 0.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        atacante->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();

        // B) DEFENSOR
        glPushMatrix();
        glTranslatef(xD, -3.7f, zD);
        float angD = 0.0f;
        if (!atacanteIzquierda) angD = defensor->EsAzul() ? 90.0f : -90.0f; // Mira a la Derecha (+X)
        else angD = defensor->EsAzul() ? -90.0f : 90.0f;                    // Mira a la Izquierda (-X)
        glRotatef(angD, 0.0f, 1.0f, 0.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        defensor->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();
    }

    BarraVida();
    dibujaContador();
}


void Arena::dibujaPlataforma()
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

    // Validaciones de seguridad adaptadas a la VidaMax de cada pieza
    if (vidaAtacante < 0) vidaAtacante = 0;
    if (vidaAtacante > atacante->GetVidaMax()) vidaAtacante = atacante->GetVidaMax();
    if (vidaDefensor < 0) vidaDefensor = 0;
    if (vidaDefensor > defensor->GetVidaMax()) vidaDefensor = defensor->GetVidaMax();

    // CÁLCULO DE ESCALADO REAL PROPORCIONAL
    float vidaA = (float)vidaAtacante / (float)atacante->GetVidaMax();
    float vidaD = (float)vidaDefensor / (float)defensor->GetVidaMax();

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

    // ================= BARRA ATACANTE =================
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(50, 710, 350, 730); // Fondo oscuro

    if (vidaA > 0.0f) {
        if (vidaA > 0.4f) glColor3f(0.0f, 1.0f, 0.0f); // Verde
        else glColor3f(1.0f, 0.0f, 0.0f);              // Rojo (Poca vida)
        glRectf(50, 710, 50 + (300.0f * vidaA), 730);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP); // Borde blanco
    glVertex2f(50, 710); glVertex2f(350, 710);
    glVertex2f(350, 730); glVertex2f(50, 730);
    glEnd();

    // ================= BARRA DEFENSOR =================
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(650, 710, 950, 730); // Fondo oscuro

    if (vidaD > 0.0f) {
        if (vidaD > 0.4f) glColor3f(0.0f, 1.0f, 0.0f); // Verde
        else glColor3f(1.0f, 0.0f, 0.0f);              // Rojo (Poca vida)
        glRectf(650, 710, 650 + (300.0f * vidaD), 730);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP); // Borde blanco
    glVertex2f(650, 710); glVertex2f(950, 710);
    glVertex2f(950, 730); glVertex2f(650, 730);
    glEnd();

    // ================= TEXTOS FLOTANTES =================
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    ETSIDI::setFont("fuentes/jedisf.ttf", 22);

    // Textos Atacante (Color de la facción)
    if (atacante->GetBando() == 1) ETSIDI::setTextColor(0.2f, 0.8f, 1.0f);
    else ETSIDI::setTextColor(1.0f, 0.2f, 0.2f);
    ETSIDI::printxy("ATACANTE", 50, 740);

    // Cambiado para mostrar (Actual / Máxima)
    char bufA[32]; sprintf(bufA, "HP: %d / %d", vidaAtacante, atacante->GetVidaMax());
    ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
    ETSIDI::printxy(bufA, 210, 740);

    // Textos Defensor (Color de la facción)
    if (defensor->GetBando() == 1) ETSIDI::setTextColor(0.2f, 0.8f, 1.0f);
    else ETSIDI::setTextColor(1.0f, 0.2f, 0.2f);
    ETSIDI::printxy("DEFENSOR", 650, 740);

    // Cambiado para mostrar (Actual / Máxima)
    char bufD[32]; sprintf(bufD, "HP: %d / %d", vidaDefensor, defensor->GetVidaMax());
    ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
    ETSIDI::printxy(bufD, 810, 740);

    // Restaurar estados OpenGL
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
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


void Arena::actualiza()
 {
    if (atacante == nullptr || defensor == nullptr) return;

    if (contadorTregua > 0) contadorTregua--;

    // 1. MOVIMIENTO ATACANTE (W, A, S, D)
    if (atacante->GetBando() == 1) {
    // Jugador humano: WASD
    if (teclas['w'] || teclas['W']) zA -= velocidad;
    if (teclas['s'] || teclas['S']) zA += velocidad;
    if (teclas['a'] || teclas['A']) xA -= velocidad;
    if (teclas['d'] || teclas['D']) xA += velocidad;
    }
    else {
        //IA avanza hacia defensor
        if (contadorTregua <= 0) {
            float dirX = xD - xA;
            float dirZ = zD - zA;
            float dist = sqrt(dirX * dirX + dirZ * dirZ);
            if (dist > 0.1f) {
                xA += (dirX / dist) * velocidad;
                zA += (dirZ / dist) * velocidad;
            }
        }
    }

    // 2. MOVIMIENTO DEFENSOR (Flechas)
    if (defensor->GetBando() == 1) {
    if (teclasEspeciales[GLUT_KEY_UP])    zD -= velocidad;
    if (teclasEspeciales[GLUT_KEY_DOWN])  zD += velocidad;
    if (teclasEspeciales[GLUT_KEY_LEFT])  xD -= velocidad;
    if (teclasEspeciales[GLUT_KEY_RIGHT]) xD += velocidad;
    }
    else {
        // IA avanza hacia el atacante
        if (contadorTregua <= 0) {
            float dirX = xA - xD;
            float dirZ = zA - zD;
            float dist = sqrt(dirX * dirX + dirZ * dirZ);
            if (dist > 0.1f) {
                xD += (dirX / dist) * velocidad;
                zD += (dirZ / dist) * velocidad;
            }
        }
    }

    // Límites de la plataforma
    if (xA > 7.0f) xA = 7.0f; if (xA < -7.0f) xA = -7.0f;
    if (zA > 3.5f) zA = 3.5f; if (zA < -3.5f) zA = -3.5f;
    if (xD > 7.0f) xD = 7.0f; if (xD < -7.0f) xD = -7.0f;
    if (zD > 3.5f) zD = 3.5f; if (zD < -3.5f) zD = -3.5f;

    // 3. GESTIÓN DE COLISIÓN ÚNICA (ESCARAMUZA)
    float difX = xA - xD;
    float difZ = zA - zD;
    float distancia = sqrt((difX * difX) + (difZ * difZ));
    float radioColision = 1.5f;

    if (distancia < radioColision) {
        // ¡CHOQUE! Aplicamos daño cruzado con mitigación de armadura
        int danoAlDefensor = atacante->GetAtaque() - defensor->GetArmadura();
        int danoAlAtacante = defensor->GetAtaque() - atacante->GetArmadura();

        if (danoAlDefensor > 0) defensor->RecibirDanio(danoAlDefensor);
        if (danoAlAtacante > 0) atacante->RecibirDanio(danoAlAtacante);

        // --- 4. LÓGICA ROBUSTA DE POSICIONAMIENTO ADYACENTE EN EL TABLERO ---
        extern Mundo mundo;

        // CASO A: El defensor muere y el atacante sobrevive -> Ocupa la casilla del defensor
        if (!defensor->EstaViva() && atacante->EstaViva()) {
            mundo.tablero.casillas[fDestino][cDestino] = atacante;
            mundo.tablero.casillas[fOrigen][cOrigen] = nullptr;
        }
        // CASO B: El atacante muere y el defensor sobrevive -> El atacante desaparece del mapa
        else if (!atacante->EstaViva() && defensor->EstaViva()) {
            mundo.tablero.casillas[fOrigen][cOrigen] = nullptr;
        }
        // CASO C: Ambos mueren en el choque simultáneo -> Ambas casillas quedan vacías
        else if (!atacante->EstaViva() && !defensor->EstaViva()) {
            mundo.tablero.casillas[fDestino][cDestino] = nullptr;
            mundo.tablero.casillas[fOrigen][cOrigen] = nullptr;
        }
        // CASO D: Ambos sobreviven -> El atacante se queda adyacente al defensor
        else if (atacante->EstaViva() && defensor->EstaViva()) {

            int mejorF = fOrigen;
            int mejorC = cOrigen;
            float distMin = 9999.0f;

            // Buscamos candidatos alrededor del DEFENSOR (donde ocurrió el choque)
            for (int df = -1; df <= 1; df++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (df == 0 && dc == 0) continue;

                    int fCand = fDestino + df;
                    int cCand = cDestino + dc;

                    if (fCand >= 0 && fCand < 9 && cCand >= 0 && cCand < 9) {
                        // Es un sitio válido si está vacío O si es la casilla donde estaba el atacante
                        if (mundo.tablero.casillas[fCand][cCand] == nullptr ||
                            (fCand == fOrigen && cCand == cOrigen)) {

                            // Distancia desde el atacante al candidato (queremos el que esté más cerca del atacante)
                            float dist = sqrt(pow(fCand - fOrigen, 2) + pow(cCand - cOrigen, 2));

                            if (dist < distMin) {
                                distMin = dist;
                                mejorF = fCand;
                                mejorC = cCand;
                            }
                        }
                    }
                }
            }

            // Realizamos el movimiento a la mejor casilla encontrada
            mundo.tablero.casillas[mejorF][mejorC] = atacante;
            if (mejorF != fOrigen || mejorC != cOrigen) {
                mundo.tablero.casillas[fOrigen][cOrigen] = nullptr;
            }
        }

        // --- 5. FIN DEL COMBATE Y CAMBIO DE TURNO ---
        extern Mundo mundo;
        mundo.tablero.setBloqueoCuracion(true); // Bloqueamos curación justo antes de avanzar
        mundo.tablero.avanzarTurno();

        extern Estado estado;
        estado = JUGANDO;
    }
}

void Arena::dibujaContador()
{
    if (contadorTregua <= 0) return;

    const char* texto = "";
    if (contadorTregua > 160) texto = "3";
    else if (contadorTregua > 120) texto = "2";
    else if (contadorTregua > 80)  texto = "1";
    else                           texto = "FIGHT!";

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    if (contadorTregua > 80) {
        ETSIDI::setTextColor(1.0f, 1.0f, 0.0f); // Amarillo
    }
    else {
        ETSIDI::setTextColor(0.0f, 1.0f, 0.2f); // Verde
    }

    ETSIDI::setFont("fuentes/jedisf.ttf", 90);

    if (contadorTregua > 80) {
        ETSIDI::printxy(texto, 460, 500); // 3, 2, 1
    }
    else {
        ETSIDI::printxy(texto, 340, 500); // FIGHT!
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Arena::keyDown(unsigned char key) { teclas[key] = true; }
void Arena::keyUp(unsigned char key) { teclas[key] = false; }
void Arena::specialKeyDown(int key) { teclasEspeciales[key] = true; }
void Arena::specialKeyUp(int key) { teclasEspeciales[key] = false; }