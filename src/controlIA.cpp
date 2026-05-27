#include "controlIA.h"
#include "piezas.h"
#include "mundo.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "tablero.h"

class Mundo;
using namespace std;

bool ControlIA::PtoPoder(Tablero& tablero, int f, int c) {

    int tam = Tablero::getTamTablero();
    // Recorremos el tablero buscando si la coordenada (f, c) cumple el patrón de punto de poder (tipo 2)
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            // ptos poder
            bool condicionPoder = ((i == 4 && (j == 0 || j == 4 || j == 8)) ||
                (j == 4 && (i == 0 || i == 4 || i == 8)));
            // Si la casilla que estamos buscando coincide con una posición del patrón
            if (i == f && j == c && condicionPoder) {
                return true;
            }
        }
    }
    return false;
}

bool ControlIA::rivalCombate(Tablero& tablero, int fD, int cD, int miBando) {
    int bandoRival = (miBando == 2) ? 1 : 2;
    int tam = Tablero::getTamTablero();
    for (int rF = 0; rF < tam; rF++) {
        for (int rC = 0; rC < tam; rC++) {
            Pieza* pRival = tablero.casillas[rF][rC];
            if (pRival != nullptr && pRival->GetBando() == bandoRival) {
                // Si el rival se puede mover a donde queremos ir, estamos expuestos
                if (pRival->MovimientoValido(rF, rC, fD, cD)) {
                    return true;
                }
            }
        }
    }
    return false;
}
void ControlIA::ejecutarturno(Tablero& tablero, Dificultad nivel)
{
    int miBando = 2; // Bando de la IA
    vector<Mov> listaPuntosPoder;
    vector<Mov> listaCombatesSeguros;
    vector<Mov> listaMovimientosEstandar;
    //recorrer tablero y piezas
    int tam = Tablero::getTamTablero();
    for (int f = 0; f < tam; f++) {
        for (int c = 0; c < tam; c++) {
            Pieza* p = tablero.casillas[f][c];

            // Filtramos las piezas que pertenecen a la IA
            if (p != nullptr && p->GetBando() == miBando) {

                for (int df = 0; df < tam; df++) {
                    for (int dc = 0; dc < tam; dc++) {

                        if (p->MovimientoValido(f, c, df, dc)) {
                            if (!p->EsVoladora() && !tablero.CaminoLibre(f, c, df, dc)) continue;
                            Pieza* objetivo = tablero.casillas[df][dc];

                            Mov m;
                            m.f0 = f; m.c0 = c; m.fD = df; m.cD = dc; m.punt = 0;

                            // Primera prioridad: OCUPAR PTO PODER
                            if (PtoPoder(tablero, df, dc) && objetivo == nullptr) {
                                // SI MODO DIFICIL=>se mira si la otra pieza nos puede comer
                                if (nivel == DIFICIL && rivalCombate(tablero, df, dc, miBando)) {
                                    m.punt = 10;
                                }
                                else {
                                    m.punt = 1000; // Prioridad máxima
                                }
                                listaPuntosPoder.push_back(m);
                            }

                            // Segunda prioridad: ir a combate según vida
                            else if (objetivo != nullptr && objetivo->GetBando() == 1) {
                                int miVida = p->GetVida();
                                int vidaRival = objetivo->GetVida();
                                int diferenciaVida = miVida - vidaRival;

                                //Diferentes umbrales según la dificultad
                                int umbralRequerido = 40;
                                if (nivel == FACIL)   umbralRequerido = -30;
                                if (nivel == MEDIO)   umbralRequerido = 10; 
                                if (nivel == DIFICIL) umbralRequerido = 40;

                                if (diferenciaVida >= umbralRequerido) {
                                    m.punt = 500 + diferenciaVida;
                                    listaCombatesSeguros.push_back(m);
                                }
                            }

                            //Apertura 
                            else if (objetivo == nullptr) {
                                //prioridad ir haia el centro
                                int posicionamiento = (4 - abs(df - 4)) + (4 - abs(dc - 4));
                                m.punt = posicionamiento;
                                listaMovimientosEstandar.push_back(m);
                            }
                        }
                    }
                }
            }
        }
    }

    //decisión segun prioridad
    Mov jugadaFinal;
    bool jugadaEncontrada = false;

    //PTOS PODER
    if (!listaPuntosPoder.empty()) {
        sort(listaPuntosPoder.begin(), listaPuntosPoder.end(), [](const Mov& a, const Mov& b) { return a.punt > b.punt; });
        if (listaPuntosPoder[0].punt > 50) {
            jugadaFinal = listaPuntosPoder[0];
            jugadaEncontrada = true;
            cout << "IA: Prioridad 1 -> Ocupando punto de poder dorado." << endl;
        }
    }

    //GARANTIA DE VIDA EN EL COMBATE
    if (!jugadaEncontrada && !listaCombatesSeguros.empty()) {
        sort(listaCombatesSeguros.begin(), listaCombatesSeguros.end(), [](const Mov& a, const Mov& b) { return a.punt > b.punt; });
        jugadaFinal = listaCombatesSeguros[0];
        jugadaEncontrada = true;
        cout << "IA: Prioridad 2 -> Entrando en combate ventajoso (Diferencia HP favorable)." << endl;
    }

    //AVANCE INICIAL
    if (!jugadaEncontrada && !listaMovimientosEstandar.empty()) {
        sort(listaMovimientosEstandar.begin(), listaMovimientosEstandar.end(), [](const Mov& a, const Mov& b) { return a.punt > b.punt; });

        // Simulación de error humano en modo Fácil
        if (nivel == FACIL && listaMovimientosEstandar.size() > 2 && (rand() % 10 < 4)) {
            jugadaFinal = listaMovimientosEstandar[rand() % 3];
        }
        else {
            jugadaFinal = listaMovimientosEstandar[0]; // El mejor avance posicional
        }
        jugadaEncontrada = true;
        cout << "IA: Prioridad 3 -> Avanzando posiciones hacia el frente." << endl;
    }

    //MOVER PIEZA
    if (jugadaEncontrada) {
        Pieza* pOrigen = tablero.casillas[jugadaFinal.f0][jugadaFinal.c0];
        Pieza* pDestino = tablero.casillas[jugadaFinal.fD][jugadaFinal.cD];

        if (pDestino != nullptr && pDestino->GetBando() != miBando) {
            // Hay enemigo en el destino: lanzar combate en la Arena
            tablero.filaOrigen = jugadaFinal.f0;
            tablero.colOrigen = jugadaFinal.c0;
            tablero.filaSeleccionada = jugadaFinal.fD;
            tablero.colSeleccionada = jugadaFinal.cD;

            extern Mundo mundo;
            mundo.arena.inicializa(pOrigen, pDestino, miBando);
            extern Estado estado;
            estado = COMBATE;
            cout << "IA: Entrando en la Arena de combate." << endl;
        }
        else {
            // Casilla vacía: movimiento simple
            tablero.casillas[jugadaFinal.fD][jugadaFinal.cD] = pOrigen;
            tablero.casillas[jugadaFinal.f0][jugadaFinal.c0] = nullptr;
        }
    }
    else {
        cout << "IA: No se han encontrado movimientos viables esta ronda." << endl;
    }
}