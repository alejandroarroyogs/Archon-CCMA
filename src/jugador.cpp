#include "jugador.h"

// Inicializamos el jugador con los datos que le pasemos al crearlo
jugador::jugador(int b, bool ia) {
    bando = b;
    es_ia = ia;
}

// El destructor está vacío porque no usamos punteros dinámicos ('new') dentro de esta clase
jugador::~jugador() {
}

// Devuelve el número de bando del jugador
int jugador::getBando() {
    return bando;
}

// Devuelve verdadero (true) si es la máquina, falso (false) si es humano
bool jugador::esIA() {
    return es_ia;
}