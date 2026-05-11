#include "hechizos.h"
#include "mundo.h"
#include <iostream>

bool HechizoTeleport::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) {
        std::cout << "[TELEPORT] Este poder ya ha sido agotado." << std::endl;
        return false;
    }

    // El movimiento y comprobaciones del Salto Hiperespacial se resuelven en el Tablero.
    // Solo marcamos el uso del hechizo.
    _yaUsado = true;
    return true;
}

bool HechizoHeal::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) {
        std::cout << "[CURACIÓN] Este hechizo ya ha sido utilizado." << std::endl;
        return false;
    }

    // 1. Validar que haya un objetivo bajo el cursor
    if (p == nullptr) {
        std::cout << "[CURACIÓN] No hay ninguna pieza en esa casilla para curar." << std::endl;
        return false;
    }

    // 2. Obtener el bando del jugador del turno actual de forma segura
    int bandoLanzador = m.tablero.turnoActual;

    // 3. Comprobar que la pieza objetivo pertenece a tu bando (es aliada)
    if (p->GetBando() != bandoLanzador) {
        std::cout << "[CURACIÓN] ¡No puedes curar a una pieza enemiga!" << std::endl;
        return false;
    }

    // 4. Aplicar la curación restaurando la salud al máximo (100)
    p->SetVida(100);
    _yaUsado = true;

    std::cout << "[CURACIÓN] ¡Hechizo ejecutado con éxito! Unidad aliada curada al 100%." << std::endl;
    return true;
}

bool HechizoShiftTime::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;
    m.cambiaCiclo();
    _yaUsado = true;
    return true;
}

bool HechizoExchange::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;
    _yaUsado = true;
    return true;
}

bool HechizoSummon::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;
    _yaUsado = true;
    return true;
}

bool HechizoRevive::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;
    _yaUsado = true;
    return true;
}

bool HechizoImprison::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) {
        std::cout << "[CARBONITA] Este poder ya ha sido agotado." << std::endl;
        return false;
    }

    // 1. Validar que haya un objetivo bajo el cursor
    if (p == nullptr) {
        std::cout << "[CARBONITA] No hay ninguna pieza en esa casilla para encarcelar." << std::endl;
        return false;
    }

    // 2. Comprobar que la pieza objetivo pertenece al bando rival (enemiga)
    int bandoLanzador = m.tablero.turnoActual;
    if (p->GetBando() == bandoLanzador) {
        std::cout << "[CARBONITA] ¡No puedes congelar a tus propias unidades!" << std::endl;
        return false;
    }

    // 3. Si ya está congelada, evitamos malgastar el hechizo
    if (p->estaEncarcelada) {
        std::cout << "[CARBONITA] Esta unidad ya se encuentra congelada en carbonita." << std::endl;
        return false;
    }

    // 4. Encarcelar a la pieza enemiga por 3 turnos globales del tablero
    p->encarcelar(3);
    _yaUsado = true;

    std::cout << "[CARBONITA] ¡La unidad enemiga ha sido congelada en bloques de carbonita!" << std::endl;
    return true;
}