#include "tablero.h"
#include "freeglut.h"
#include "piezas.h"

Pieza* tableroPiezas[9][9];


// Constructor: pone todas las casillas a vacío (puntero nulo) al empezar
Tablero::Tablero() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j] = 0; 
        }
    }
    filaSeleccionada = 0;
    colSeleccionada = 0;
    piezaSeleccionada = false;
    filaOrigen = 0;
    colOrigen = 0;
    turnoActual = 1;
}



// Destructor: borra las piezas de la memoria cuando cerramos el juego (Capítulo de Clases)
Tablero::~Tablero() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (casillas[i][j] != 0) {
                delete casillas[i][j];
                casillas[i][j] = 0;
            }
        }
    }
}

// Inicializa las piezas iniciales
void Tablero::inicializa() {
    // Bando AZUL (1)
    casillas[0][0] = new Soldado(1);
    casillas[0][1] = new Mago(1);
    casillas[0][2] = new Arquero(1);
    casillas[0][3] = new Caballero(1);
    casillas[0][4] = new Rey(1);

    // Bando ROJO (2)
    casillas[8][4] = new Rey(2);
    casillas[8][5] = new Caballero(2);
    casillas[8][6] = new Arquero(2);
    casillas[8][7] = new Mago(2);
    casillas[8][8] = new Soldado(2);
}

// Dibuja el tablero y manda a las piezas a dibujarse a sí mismas
void Tablero::dibuja() {
    glDisable(GL_LIGHTING); // Desactivamos luces para ver los colores planos de las casillas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        10, 20, 25,   // cámara
        0, 0, 0,     // mira al centro
        0, 1, 0      // eje Y arriba
    );
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // Lógica de Puntos de Poder: esquinas y centro del área 5x5 (filas/cols 2 a 6)
            bool esEsquina5x5 = (i == 2 || i == 6) && (j == 2 || j == 6);
            bool esCentroAbsoluto = (i == 4 && j == 4);

            if (esEsquina5x5 || esCentroAbsoluto) {
                // Color para los Puntos de Poder (Dorado/Amarillo)
                glColor3ub(255, 215, 0);
            }
            else {
                // Colores normales alternos (Gris oscuro y Gris claro)
                if ((i + j) % 2 == 0) glColor3ub(60, 60, 60);
                else glColor3ub(180, 180, 180);
            }

            // Cálculo de posición (centrado en 0,0)
            float x = (j - 4) * 2.0f;
            float z = (i - 4) * 2.0f;

            // Dibujamos el cuadrado de la casilla
            glBegin(GL_QUADS);
            glVertex3f(x - 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z - 1.0f);
            glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
            glVertex3f(x - 1.0f, 0.0f, z + 1.0f);
            glEnd();

            // Dibujamos la pieza si existe (Polimorfismo)
            if (casillas[i][j] != 0) {
                casillas[i][j]->Dibujar(x, z);
            }
        }
    }
    glEnable(GL_LIGHTING);

}

