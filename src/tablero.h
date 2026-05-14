#pragma once
#include "piezas.h" 
#include "hechizos.h"
#include <vector>
#include <string>

extern class Mundo mundo;

class Tablero {
    static const int TAM_TABLERO = 9;
    bool teclasPulsadas[256];
    int cooldownMovimiento;

    // --- Gestión de Hechizos ---
    std::vector<Hechizo*> hechizosAzules;
    std::vector<Hechizo*> hechizosRojos;

    friend class ControlIA;
    friend class Logica;

    // Métodos auxiliares privados para la lógica interna de los hechizos
    bool liderEstaVivo(int bando);
    void confirmarObjetivoHechizo();
    void cancelarSeleccionHechizo();
    void actualizarTurnosPrision();

    // Mensajes de error en pantalla
    std::string mensajeErrorHechizo;
    int timerMensajeError;

public:
    Pieza* casillas[TAM_TABLERO][TAM_TABLERO];
    Tablero();
    virtual ~Tablero();

    void inicializa();
    void dibuja();
    void dibujaInterfazHechizos();

    void tecla(unsigned char key);
    void teclaLiberada(unsigned char key);
    void actualizarMovimiento();
    void dibujaBarraVida(float x, float z, int vidaActual, int vidaMax);

    // Lógica de lanzamiento y selección
    void lanzarHechizo(int indice);

    // Variables de estado de hechizos
    bool seleccionandoHechizo;        // True si el jugador está moviendo el selector de hechizo
    int indiceHechizoSeleccionado;    // Guarda el índice (0 a 6) del hechizo activado

    // --- Estado para el Hechizo de Teletransporte ---
    bool faseTeleportDestino;          // False = Seleccionando pieza aliada, True = Seleccionando destino vacío
    int filaTeleportOrigen;            // Guarda la fila de la pieza aliada seleccionada
    int colTeleportOrigen;             // Guarda la columna de la pieza aliada seleccionada

    // --- Estado para el Hechizo de Intercambio  ---
    bool faseExchangeSegunda;          // False = Seleccionando primera pieza, True = Seleccionando segunda pieza
    int filaExchangeOrigen;            // Guarda la fila de la primera pieza seleccionada
    int colExchangeOrigen;             // Guarda la columna de la primera pieza seleccionada

    static int getTamTablero() { return TAM_TABLERO; }
    int filaSeleccionada, colSeleccionada;
    bool piezaSeleccionada;
    int filaOrigen, colOrigen;
    int turnoActual;

    // Movimiento general
    int getTurno() { return turnoActual; }
    Pieza* getPiezaEnCursor();
};