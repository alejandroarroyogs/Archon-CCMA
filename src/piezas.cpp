#include "freeglut.h"
#include "piezas.h"
#include <cmath>
#include <iostream>

Pieza::Pieza(int b, int v, int atq, int arm) : bando(b), vida(v), vidaMax(v), ataque(atq), armadura(arm), danio(atq), estaEncarcelada(false), turnosPrision(0) {}
Pieza::~Pieza() {}

int Pieza::GetBando() { return bando; }
int Pieza::GetVida() { return vida; }
int Pieza::GetDanio() { return danio; }
void Pieza::SetVida(int v) { vida = v; }
void Pieza::RecibirDanio(int d) {
    vida -= d;
    if (vida < 0) vida = 0;
}
void Pieza::recuperarVida(int cantidad) {
    vida += cantidad;
    if (vida > vidaMax) vida = vidaMax;
}
bool Pieza::EstaViva() { return vida > 0; }
bool Pieza::EsAzul() { return bando == 1; }
bool Pieza::EsRoja() { return bando == 2; }

bool EsAzul(Pieza* pieza) { return (pieza != nullptr) && pieza->EsAzul(); }
bool EsRoja(Pieza* pieza) { return (pieza != nullptr) && pieza->EsRoja(); }

