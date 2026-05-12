#pragma once
#include <string>
#include <vector>
#include "piezas.h"

class Mundo;

class Hechizo {
protected:
    std::string _nombre;
    bool _yaUsado; // Cada hechizo se usa una sola vez
public:
    Hechizo(std::string nombre) : _nombre(nombre), _yaUsado(false) {}
    virtual ~Hechizo() {}

    // Polimorfismo puro
    virtual bool aplica(Mundo& mundo, Pieza* objetivo = nullptr) = 0;

    std::string getNombre() const { return _nombre; }
    bool estaUsado() const { return _yaUsado; }
    void setUsado(bool usado) { _yaUsado = usado; }
};

class HechizoTeleport : public Hechizo {
public:
    HechizoTeleport() : Hechizo("Salto Hiperespacial") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoHeal : public Hechizo {
public:
    HechizoHeal() : Hechizo("Curacion de la Fuerza") {}
    bool aplica(Mundo& m, Pieza* p) override;
};

class HechizoShiftTime : public Hechizo {
public:
    HechizoShiftTime() : Hechizo("Alteracion Cronologica") {}
    bool aplica(Mundo& m, Pieza* p = nullptr) override;
};

class HechizoExchange : public Hechizo {
private:
    Pieza* _primeraPieza;
public:
    HechizoExchange() : Hechizo("Confusion Mental"), _primeraPieza(nullptr) {}
    bool aplica(Mundo& m, Pieza* p) override;
    void reset() { _primeraPieza = nullptr; }
};

class HechizoRevive : public Hechizo {
public:
    HechizoRevive() : Hechizo("Espiritu de la Fuerza") {}
    bool aplica(Mundo& m, Pieza* p = nullptr) override;
};

class HechizoImprison : public Hechizo {
public:
    HechizoImprison() : Hechizo("Bloqueo Carbonita") {}
    bool aplica(Mundo& m, Pieza* p) override;
};