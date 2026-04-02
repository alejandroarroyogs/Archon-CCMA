#include "../../freeglut.h"
#include "tablero.h"
#include "../piezas/piezas.h"

Pieza* tableroPiezas[TAM_TABLERO][TAM_TABLERO];

int filaSeleccionada = 0;
int colSeleccionada = 0;

bool piezaSeleccionada = false;
int filaOrigen = 0;
int colOrigen = 0;

int turnoActual = 1; // 1 = azul, 2 = rojo

bool enCombate = false;
Pieza* atacante = 0;
Pieza* defensor = 0;

int combateFilaOrigen = -1;
int combateColOrigen = -1;
int combateFilaDestino = -1;
int combateColDestino = -1;

int tiempoEntreRondas = 0;
bool combateResuelto = false;

bool combateIniciado = false;

int flashAtacante = 0;
int flashDefensor = 0;

void LiberarTablero()
{
    for (int i = 0; i < TAM_TABLERO; i++)
    {
        for (int j = 0; j < TAM_TABLERO; j++)
        {
            if (tableroPiezas[i][j] != 0)
            {
                delete tableroPiezas[i][j];
                tableroPiezas[i][j] = 0;
            }
        }
    }
}

void InicializarTablero()
{
    for (int i = 0; i < TAM_TABLERO; i++)
    {
        for (int j = 0; j < TAM_TABLERO; j++)
        {
            tableroPiezas[i][j] = 0;
        }
    }

    // AZUL
    tableroPiezas[0][0] = new Soldado(1);
    tableroPiezas[0][1] = new Mago(1);
    tableroPiezas[0][2] = new Arquero(1);
    tableroPiezas[0][3] = new Caballero(1);
    tableroPiezas[0][4] = new Rey(1);

    // ROJO
    tableroPiezas[8][4] = new Rey(2);
    tableroPiezas[8][5] = new Caballero(2);
    tableroPiezas[8][6] = new Arquero(2);
    tableroPiezas[8][7] = new Mago(2);
    tableroPiezas[8][8] = new Soldado(2);
}

bool MovimientoValido(int filaO, int colO, int filaD, int colD)
{
    if (filaO < 0 || filaO >= TAM_TABLERO || colO < 0 || colO >= TAM_TABLERO)
        return false;

    if (filaD < 0 || filaD >= TAM_TABLERO || colD < 0 || colD >= TAM_TABLERO)
        return false;

    Pieza* pieza = tableroPiezas[filaO][colO];

    if (pieza == 0)
        return false;

    return pieza->MovimientoValido(filaO, colO, filaD, colD);
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
                Pieza* piezaOrigen = tableroPiezas[filaOrigen][colOrigen];
                Pieza* piezaDestino = tableroPiezas[fila][col];

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

void ResolverCombate(bool ganaAtacante)
{
    if (!enCombate)
        return;

    if (ganaAtacante)
    {
        if (tableroPiezas[combateFilaDestino][combateColDestino] != 0)
        {
            delete tableroPiezas[combateFilaDestino][combateColDestino];
            tableroPiezas[combateFilaDestino][combateColDestino] = 0;
        }

        tableroPiezas[combateFilaDestino][combateColDestino] = tableroPiezas[combateFilaOrigen][combateColOrigen];
        tableroPiezas[combateFilaOrigen][combateColOrigen] = 0;
    }
    else
    {
        if (tableroPiezas[combateFilaOrigen][combateColOrigen] != 0)
        {
            delete tableroPiezas[combateFilaOrigen][combateColOrigen];
            tableroPiezas[combateFilaOrigen][combateColOrigen] = 0;
        }
    }

    piezaSeleccionada = false;

    if (turnoActual == 1)
        turnoActual = 2;
    else
        turnoActual = 1;

    enCombate = false;
    atacante = 0;
    defensor = 0;

    combateFilaOrigen = -1;
    combateColOrigen = -1;
    combateFilaDestino = -1;
    combateColDestino = -1;

    tiempoEntreRondas = 0;
    combateResuelto = false;

    combateIniciado = false;

    flashAtacante = 0;
    flashDefensor = 0;
}

void IniciarCombateAutomatico()
{
    if (!enCombate)
        return;

    if (atacante == 0 || defensor == 0)
        return;

    tiempoEntreRondas = 20;
    combateResuelto = false;
    combateIniciado = true;
}

void EjecutarRondaCombate()
{
    if (!enCombate)
        return;

    if (atacante == 0 || defensor == 0)
        return;

    if (combateResuelto)
        return;

    defensor->RecibirDanio(atacante->GetDanio());
    flashDefensor = 8;

    if (!defensor->EstaViva())
    {
        combateResuelto = true;
        ResolverCombate(true);
        return;
    }

    atacante->RecibirDanio(defensor->GetDanio());
    flashAtacante = 8;

    if (!atacante->EstaViva())
    {
        combateResuelto = true;
        ResolverCombate(false);
        return;
    }

    tiempoEntreRondas = 20;
}

void DibujaTablero()
{
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
}

void ResolverCombateAutomatico()
{
    IniciarCombateAutomatico();
}