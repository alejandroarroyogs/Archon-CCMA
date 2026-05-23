#include "controlIA.h"
#include "piezas.h"
#include "mundo.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

class Mundo;
using namespace std;

bool ControlIA::PtoPoder(Tablero& tablero, int f, int c) {
    // Recorremos el tablero buscando si la coordenada (f, c) cumple el patrón de punto de poder (tipo 2)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // El patrón exacto de tu tipoMapa para el valor 2:
            // Centro (4,4), esquinas de la cruz central: (0,4), (8,4), (4,0), (4,8)
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

    for (int rF = 0; rF < 9; rF++) {
        for (int rC = 0; rC < 9; rC++) {
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

    // ==========================================
    // 1. ESCANEO DEL TABLERO Y RECOPILACIÓN
    // ==========================================
    for (int f = 0; f < 9; f++) {
        for (int c = 0; c < 9; c++) {
            Pieza* p = tablero.casillas[f][c];

            // Filtramos las piezas que pertenecen a la IA
            if (p != nullptr && p->GetBando() == miBando) {

                for (int df = 0; df < 9; df++) {
                    for (int dc = 0; dc < 9; dc++) {

                        if (p->MovimientoValido(f, c, df, dc)) {
                            Pieza* objetivo = tablero.casillas[df][dc];

                            Mov m;
                            m.f0 = f; m.c0 = c; m.fD = df; m.cD = dc; m.punt = 0;

                            // ---- CATEGORÍA A: OCUPAR PUNTO DE PODER LIBRE ----
                            if (PtoPoder(tablero, df, dc) && objetivo == nullptr) {
                                // Previsión en Difícil: si es una trampa, bajamos la prioridad
                                if (nivel == DIFICIL && rivalCombate(tablero, df, dc, miBando)) {
                                    m.punt = 10;
                                }
                                else {
                                    m.punt = 1000; // Prioridad máxima
                                }
                                listaPuntosPoder.push_back(m);
                            }

                            // ---- CATEGORÍA B: EVALUACIÓN DE COMBATES SEGÚN LA VIDA ----
                            else if (objetivo != nullptr && objetivo->GetBando() == 1) {
                                int miVida = p->GetVida();
                                int vidaRival = objetivo->GetVida();
                                int diferenciaVida = miVida - vidaRival;

                                // Umbrales adaptativos según la dificultad seleccionada
                                int umbralRequerido = 40;
                                if (nivel == FACIL)   umbralRequerido = -30; // Se tira aunque tenga menos vida
                                if (nivel == MEDIO)   umbralRequerido = 10;  // Va si está ligeramente en ventaja
                                if (nivel == DIFICIL) umbralRequerido = 40;  // Tu condición estricta de +40 HP

                                if (diferenciaVida >= umbralRequerido) {
                                    m.punt = 500 + diferenciaVida;
                                    listaCombatesSeguros.push_back(m);
                                }
                            }

                            // ---- CATEGORÍA C: APERTURA Y DESPLIEGUE HACIA EL CENTRO ----
                            else if (objetivo == nullptr) {
                                // Bonificamos la cercanía al centro geométrico (4,4) para avanzar líneas
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

    // ==========================================
    // 2. TOMA DE DECISIONES POR CAPAS DE PRIORIDAD
    // ==========================================
    Mov jugadaFinal;
    bool jugadaEncontrada = false;

    // 1ª Capa: Puntos de poder limpios
    if (!listaPuntosPoder.empty()) {
        sort(listaPuntosPoder.begin(), listaPuntosPoder.end(), [](const Mov& a, const Mov& b) { return a.punt > b.punt; });
        if (listaPuntosPoder[0].punt > 50) {
            jugadaFinal = listaPuntosPoder[0];
            jugadaEncontrada = true;
            cout << "IA: Prioridad 1 -> Ocupando punto de poder dorado." << endl;
        }
    }

    // 2ª Capa: Combates ventajosos (Garantía de vida)
    if (!jugadaEncontrada && !listaCombatesSeguros.empty()) {
        sort(listaCombatesSeguros.begin(), listaCombatesSeguros.end(), [](const Mov& a, const Mov& b) { return a.punt > b.punt; });
        jugadaFinal = listaCombatesSeguros[0];
        jugadaEncontrada = true;
        cout << "IA: Prioridad 2 -> Entrando en combate ventajoso (Diferencia HP favorable)." << endl;
    }

    // 3ª Capa: Despliegue inicial (Cuando no hay enemigos ni puntos a tiro)
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

    // ==========================================
    // 3. EJECUCIÓN DEL MOVIMIENTO
    // ==========================================
    if (jugadaEncontrada) {
        tablero.casillas[jugadaFinal.fD][jugadaFinal.cD] = tablero.casillas[jugadaFinal.f0][jugadaFinal.c0];
        tablero.casillas[jugadaFinal.f0][jugadaFinal.c0] = nullptr;
    }
    else {
        cout << "IA: No se han encontrado movimientos viables esta ronda." << endl;
    }
}