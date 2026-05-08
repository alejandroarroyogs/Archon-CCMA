#include "controlIA.h"
#include "piezas.h"
#include "mundo.h"
#include <vector>
#include <iostream>

class Mundo;
using namespace std;

int ControlIA::evalMov(Tablero& tablero, int fD, int cD)
{
    int score = 0;
    Pieza* objetivo = tablero.casillas[fD][cD];

    if (objetivo && objetivo->GetBando() == 1) score += 100; // Prioridad: Capturar

    // Bonus por posición central
    score += (5 - abs(fD - 4)) + (5 - abs(cD - 4));

    return score;
}

void ControlIA::ejecutarturno(Tablero& tablero)
{
    vector<Mov> posibles;
    cout << "IA: Pensando mi siguiente jugada..." << endl;

    for (int f = 0; f < 9; f++) {
        for (int c = 0; c < 9; c++) {
            Pieza* p = tablero.casillas[f][c];

            // Verificamos que existe pieza y es del bando 2 (IA)
            if (p != nullptr && p->GetBando() == 2) {

                for (int df = 0; df < 9; df++) {
                    for (int dc = 0; dc < 9; dc++) {

                        // USAMOS EL PUNTERO 'p' PARA LLAMAR A LA FUNCIÓN
                        if (p->MovimientoValido(f, c, df, dc)) {
                            int score = evalMov(tablero, df, dc);

                            // Guardamos el movimiento con los nombres del struct
                            Mov m;
                            m.f0 = f;  m.c0 = c;
                            m.fD = df; m.cD = dc;
                            m.punt = score;
                            posibles.push_back(m);
                        }
                    }
                }
            }
        }
    }

    if (!posibles.empty()) {
        cout << "IA: He encontrado " << posibles.size() << " movimientos. Ejecutando el mejor!" << endl;
        Mov mejor = posibles[0];
        for (auto& m : posibles) {
            if (m.punt > mejor.punt) mejor = m;
        }
    // Para mover, necesitamos que Tablero tenga una función 
            // que acepte coordenadas, o mover nosotros los punteros:
            tablero.casillas[mejor.fD][mejor.cD] = tablero.casillas[mejor.f0][mejor.c0];
            tablero.casillas[mejor.f0][mejor.c0] = nullptr;

            // ¡No olvides avisar al mundo que el turno ha terminado!
            mundo.cambiaCiclo();
    }
    else cout << "IA: ERROR " << endl;
   
}
