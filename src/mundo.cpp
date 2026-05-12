#include "mundo.h"
#include <iostream>
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "hechizos.h"
#include "arena.h"
#include "jugador.h"
#include "controlIA.h"
#include "fin.h"

Estado estado = JUGANDO;
int modoJuego = 0;

Mundo::Mundo() {
    j1 = 0;
    j2 = 0;
  
}

Mundo::~Mundo() {

    if (j1) delete j1;
    if (j2) delete j2;

    for (Pieza* p : listaPiezas) {
        delete p;
    }
    listaPiezas.clear();
}

void Mundo::Inicializar() {
    inicializarPartida();
    
}

void Mundo::Dibujar()
{
    switch (estado) {
    case MENU:
        interfaz.dibujaMenu();
        break;
    case INSTRUC:
        interfaz.dibujaInstrucciones();
        break;
    case SELEC_MODO:
        interfaz.eligeModo();
        break;
    case COMBATE:
        // Si entramos en combate y la música no está sonando, la encendemos
        if (!musicaCombateLanzada) {
            arena.ponMusica();
            musicaCombateLanzada = true;
        }
        arena.dibuja();
        break;
    case JUGANDO:
        musicaCombateLanzada = false;
        tablero.dibuja();
        break;
    case GAMEOVER:
        if (!musicaCombateLanzada) {
            fin.ponMusica();
            musicaCombateLanzada = true;
        }
        fin.dibuja();
        break;
    }
}

void Mundo::tecla(unsigned char key)
{
    switch (estado) {
    case JUGANDO:
        // Verificamos si el jugador actual es humano antes de pasar la tecla
        if ((tablero.turnoActual == 1 && j1 != 0 && !j1->esIA()) ||
            (tablero.turnoActual == 2 && j2 != 0 && !j2->esIA()))
        {
            // Ahora tablero.tecla ya gestiona internamente los hechizos (1-9)
            tablero.tecla(key);
        }
        break;

    case COMBATE:
        arena.setTurno(tablero.turnoActual);
        break;
    }
}
void Mundo::teclaLiberada(unsigned char key)
{
    if (estado == JUGANDO) {
        tablero.teclaLiberada(key);
    }
}

void Mundo::Timer(int value)
{
    if (estado == JUGANDO) {
        tablero.logica.actualizarMovimiento();

        // Si es Modo 1 Jugador Y es el turno del bando 2 Y ese jugador es IA
        if (modoJuego == 1 && tablero.turnoActual == 2 && j2->esIA()) {
            static int contadorEspera = 0;
            contadorEspera++;

            if (contadorEspera > 30) { // Espera medio segundo aprox
                turnoIA();
                contadorEspera = 0;
                tablero.turnoActual = 1; // Devolvemos el turno al humano
            }
        }
    }
    glutPostRedisplay();


}

void Mundo::inicializarPartida()
{

    if (j1) { delete j1; j1 = nullptr; }
    if (j2) { delete j2; j2 = nullptr; }
    //después del menú, se crea la partida
    //crear los jugadoressss
    //el primero siempre persona
    j1 = new jugador(1, false);
    //segundo ia o persona según modo juego
    if (modoJuego == 1) {
        j2 = new jugador(2, true);
    }
    else {
        j2 = new jugador(2, false);
    }

    tablero.inicializa();
}


void Mundo::cambiaCiclo()
{
    tablero.turnoActual = (tablero.turnoActual == 1) ? 2 : 1;
}

void Mundo::turnoIA()
{
    ControlIA::ejecutarturno(tablero);
}
