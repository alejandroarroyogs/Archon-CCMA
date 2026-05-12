#include "hechizos.h"
#include "mundo.h"
#include "piezas.h"
#include "tablero.h"
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

    std::cout << "[HECHIZO] desatando Alteracion Cronologica..." << std::endl;

    // Llamamos a la función de mundo que cambia el flujo del ciclo del tablero
    m.cambiaCiclo();

    _yaUsado = true;
    return true;
}

bool HechizoExchange::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;

    // Validación 1: El usuario debe hacer clic sobre una casilla con pieza
    if (p == nullptr) {
        std::cout << "[CONFUSIÓN MENTAL] Debes seleccionar una pieza valida." << std::endl;
        return false;
    }

    // FASE 1: Guardar la primera pieza
    if (_primeraPieza == nullptr) {
        _primeraPieza = p;
        std::cout << "[CONFUSIÓN MENTAL] Primera pieza fijada. Selecciona la segunda pieza para intercambiarlas." << std::endl;

        // Retornamos falso temporalmente porque el hechizo NO ha terminado completo,
        // esto evita que el tablero gaste el hechizo o pase el turno a la primera pulsación.
        return false;
    }

    // FASE 2: Tenemos la segunda pieza, realizamos el intercambio en la matriz de casillas
    if (_primeraPieza == p) {
        std::cout << "[CONFUSIÓN MENTAL] No puedes intercambiar una pieza consigo misma. Proceso reiniciado." << std::endl;
        _primeraPieza = nullptr;
        return false;
    }

    int f1 = -1, c1 = -1, f2 = -1, c2 = -1;
    int TAM = m.tablero.getTamTablero();

    // Buscamos las coordenadas de ambas piezas en el tablero actual
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (m.tablero.casillas[i][j] == _primeraPieza) { f1 = i; c1 = j; }
            if (m.tablero.casillas[i][j] == p) { f2 = i; c2 = j; }
        }
    }

    // Si por seguridad ambas existen en la matriz, hacemos el SWAP
    if (f1 != -1 && f2 != -1) {
        m.tablero.casillas[f1][c1] = p;
        m.tablero.casillas[f2][c2] = _primeraPieza;

        std::cout << "[HECHIZO] ¡Confusion Mental completada! Las unidades seleccionadas han intercambiado sus posiciones." << std::endl;

        _primeraPieza = nullptr; // Reseteamos el estado interno
        _yaUsado = true;         // Hechizo consumido con éxito
        return true;
    }

    _primeraPieza = nullptr;
    return false;
}

bool HechizoRevive::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;

    // 1. El objetivo de la casilla debe estar vacío para poder materializar la pieza
    if (p != nullptr) {
        std::cout << "[REVIVE] No puedes revivir a alguien en una casilla ocupada." << std::endl;
        return false;
    }

    int bandoLanzador = m.tablero.getTurno();
    Pieza* piezaMuerta = nullptr;

    // 2. Buscamos en la lista global de piezas del mundo una que sea de nuestro bando y esté muerta
    for (Pieza* pieza : m.listaPiezas) {
        if (pieza->GetBando() == bandoLanzador && !pieza->EstaViva()) {
            piezaMuerta = pieza;
            break; // Rescatamos la primera que encontremos
        }
    }

    if (piezaMuerta == nullptr) {
        std::cout << "[REVIVE] No tienes ninguna pieza destruida en tu cementerio." << std::endl;
        return false;
    }

    // 3. Resucitamos a la unidad: le restauramos la vida al máximo y la ponemos en las coordenadas elegidas
    piezaMuerta->SetVida(100);
    int fila = m.tablero.filaSeleccionada;
    int col = m.tablero.colSeleccionada;
    m.tablero.casillas[fila][col] = piezaMuerta;

    std::cout << "[HECHIZO] ¡Espiritu de la Fuerza invocado! Una unidad ha regresado al tablero en la casilla [" << fila << "][" << col << "]." << std::endl;

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