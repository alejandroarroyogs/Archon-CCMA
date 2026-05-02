#pragma once

class Pieza
{

    int bando; // 1 = azul, 2 = rojo
    int vida;
    int danio;
    friend class Arena;

public:
    Pieza(int b, int v, int d);
    virtual ~Pieza();

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
};

class Soldado : public Pieza
{
public:
    Soldado(int b);
    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

class Mago : public Pieza
{
public:
    Mago(int b);
    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

class Arquero : public Pieza
{
public:
    Arquero(int b);
    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

class Caballero : public Pieza
{
public:
    Caballero(int b);
    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

class Rey : public Pieza
{
public:
    Rey(int b);
    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

// Funciones auxiliares 
bool EsAzul(Pieza* pieza);
bool EsRoja(Pieza* pieza);
