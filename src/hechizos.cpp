#include "hechizos.h"
#include "mundo.h" //Cuando este programada la estructuración de los menus
//Cuando esten las piezas

bool HechizoTeleport::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado || !p) return false;
    _yaUsado = true;
    return true;
}

bool HechizoHeal::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado || !p) return false;
    p->SetVida(100); 
    _yaUsado = true;
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
    // Requiere lógica de selección de dos piezas
    _yaUsado = true;
    return true;
}

bool HechizoSummon::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;
    // Invoca unidad temporal para combate 
    _yaUsado = true;
    return true;
}

bool HechizoRevive::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado) return false;
    // Resucita pieza aliada junto al hechicero 
    _yaUsado = true;
    return true;
}

bool HechizoImprison::aplica(Mundo& m, Pieza* p) {
    if (_yaUsado || !p) return false;
    // Bloquea movimiento según ciclos de color
    _yaUsado = true;
    return true;
}
