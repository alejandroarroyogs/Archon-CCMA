#pragma once
#include <string>
#include <vector>
#include "piezas.h"


class Mundo;

class Hechizo {
protected:
    std::string _nombre;
    bool _yaUsado;

public:
    Hechizo(std::string nombre) : _nombre(nombre), _yaUsado(false) {}
    virtual ~Hechizo() {}

    virtual bool aplica(Mundo& m, Pieza* p) = 0; // Método virtual puro

    std::string getNombre() { return _nombre; }
    bool estaUsado() { return _yaUsado; }
};

class HechizoTeleport : public Hechizo { 
public:
    HechizoTeleport() : Hechizo("Salto Hiperespacial") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoHeal : public Hechizo {
public:
    HechizoHeal() : Hechizo("Curacion de la Fuerza") {}
    virtual bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoExchange : public Hechizo { // Confusión Mental [cite: 169]
public:
    HechizoExchange() : Hechizo("Confusion Mental") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoSummon : public Hechizo { // Llamada a la Flota [cite: 170]
public:
    HechizoSummon() : Hechizo("Llamada a la Flota") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoRevive : public Hechizo { // Espíritu de la Fuerza [cite: 171]
public:
    HechizoRevive() : Hechizo("Espiritu de la Fuerza") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoImprison : public Hechizo { // Bloqueo Carbonita [cite: 172]
public:
    HechizoImprison() : Hechizo("Bloqueo Carbonita") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

