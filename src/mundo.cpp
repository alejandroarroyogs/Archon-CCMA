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
#include <fstream>
#include <iomanip>

Estado estado = MENU;
int modoJuego = 0;
bool combateFinalizado = false;

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
    limpiarMemoriaDinamica();
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
    case SELEC_DIFICULTAD:
        interfaz.eligeDificultad();
        break;
    case COMBATE:
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
        // Tu timer acumula el tiempo real de juego (25ms = 0.025 segundos)
        cronometro += 0.025f;

        if (combateFinalizado) {
            tablero.avanzarTurno();
            combateFinalizado = false;
        }
        tablero.actualizarMovimiento();

        if (modoJuego == 1 && tablero.turnoActual == 2 && j2->esIA()) {
            static int contadorEspera = 0;
            contadorEspera++;
            if (contadorEspera > 30) {
                turnoIA();
                contadorEspera = 0;
                tablero.turnoActual = 1;
            }
        }
    }
    else if (estado == COMBATE) {
        arena.actualiza();
    }

    glutPostRedisplay();
}

void Mundo::inicializarPartida()
{
    cronometro = 0.0f;

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
    tablero.turnoGlobal += 2;
}

void Mundo::turnoIA()
{
    ControlIA::ejecutarturno(tablero, interfaz.nivel());
}

void Mundo::registrarFinPartida(std::string ganador)
{
    RegistroPartida* nuevaPartida = new RegistroPartida();
    nuevaPartida->ganador = ganador;
    nuevaPartida->duracion = cronometro;
    historial.push_back(nuevaPartida);

    // escritura
    std::ofstream archivoOut("registro_partidas.txt", std::ios::app);
    if (archivoOut.is_open()) {
        archivoOut << "GANADOR: " << nuevaPartida->ganador
            << " | DURACION: " << std::fixed << std::setprecision(1)
            << nuevaPartida->duracion<< " segundos.\n";
        archivoOut.close();
    }

    // lectura
    std::ifstream archivoIn("registro_partidas.txt");
    std::string linea;
    int victoriasFuerza = 0;
    int victoriasSith = 0;

    while (std::getline(archivoIn, linea)) {
        if (linea.find("GANADOR: JEDI") != std::string::npos) {
            victoriasFuerza++;
        }
        if (linea.find("GANADOR: SITH") != std::string::npos) {
            victoriasSith++;
        }
    }
    archivoIn.close();

    //impresión
    std::cout << "\n==================================================" << std::endl;
    std::cout << "          HISTORIAL GLOBAL DE VICTORIAS           " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "  VICTORIAS DE LA FUERZA (JEDI): " << victoriasFuerza << std::endl;
    std::cout << "  VICTORIAS DEL IMPERIO  (SITH): " << victoriasSith << std::endl;
    std::cout << "==================================================\n" << std::endl;
}
void Mundo::limpiarMemoriaDinamica()
{
    // Liberación estricta de la memoria dinámica para evitar leaks
    for (size_t i = 0; i < historial.size(); ++i) {
        delete historial[i];
    }
    historial.clear();
}
