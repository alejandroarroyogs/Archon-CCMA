#pragma once
#include "modeloobj.h"

class Pieza {

    int bando; // 1 = azul, 2 = rojo
    int vida;
    int vidaMax;
    int danio;
    friend class Arena;
    friend class Hechizo;

    int ataque;
    int armadura;


public:
    Pieza(int b, int v, int atq, int arm);
    virtual ~Pieza();



    int GetBando();
    int GetVida();
    int GetVidaMax() { return vidaMax; } 
    int GetDanio();

    int GetAtaque() { return ataque; }
    int GetArmadura() { return armadura; }

    void SetVida(int v);
    void RecibirDanio(int d);

    bool EstaViva();
    bool EsAzul();
    bool EsRoja();

    virtual bool MovimientoValido(int filaO, int colO, int filaD, int colD) = 0;
    virtual bool EsVoladora() const { return false; }
    virtual void Dibujar(float x, float z) = 0;
    virtual void DibujarCombate(float x, float z, bool flash) = 0;

    void recuperarVida(int cantidad);

    // --- Estado de Bloqueo Carbonita ---
    bool estaEncarcelada;
    int turnosPrision;     // Contador de turnos restantes en carbonita

    // Funciones auxiliares
    void encarcelar(int turnos) { estaEncarcelada = true; turnosPrision = turnos; }
    void reducirCondena() {
        if (turnosPrision > 0) {
            turnosPrision--;
            if (turnosPrision == 0) estaEncarcelada = false;
        }
    }
};

bool EsAzul(Pieza* pieza);
bool EsRoja(Pieza* pieza);

