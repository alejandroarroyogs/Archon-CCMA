#include "../../freeglut.h"
#include "tablero.h"
#include "../piezas/piezas.h"

int tableroPiezas[TAM_TABLERO][TAM_TABLERO];

int filaSeleccionada = 0;
int colSeleccionada = 0;

bool piezaSeleccionada = false;
int filaOrigen;
int colOrigen;

int turnoActual = 1; // 1 = azul, 2 = rojo

void InicializarTablero()
{
    for (int i = 0; i < TAM_TABLERO; i++)
    {
        for (int j = 0; j < TAM_TABLERO; j++)
        {
            tableroPiezas[i][j] = VACIA;
        }
    }

    // Bando azul
    tableroPiezas[0][0] = SOLDADO_AZUL;
    tableroPiezas[0][1] = MAGO_AZUL;
    tableroPiezas[0][2] = SOLDADO_AZUL;

    // Bando rojo
    tableroPiezas[8][6] = SOLDADO_ROJO;
    tableroPiezas[8][7] = MAGO_ROJO;
    tableroPiezas[8][8] = SOLDADO_ROJO;
}

bool MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    int pieza = tableroPiezas[filaO][colO];

    int df = filaD - filaO;
    int dc = colD - colO;

    if (df == 0 && dc == 0)
        return false;

    int absDf = df < 0 ? -df : df;
    int absDc = dc < 0 ? -dc : dc;

    // Soldado: 1 casilla
    if (pieza == SOLDADO_AZUL || pieza == SOLDADO_ROJO)
    {
        if (absDf <= 1 && absDc <= 1)
            return true;
    }

    // Mago: hasta 2 casillas
    if (pieza == MAGO_AZUL || pieza == MAGO_ROJO)
    {
        if (absDf <= 2 && absDc <= 2)
            return true;
    }

    return false;
}

void DibujarMovimientosValidos()
{
    if (!piezaSeleccionada)
        return;

    for (int fila = 0; fila < TAM_TABLERO; fila++)
    {
        for (int col = 0; col < TAM_TABLERO; col++)
        {
            if (MovimientoValido(filaOrigen, colOrigen, fila, col))
            {
                int piezaOrigen = tableroPiezas[filaOrigen][colOrigen];
                int piezaDestino = tableroPiezas[fila][col];

                bool mismoBando = (EsAzul(piezaOrigen) && EsAzul(piezaDestino)) ||
                    (EsRoja(piezaOrigen) && EsRoja(piezaDestino));

                if (mismoBando)
                    continue;

                float x = (col - TAM_TABLERO / 2) * 2.0f;
                float z = (fila - TAM_TABLERO / 2) * 2.0f;

                glColor4f(0.0f, 0.8f, 0.2f, 0.35f);

                glBegin(GL_QUADS);
                glVertex3f(x, 0.03f, z);
                glVertex3f(x + 2.0f, 0.03f, z);
                glVertex3f(x + 2.0f, 0.03f, z + 2.0f);
                glVertex3f(x, 0.03f, z + 2.0f);
                glEnd();
            }
        }
    }
}