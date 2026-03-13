#pragma once

const int VACIA = 0;
const int SOLDADO_AZUL = 1;
const int SOLDADO_ROJO = 2;
const int MAGO_AZUL = 3;
const int MAGO_ROJO = 4;

bool EsAzul(int pieza);
bool EsRoja(int pieza);
void DibujarPieza(float x, float z, float r, float g, float b, int tipo);
