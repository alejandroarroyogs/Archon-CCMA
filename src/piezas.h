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

//Jedi/sith  7 por equipo
class Jedi : public Pieza
{
public:
    Jedi(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Baby yoda  1 en los buenos
class BabyYoda : public Pieza
{
public:
    BabyYoda(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Darth vader  1 en los  malos
class DarthVader : public Pieza
{
public:
    DarthVader(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Caballero jedi/sith 2 por equipo
class CaballeroJedi : public Pieza
{
public:
    CaballeroJedi(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};


//Tirador 2 por equipo
class Tirador : public Pieza
{
public:
    Tirador(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};



//Skywalker/ kylo ren  2 por equipo
class Skywalker : public Pieza
{
public:
    Skywalker(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Drones 2 por equipo

class Drone : public Pieza
{
public:
    Drone(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

//Chewbacca/ Jabba the hut  2 por equipo

class Chewbacca : public Pieza
{
public:
    Chewbacca(int b);

    bool MovimientoValido(int filaO, int colO, int filaD, int colD);
    void Dibujar(float x, float z);
    void DibujarCombate(float x, float z, bool flash);
};

// Funciones auxiliares 
bool EsAzul(Pieza* pieza);
bool EsRoja(Pieza* pieza);
