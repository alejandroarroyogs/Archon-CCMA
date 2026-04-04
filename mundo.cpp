#include "mundo.h"
#include <iostream>
#include "codigo/tablero/tablero.h"
#include "codigo/piezas/piezas.h"
#include "freeglut.h"
#include "arena.h"
#include "interfaz.h"

Mundo::Mundo() {
    estado = MENU; // Ponlo en JUGANDO para probar si se ve el tablero
}

Mundo::~Mundo() {
    LiberarTablero();
}
void Mundo::Inicializar() {
    InicializarTablero();
}
void Mundo::Dibujar() {
    // Limpiamos la pantalla UNA vez con el color base (oscuro)
   //menciono porque está en el ondrax
    //glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (estado) {
    case MENU:
        interfaz.dibujaMenu();
        break;
    case INSTRUC:
        interfaz.dibujaInstrucciones();
        break;
    case JUGANDO:
            DibujarJuego();
        break;
    }

    //glutSwapBuffers();// El swap final que muestra todo lo dibujado

}

void Mundo::DibujarJuego() {
    // 1. Configuración de cámara 3D para el tablero
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 18.0, 28.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 2. Dibujar el Tablero
    float size = 2.0f;
    int tablero = 9;
    for (int x = 0; x < tablero; x++) {
        for (int z = 0; z < tablero; z++) {
            if ((x + z) % 2 == 0) glColor3f(0.8f, 0.8f, 0.8f);
            else glColor3f(0.2f, 0.2f, 0.2f);
            float x0 = (x - tablero / 2) * size;
            float z0 = (z - tablero / 2) * size;
            glBegin(GL_QUADS);
            glVertex3f(x0, 0, z0); glVertex3f(x0 + size, 0, z0);
            glVertex3f(x0 + size, 0, z0 + size); glVertex3f(x0, 0, z0 + size);
            glEnd();
        }
    }

    // 3. Dibujar Selección (Amarillo) y Origen (Verde)
    // // Selección
    float xSel = (colSeleccionada - TAM_TABLERO / 2) * 2.0f;
    float zSel = (filaSeleccionada - TAM_TABLERO / 2) * 2.0f;
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(xSel, 0.01f, zSel); glVertex3f(xSel + 2.0f, 0.01f, zSel);
    glVertex3f(xSel + 2.0f, 0.01f, zSel + 2.0f); glVertex3f(xSel, 0.01f, zSel + 2.0f);
    glEnd();

    if (piezaSeleccionada) {
        float xOri = (colOrigen - TAM_TABLERO / 2) * 2.0f;
        float zOri = (filaOrigen - TAM_TABLERO / 2) * 2.0f;
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(xOri, 0.02f, zOri); glVertex3f(xOri + 2.0f, 0.02f, zOri);
        glVertex3f(xOri + 2.0f, 0.02f, zOri + 2.0f); glVertex3f(xOri, 0.02f, zOri + 2.0f);
        glEnd();
    }

    DibujarMovimientosValidos();

    for (int fila = 0; fila < TAM_TABLERO; fila++) {
        for (int col = 0; col < TAM_TABLERO; col++) {
            if (tableroPiezas[fila][col] != 0) {
                float x = (col - TAM_TABLERO / 2) * 2.0f + 1.0f;
                float z = (fila - TAM_TABLERO / 2) * 2.0f + 1.0f;
                tableroPiezas[fila][col]->Dibujar(x, z);
            }
        }
    }


    // 4. Dibujar Piezas
    for (int fila = 0; fila < TAM_TABLERO; fila++) {
        for (int col = 0; col < TAM_TABLERO; col++) {
            if (tableroPiezas[fila][col] != 0) {
                float x = (col - TAM_TABLERO / 2) * 2.0f + 1.0f;
                float z = (fila - TAM_TABLERO / 2) * 2.0f + 1.0f;
                tableroPiezas[fila][col]->Dibujar(x, z);
            }
        }
    }
    glutSwapBuffers();
};


void Mundo::tecla(unsigned char key)
{
    // Lógica para salir del menú
    if (estado == MENU) {
        if (key == 13) estado = JUGANDO; // ENTER para empezar
        return;
    }

    // Código original de teclado
    if (enCombate) {
        if (key == 13 && !combateIniciado && !combateResuelto)
            IniciarCombateAutomatico();
        return;
    }

    if (key == 27) exit(0);

    if (key == 'w' || key == 'W') if (filaSeleccionada > 0) filaSeleccionada--;
    if (key == 's' || key == 'S') if (filaSeleccionada < TAM_TABLERO - 1) filaSeleccionada++;
    if (key == 'a' || key == 'A') if (colSeleccionada > 0) colSeleccionada--;
    if (key == 'd' || key == 'D') if (colSeleccionada < TAM_TABLERO - 1) colSeleccionada++;

    if (key == ' ') {
        Pieza* pieza = tableroPiezas[filaSeleccionada][colSeleccionada];
        if ((turnoActual == 1 && EsAzul(pieza)) || (turnoActual == 2 && EsRoja(pieza))) {
            piezaSeleccionada = true;
            filaOrigen = filaSeleccionada;
            colOrigen = colSeleccionada;
        }
    }

    if (key == 13 && piezaSeleccionada) {
        Pieza* piezaOrigen = tableroPiezas[filaOrigen][colOrigen];
        Pieza* piezaDestino = tableroPiezas[filaSeleccionada][colSeleccionada];
        bool mismoBando = (EsAzul(piezaOrigen) && EsAzul(piezaDestino)) || (EsRoja(piezaOrigen) && EsRoja(piezaDestino));

        if (!mismoBando && MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada)) {
            if (piezaDestino != 0) {
                enCombate = true; atacante = piezaOrigen; defensor = piezaDestino;
                combateFilaOrigen = filaOrigen; combateColOrigen = colOrigen;
                combateFilaDestino = filaSeleccionada; combateColDestino = colSeleccionada;
                tiempoEntreRondas = 0; combateResuelto = false; combateIniciado = false;
                glutPostRedisplay();
                return;
            }
            tableroPiezas[filaSeleccionada][colSeleccionada] = piezaOrigen;
            tableroPiezas[filaOrigen][colOrigen] = 0;
            piezaSeleccionada = false;
            turnoActual = (turnoActual == 1) ? 2 : 1;
        }
    }

    if (key == 'c' || key == 'C') piezaSeleccionada = false;
    glutPostRedisplay();
};

void Mundo::Timer(int value)
{
    if (enCombate && combateIniciado)
    {
        if (tiempoEntreRondas > 0)
        {
            tiempoEntreRondas--;
        }
        else
        {
            if (!combateResuelto)
                EjecutarRondaCombate();
        }
    }

    if (flashAtacante > 0)
        flashAtacante--;

    if (flashDefensor > 0)
        flashDefensor--;

    glutPostRedisplay();
    
};
