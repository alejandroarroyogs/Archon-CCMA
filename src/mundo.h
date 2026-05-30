#pragma once
#include "arena.h"
#include "freeglut.h"
#include "tablero.h"
#include "piezas.h"
#include "interfaz.h"
#include "jugador.h" 
#include "fin.h"
#include <string>
#include <vector>
#include "logica.h"

// Estado del juego
enum Estado { MENU, JUGANDO, COMBATE, INSTRUC, GAMEOVER, SELEC_MODO, SELEC_DIFICULTAD };
extern Estado estado;
extern int modoJuego;

struct RegistroPartida {
    std::string ganador;
    float duracion;
};


class Mundo
{
    jugador* j1;
    jugador* j2;
    bool musicaCombateLanzada = false;
    std::vector<RegistroPartida*> historial;
    float cronometro;
    bool FinJuego;
    int bandoGanador = 0;

public:
    Mundo();
    virtual ~Mundo();
    void Inicializar();
    void Dibujar();
    Interfaz interfaz;
    Tablero tablero;
    Arena arena;
    Fin fin;
    Logica logica;
    std::vector<Pieza*>listaPiezas;
    
    void tecla(unsigned char key);
    void teclaLiberada(unsigned char key);
    void Timer(int value);
    void inicializarPartida();
    void cambiaCiclo();
    void turnoIA();

    //MEMORIA
    void registrarFinPartida(std::string ganador);
    void limpiarMemoriaDinamica();
};

