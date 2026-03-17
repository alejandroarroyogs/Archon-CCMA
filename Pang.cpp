#include "freeglut.h"
#include "codigo/tablero/tablero.h"
#include "codigo/piezas/piezas.h"

// Callbacks
void OnDraw(void);
void OnTimer(int value);
void OnKeyboardDown(unsigned char key, int x, int y);
void DibujarBarraVida(float x, float y, float z, int vidaActual, int vidaMax);

void DibujarBarraVida(float x, float y, float z, int vidaActual, int vidaMax)
{
    float anchoTotal = 4.0f;
    float alto = 0.35f;

    float porcentaje = 0.0f;
    if (vidaMax > 0)
        porcentaje = (float)vidaActual / (float)vidaMax;

    if (porcentaje < 0.0f)
        porcentaje = 0.0f;
    if (porcentaje > 1.0f)
        porcentaje = 1.0f;

    glPushMatrix();
    glTranslatef(x, y, z);

    // Fondo
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-anchoTotal / 2.0f, 0.0f, 0.0f);
    glVertex3f(anchoTotal / 2.0f, 0.0f, 0.0f);
    glVertex3f(anchoTotal / 2.0f, alto, 0.0f);
    glVertex3f(-anchoTotal / 2.0f, alto, 0.0f);
    glEnd();

    // Vida actual
    if (porcentaje > 0.5f)
        glColor3f(0.1f, 0.9f, 0.1f);
    else if (porcentaje > 0.25f)
        glColor3f(0.95f, 0.75f, 0.1f);
    else
        glColor3f(0.9f, 0.1f, 0.1f);

    float anchoVida = anchoTotal * porcentaje;

    glBegin(GL_QUADS);
    glVertex3f(-anchoTotal / 2.0f, 0.0f, 0.01f);
    glVertex3f(-anchoTotal / 2.0f + anchoVida, 0.0f, 0.01f);
    glVertex3f(-anchoTotal / 2.0f + anchoVida, alto, 0.01f);
    glVertex3f(-anchoTotal / 2.0f, alto, 0.01f);
    glEnd();

    glPopMatrix();
}

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

    LiberarTablero();

    return 0;
}

void OnDraw(void)
{
    if (enCombate)
    {
        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(0.0, 5.0, 15.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

        // Suelo simple de arena
        glColor3f(0.25f, 0.08f, 0.08f);
        glBegin(GL_QUADS);
        glVertex3f(-8.0f, -1.0f, -4.0f);
        glVertex3f(8.0f, -1.0f, -4.0f);
        glVertex3f(8.0f, -1.0f, 4.0f);
        glVertex3f(-8.0f, -1.0f, 4.0f);
        glEnd();

        // Dibujar atacante
        if (atacante != 0)
        {
            if (flashAtacante > 0)
                glColor3f(1.0f, 1.0f, 1.0f);

            atacante->DibujarCombate(-3.0f, 0.0f, flashAtacante > 0);
            DibujarBarraVida(-3.0f, 2.4f, 0.0f, atacante->GetVida(), 15);
        }

        // Dibujar defensor
        if (defensor != 0)
        {
            if (flashDefensor > 0)
                glColor3f(1.0f, 1.0f, 1.0f);

            defensor->DibujarCombate(3.0f, 0.0f, flashDefensor > 0);
            DibujarBarraVida(3.0f, 2.4f, 0.0f, defensor->GetVida(), 15);
        }

        glutSwapBuffers();
        return;
    }

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
            if (tableroPiezas[fila][col] != 0)
            {
                float x = (col - TAM_TABLERO / 2) * 2.0f + 1.0f;
                float z = (fila - TAM_TABLERO / 2) * 2.0f + 1.0f;

                tableroPiezas[fila][col]->Dibujar(x, z);
            }
        }
    }

    glutSwapBuffers();
}

void OnKeyboardDown(unsigned char key, int x, int y)
{
    if (enCombate)
    {
        if (key == 13) // ENTER
        {
            if (!combateIniciado && !combateResuelto)
                IniciarCombateAutomatico();

            glutPostRedisplay();
        }

        return;
    }

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
        Pieza* pieza = tableroPiezas[filaSeleccionada][colSeleccionada];

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
            Pieza* piezaOrigen = tableroPiezas[filaOrigen][colOrigen];
            Pieza* piezaDestino = tableroPiezas[filaSeleccionada][colSeleccionada];

            bool mismoBando = (EsAzul(piezaOrigen) && EsAzul(piezaDestino)) ||
                (EsRoja(piezaOrigen) && EsRoja(piezaDestino));

            if (!mismoBando &&
                MovimientoValido(filaOrigen, colOrigen, filaSeleccionada, colSeleccionada))
            {
                if (piezaDestino != 0)
                {
                    enCombate = true;
                    atacante = piezaOrigen;
                    defensor = piezaDestino;

                    combateFilaOrigen = filaOrigen;
                    combateColOrigen = colOrigen;
                    combateFilaDestino = filaSeleccionada;
                    combateColDestino = colSeleccionada;

                    tiempoEntreRondas = 0;
                    combateResuelto = false;
                    combateIniciado = false;

                    glutPostRedisplay();
                    return;
                }

                tableroPiezas[filaSeleccionada][colSeleccionada] = piezaOrigen;
                tableroPiezas[filaOrigen][colOrigen] = 0;

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
    glutTimerFunc(25, OnTimer, 0);
}