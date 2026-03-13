#include "freeglut.h"
#include "codigo/tablero/tablero.h"
#include "codigo/piezas/piezas.h"

// Callbacks
void OnDraw(void);
void OnTimer(int value);
void OnKeyboardDown(unsigned char key, int x, int y);

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 800);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("ARCHON");

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1000 / 800.0f, 0.1, 150);

    InicializarTablero();

    glutDisplayFunc(OnDraw);
    glutTimerFunc(25, OnTimer, 0);
    glutKeyboardFunc(OnKeyboardDown);

    glutMainLoop();

    return 0;
}

void OnDraw(void)
{
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 18.0, 28.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    // Tablero 
    float size = 2.0f;
    int tablero = 9;

    for (int x = 0; x < tablero; x++)
    {
        for (int z = 0; z < tablero; z++)
        {
            if ((x + z) % 2 == 0)
                glColor3f(0.8f, 0.8f, 0.8f);
            else
                glColor3f(0.2f, 0.2f, 0.2f);

            float x0 = (x - tablero / 2) * size;
            float z0 = (z - tablero / 2) * size;

            glBegin(GL_QUADS);

            glVertex3f(x0, 0, z0);
            glVertex3f(x0 + size, 0, z0);
            glVertex3f(x0 + size, 0, z0 + size);
            glVertex3f(x0, 0, z0 + size);

            glEnd();
        }
    }

    // Dibujar selección
    float xSel = (colSeleccionada - TAM_TABLERO / 2) * 2.0f;
    float zSel = (filaSeleccionada - TAM_TABLERO / 2) * 2.0f;

    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
    glVertex3f(xSel, 0.01f, zSel);
    glVertex3f(xSel + 2.0f, 0.01f, zSel);
    glVertex3f(xSel + 2.0f, 0.01f, zSel + 2.0f);
    glVertex3f(xSel, 0.01f, zSel + 2.0f);
    glEnd();

    // Dibujar origen de pieza seleccionada
    if (piezaSeleccionada)
    {
        float xOri = (colOrigen - TAM_TABLERO / 2) * 2.0f;
        float zOri = (filaOrigen - TAM_TABLERO / 2) * 2.0f;

        glColor3f(0.0f, 1.0f, 0.0f);
        glLineWidth(3.0f);

        glBegin(GL_LINE_LOOP);
        glVertex3f(xOri, 0.02f, zOri);
        glVertex3f(xOri + 2.0f, 0.02f, zOri);
        glVertex3f(xOri + 2.0f, 0.02f, zOri + 2.0f);
        glVertex3f(xOri, 0.02f, zOri + 2.0f);
        glEnd();
    }

    DibujarMovimientosValidos();

    // Dibujar piezas leyendo la matriz
    for (int fila = 0; fila < TAM_TABLERO; fila++)
    {
        for (int col = 0; col < TAM_TABLERO; col++)
        {
            float x = (col - TAM_TABLERO / 2) * 2.0f + 1.0f;
            float z = (fila - TAM_TABLERO / 2) * 2.0f + 1.0f;

            if (tableroPiezas[fila][col] == SOLDADO_AZUL)
            {
                DibujarPieza(x, z, 0.2f, 0.8f, 1.0f, 1);
            }
            else if (tableroPiezas[fila][col] == SOLDADO_ROJO)
            {
                DibujarPieza(x, z, 1.0f, 0.3f, 0.3f, 1);
            }
            else if (tableroPiezas[fila][col] == MAGO_AZUL)
            {
                DibujarPieza(x, z, 0.0f, 0.4f, 1.0f, 2);
            }
            else if (tableroPiezas[fila][col] == MAGO_ROJO)
            {
                DibujarPieza(x, z, 0.8f, 0.0f, 0.8f, 2);
            }
        }
    }

    glutSwapBuffers();
}

void OnKeyboardDown(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);

    // mover cursor
    if (key == 'w' || key == 'W')
        if (filaSeleccionada > 0) filaSeleccionada--;

    if (key == 's' || key == 'S')
        if (filaSeleccionada < TAM_TABLERO - 1) filaSeleccionada++;

    if (key == 'a' || key == 'A')
        if (colSeleccionada > 0) colSeleccionada--;

    if (key == 'd' || key == 'D')
        if (colSeleccionada < TAM_TABLERO - 1) colSeleccionada++;

    // seleccionar pieza
    if (key == ' ')
    {
        int pieza = tableroPiezas[filaSeleccionada][colSeleccionada];

        if ((turnoActual == 1 && EsAzul(pieza)) ||
            (turnoActual == 2 && EsRoja(pieza)))
        {
            piezaSeleccionada = true;
            filaOrigen = filaSeleccionada;
            colOrigen = colSeleccionada;
        }
    }

    // mover o capturar
    if (key == 13) // ENTER
    {
        if (piezaSeleccionada)
        {
            int piezaOrigen = tableroPiezas[filaOrigen][colOrigen];
            int piezaDestino = tableroPiezas[filaSeleccionada][colSeleccionada];

       
            bool mismoBando = (EsAzul(piezaOrigen) && EsAzul(piezaDestino)) ||
                (EsRoja(piezaOrigen) && EsRoja(piezaDestino));

            if (!mismoBando &&
                MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada))
            {
                tableroPiezas[filaSeleccionada][colSeleccionada] = piezaOrigen;
                tableroPiezas[filaOrigen][colOrigen] = VACIA;

                piezaSeleccionada = false;

                if (turnoActual == 1)
                    turnoActual = 2;
                else
                    turnoActual = 1;
            }
        }
    }

    // cancelar selección
    if (key == 'c' || key == 'C')
    {
        piezaSeleccionada = false;
    }

    glutPostRedisplay();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(25, OnTimer, 0);
}
