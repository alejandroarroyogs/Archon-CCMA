#ifndef _HECHIZOS_H
#define _HECHIZOS_H

#include <string>
#include <vector>

class Mundo; 
class Pieza;

/**
 * Clase Base Abstracta Hechizo
 */
class Hechizo {
protected:
    std::string _nombre;
    bool _yaUsado; // Cada hechizo se usa una sola vez
public:
    Hechizo(std::string nombre) : _nombre(nombre), _yaUsado(false) {}
    virtual ~Hechizo() {}

    //Polimorfismo
    virtual bool aplica(Mundo& mundo, Pieza* objetivo = nullptr) = 0;


    std::string getNombre() const { return _nombre; }
    bool getUsado() const { return _yaUsado; }
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

class HechizoShiftTime : public Hechizo { // Alteración Cronológica [cite: 168]
public:
    HechizoShiftTime() : Hechizo("Alteracion Cronologica") {}
    bool aplica(Mundo& m, Pieza* p) override;
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

#endif
