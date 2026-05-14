#pragma once
#include "modeloobj.h"

class Pieza {

    int bando; // 1 = azul, 2 = rojo
    int vida;
    int vidaMax;
    int danio;
    friend class Arena;
    friend class Hechizo;


public:
    Pieza(int b, int v, int d);
    virtual ~Pieza();

   // Pieza(int v) : bando(1), vida(v), vidaMax(v), danio(0) {}

    int GetBando();
    int GetVida();
    int GetDanio();

    void SetVida(int v);
    void RecibirDanio(int d);

    bool EstaViva();
    bool EsAzul();
    bool EsRoja();

    virtual bool MovimientoValido(int filaO, int colO, int filaD, int colD) = 0;
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

/*
//Jedi/sith  7 por equipo
class Jedi : public Pieza
{
    ModeloOBJ modelo;

public:
    Jedi(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Baby yoda  1 en los buenos
class BabyYoda : public Pieza
{
    ModeloOBJ modelo;
public:
    BabyYoda(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Darth vader  1 en los  malos
class DarthVader : public Pieza
{
    ModeloOBJ modelo;
public:
    DarthVader(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Caballero jedi/sith 2 por equipo
class CaballeroJedi : public Pieza
{
    ModeloOBJ modelo;
public:
    CaballeroJedi(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};


//Tirador 2 por equipo
class Tirador : public Pieza
{
    ModeloOBJ modelo;

public:
    Tirador(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};



//Skywalker/ kylo ren  2 por equipo
class Skywalker : public Pieza
{
    ModeloOBJ modelo;
public:
    Skywalker(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Drones 2 por equipo

class Drone : public Pieza
{
    ModeloOBJ modelo;

public:
    Drone(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Chewbacca/ Jabba the hut  2 por equipo

class Chewbacca : public Pieza
{
    ModeloOBJ modelo;
public:
    Chewbacca(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

// Funciones auxiliares 
bool EsAzul(Pieza* pieza);
bool EsRoja(Pieza* pieza);
*/