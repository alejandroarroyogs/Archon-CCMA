#include "tablero.h"
#include "freeglut.h"
#include "piezas.h"


// Constructor: pone todas las casillas a vacío (puntero nulo) al empezar
Tablero::Tablero() {
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
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
    for (int i = 0; i < TAM_TABLERO; i++) {
        for (int j = 0; j < TAM_TABLERO; j++) {
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
    for (int j = 1; j <= 7; j++)
    {
        casillas[1][j] = new Jedi(1);
    }

    casillas[1][0] = new Drone(1);
    casillas[1][8] = new Drone(1);

    casillas[0][0] = new Skywalker(1);
    casillas[0][8] = new Skywalker(1);

    casillas[0][1] = new Chewbacca(1);
    casillas[0][7] = new Chewbacca(1);

    casillas[0][2] = new Tirador(1);
    casillas[0][6] = new Tirador(1);

    casillas[0][3] = new CaballeroJedi(1);
    casillas[0][5] = new CaballeroJedi(1);

    casillas[0][4] = new BabyYoda(1);


    // Bando ROJO (2)
    for (int j = 1; j <= 7; j++)
    {
        casillas[7][j] = new Jedi(2);
    }

    casillas[7][0] = new Drone(2);
    casillas[7][8] = new Drone(2);

    casillas[8][0] = new Skywalker(2);
    casillas[8][8] = new Skywalker(2);

    casillas[8][1] = new Chewbacca(2);
    casillas[8][7] = new Chewbacca(2);

    casillas[8][2] = new Tirador(2);
    casillas[8][6] = new Tirador(2);

    casillas[8][3] = new CaballeroJedi(2);
    casillas[8][5] = new CaballeroJedi(2);

    casillas[8][4] = new DarthVader(2);
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

            // ==========================================
            //                  NUEVO
            // ==========================================

            // 1. Cursor de movimiento (Cuadrado Rojo)
            if (i == filaSeleccionada && j == colSeleccionada) {
                glDisable(GL_LIGHTING);
                glColor3ub(255, 0, 0); // Color Rojo
                glLineWidth(4.0f);     // Grosor de la línea

                glBegin(GL_LINE_LOOP);
                // Ponemos la Y en 0.1f para que se dibuje un pelín por encima del suelo y se vea bien
                glVertex3f(x - 1.0f, 0.1f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.1f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.1f, z + 1.0f);
                glVertex3f(x - 1.0f, 0.1f, z + 1.0f);
                glEnd();
                glEnable(GL_LIGHTING);
            }

            // 2. Casilla origen cuando hemos agarrado una pieza (Cuadrado Verde)
            if (piezaSeleccionada == true && i == filaOrigen && j == colOrigen) {
                glDisable(GL_LIGHTING);
                glColor3ub(0, 255, 0); // Color Verde
                glLineWidth(4.0f);

                glBegin(GL_LINE_LOOP);
                glVertex3f(x - 1.0f, 0.1f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.1f, z - 1.0f);
                glVertex3f(x + 1.0f, 0.1f, z + 1.0f);
                glVertex3f(x - 1.0f, 0.1f, z + 1.0f);
                glEnd();
                glEnable(GL_LIGHTING);
            }

            // Dibujamos la pieza si existe (Polimorfismo)
            if (casillas[i][j] != 0) {
                casillas[i][j]->Dibujar(x, z);
            }
        }
    }
    glEnable(GL_LIGHTING);

}

int Tablero::getTamTablero()
{
    return TAM_TABLERO;
}

// ==========================================
//                   NUEVO
// ==========================================
void Tablero::moverIA() {
    // Solo se ejecuta si es el turno de la IA (Bando 2)
    if (turnoActual != 2) return;

    bool movido = false;

    // 1. La IA escanea tu tablero 9x9 desde abajo hacia arriba
    for (int i = TAM_TABLERO - 1; i >= 0; i--) {
        for (int j = 0; j < TAM_TABLERO; j++) {

            // 2. Busca si en esa casilla hay una pieza suya (Bando 2)
            if (casillas[i][j] != 0 && casillas[i][j]->GetBando() == 2) {

                // 3. Evalúa opciones: Intenta mover una casilla hacia arriba (i - 1)
                // Comprueba que no se sale del tablero y que la casilla destino está vacía
                if (i - 1 >= 0 && casillas[i - 1][j] == 0) {

                    // Ejecuta el movimiento
                    casillas[i - 1][j] = casillas[i][j]; // Copia el puntero a la nueva casilla
                    casillas[i][j] = 0;                  // Borra la pieza de la casilla antigua

                    movido = true;
                    break; // Corta el bucle, ya ha movido
                }
            }
        }
        if (movido) break; // Si ya movió, sale del escaneo total
    }

    // 4. Pasa el turno de vuelta al jugador humano (Bando 1)
    if (movido) {
        turnoActual = 1;
    }
}

// ==========================================
//                  NUEVO
// ==========================================
void Tablero::tecla(unsigned char key) {
    // 1. MOVER EL CURSOR (W, A, S, D)
    if (key == 'w' || key == 'W') { if (filaSeleccionada < TAM_TABLERO - 1) filaSeleccionada++; }
    if (key == 's' || key == 'S') { if (filaSeleccionada > 0) filaSeleccionada--; }
    if (key == 'a' || key == 'A') { if (colSeleccionada > 0) colSeleccionada--; }
    if (key == 'd' || key == 'D') { if (colSeleccionada < TAM_TABLERO - 1) colSeleccionada++; }

    // 2. SELECCIONAR Y MOVER PIEZA (Barra Espaciadora)
    if (key == ' ') {

        // Fase A: Si no tenemos ninguna pieza agarrada, intentamos agarrar una
        if (piezaSeleccionada == false) {
            Pieza* p = casillas[filaSeleccionada][colSeleccionada];

            // Comprobamos que hay pieza y que es de nuestro bando
            if (p != 0 && p->GetBando() == turnoActual) {
                piezaSeleccionada = true;
                filaOrigen = filaSeleccionada;
                colOrigen = colSeleccionada;
            }
        }
        // Fase B: Si ya tenemos una pieza en la mano, intentamos soltarla
        else {
           
            // (Más adelante aquí añadiremos lo de comer/iniciar combate)
            if (casillas[filaSeleccionada][colSeleccionada] == 0) {

                // Movemos el puntero a la nueva casilla
                casillas[filaSeleccionada][colSeleccionada] = casillas[filaOrigen][colOrigen];
                // Vaciamos la casilla de origen
                casillas[filaOrigen][colOrigen] = 0;

                // Soltamos la pieza
                piezaSeleccionada = false;

                // ¡Cambio de turno!
                if (turnoActual == 1) turnoActual = 2;
                else turnoActual = 1;
            }
            // Si el jugador se arrepiente y pulsa espacio sobre la misma pieza, se suelta
            else if (filaSeleccionada == filaOrigen && colSeleccionada == colOrigen) {
                piezaSeleccionada = false;
            }
        }
    }
}