#include "logica.h"
#include "tablero.h"
#include "mundo.h"

Logica::Logica(Tablero* t)
{
    tablero = t;
}
void Logica::actualizarMovimiento() {
    if (tablero->cooldownMovimiento > 0) { tablero->cooldownMovimiento--; return; }
    bool seHaMovido = false;
    if (tablero->teclasPulsadas['w'] || tablero->teclasPulsadas['W']) { if (tablero->filaSeleccionada < Tablero::getTamTablero() - 1) { tablero->filaSeleccionada++; seHaMovido = true; } }
    if (tablero->teclasPulsadas['s'] || tablero->teclasPulsadas['S']) { if (tablero->filaSeleccionada > 0) { tablero->filaSeleccionada--; seHaMovido = true; } }
    if (tablero->teclasPulsadas['a'] || tablero->teclasPulsadas['A']) { if (tablero->colSeleccionada < Tablero::getTamTablero() - 1) { tablero->colSeleccionada++; seHaMovido = true; } }
    if (tablero->teclasPulsadas['d'] || tablero->teclasPulsadas['D']) { if (tablero->colSeleccionada > 0) { tablero->colSeleccionada--; seHaMovido = true; } }
    if (seHaMovido) tablero->cooldownMovimiento = 2;
}
void Logica::tecla(unsigned char key)
{
    tablero->teclasPulsadas[key] = true;
    actualizarMovimiento();
    if (tablero->seleccionandoHechizo) {
        if (key == 13 || key == ' ') {
            tablero->teclasPulsadas[key] = false;
            tablero->confirmarObjetivoHechizo();
            return;
        }
        if (key == 8 || key == 'q' || key == 'Q') {
            tablero->teclasPulsadas[key] = false;
            tablero->cancelarSeleccionHechizo();
            return;
        }
    }

    if (!tablero->seleccionandoHechizo) {
        if (key >= '1' && key <= '6') {
            int indiceHechizo = key - '1';
            tablero->lanzarHechizo(indiceHechizo);
            return;
        }

        if (key == ' ') {
            if (!tablero->piezaSeleccionada) {
                Pieza* p = tablero->casillas[tablero->filaSeleccionada][tablero->colSeleccionada];
                if (p != nullptr && p->GetBando() == tablero->turnoActual) {
                    if (p->estaEncarcelada) {
                        tablero->mensajeErrorHechizo = "UNIDAD CONGELADA EN CARBONITA";
                        tablero->timerMensajeError = 120;
                        return;
                    }
                    tablero->piezaSeleccionada = true;
                    tablero->filaOrigen = tablero->filaSeleccionada; tablero->colOrigen = tablero->colSeleccionada;
                }
            }
            else {
                Pieza* piezaOrigen = tablero->casillas[tablero->filaOrigen][tablero->colOrigen];
                Pieza* piezaDestino = tablero->casillas[tablero->filaSeleccionada][tablero->colSeleccionada];

                if (piezaDestino == nullptr) {
                    if (piezaOrigen->MovimientoValido(tablero->filaOrigen, tablero->colOrigen, tablero->filaSeleccionada, tablero->colSeleccionada)) {
                        tablero->casillas[tablero->filaSeleccionada][tablero->colSeleccionada] = piezaOrigen;
                        tablero->casillas[tablero->filaOrigen][tablero->colOrigen] = nullptr;
                        tablero->piezaSeleccionada = false;
                        tablero->actualizarTurnosPrision();
                        tablero->turnoActual = (tablero->turnoActual == 1) ? 2 : 1;
                    }
                }
                else if (piezaDestino->GetBando() != tablero->turnoActual) {
                    if (piezaOrigen->MovimientoValido(tablero->filaOrigen, tablero->colOrigen, tablero->filaSeleccionada, tablero->colSeleccionada)) {
                        mundo.arena.inicializa(piezaOrigen, piezaDestino, tablero->turnoActual);
                        estado = COMBATE;
                        tablero->piezaSeleccionada = false;
                    }
                }
                else if (tablero->filaSeleccionada == tablero->filaOrigen && tablero->colSeleccionada == tablero->colOrigen) {
                    tablero->piezaSeleccionada = false;
                }
            }
        }
    }
    glutPostRedisplay();

}

void Logica::teclaLiberada(unsigned char key)
{
    tablero->teclasPulsadas[key] = false;
}
