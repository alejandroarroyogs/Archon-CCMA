#pragma once

class jugador {
private:
    int bando;      // 1 = Bando de la Luz, 2 = Bando Oscuro
    bool es_ia;     // true si el jugador es la máquina, false si es humano

public:
    // Constructor: pasamos el bando y si es manejado por IA
    jugador(int b, bool ia);

    // Destructor
    virtual ~jugador();

    // Funciones para obtener la información (Getters)
    int getBando();
    bool esIA();
};
