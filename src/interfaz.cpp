#include "interfaz.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include "mundo.h"

void Interfaz::dibujaTexto(float x, float y, const char* texto, int idBoton)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (ratontexto == idBoton && idBoton != -1) {
        glColor3f(1.0f, 0.4f, 0.0f); // Naranja al pasar el ratón por encima
        ETSIDI::setTextColor(1.0f, 0.4f, 0.0f);
    }
    else {
        glColor3f(1.0f, 1.0f, 0.0f); // Amarillo por defecto
        ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    }

    if (idBoton >= 10 && idBoton <= 14) {
        ETSIDI::setFont("fuentes/jedisf.ttf", 36); // Tamaño simétrico para los 5 submenús
    }
    else if (idBoton == 2) {
        ETSIDI::setFont("fuentes/jedisf.ttf", 32); // Tamaño para el botón ATRÁS
    }
    else {
        ETSIDI::setFont("fuentes/jedisf.ttf", 55); // Títulos principales
    }

    ETSIDI::printxy(texto, x, y, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Interfaz::dibujaFondo()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/deathstar.png").id);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, 1);
    glTexCoord2f(0, 0); glVertex2f(-1, 1);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void Interfaz::gestionRaton(int boton, int estdo, int x, int y)
{
    if (boton == GLUT_LEFT_BUTTON && estdo == GLUT_DOWN)
    {
        ETSIDI::play("sonidos/laser.wav");
        if (ratontexto == -1) return;

        if (estado == MENU) {
            if (ratontexto == 0)      estado = SELEC_MODO;
            else if (ratontexto == 1) {
                estado = INSTRUC;
                subMenuActual = INICIO_INSTRUC; // Siempre empezamos en la pantalla de la imagen
            }
        }
        else if (estado == SELEC_MODO) {
            if (ratontexto == 0) {
                modoJuego = 1;
                mundo.inicializarPartida();
                estado = JUGANDO;
            }
            else if (ratontexto == 1) {
                modoJuego = 2;
                mundo.inicializarPartida();
                estado = JUGANDO;
            }
        }
        else if (estado == INSTRUC) {
            if (ratontexto == 2) {
                // Si estamos en un submenú de texto, volvemos a la pantalla de tu imagen
                if (subMenuActual != INICIO_INSTRUC) {
                    subMenuActual = INICIO_INSTRUC;
                }
                // Si ya estamos en la pantalla de los 5 botones de tu imagen, volvemos al menú de inicio del juego
                else {
                    estado = MENU;
                }
            }
            else if (ratontexto == 10) subMenuActual = OBJETIVO;
            else if (ratontexto == 11) subMenuActual = MOVIMIENTO;
            else if (ratontexto == 12) subMenuActual = COMBATE;
            else if (ratontexto == 13) subMenuActual = PODERES;
            else if (ratontexto == 14) subMenuActual = PUNTUACION;
        }

        glutPostRedisplay();
    }
}

void Interfaz::movimientoRaton(int x, int y)
{
    float anchoReal = (float)glutGet(GLUT_WINDOW_WIDTH);
    float altoReal = (float)glutGet(GLUT_WINDOW_HEIGHT);

    float mvX = (x / anchoReal) * 1000.0f;
    float mvY = 800.0f - ((y / altoReal) * 800.0f);

    if (estado == MENU) {
        if (mvX >= 400 && mvX <= 600 && mvY >= 370 && mvY <= 440) ratontexto = 0;
        else if (mvX >= 270 && mvX <= 730 && mvY >= 210 && mvY <= 280) ratontexto = 1;
        else ratontexto = -1;
    }
    else if (estado == SELEC_MODO) {
        if (mvX >= 420 && mvX <= 580 && mvY >= 370 && mvY <= 440) ratontexto = 0;
        else if (mvX >= 280 && mvX <= 720 && mvY >= 210 && mvY <= 280) ratontexto = 1;
        else ratontexto = -1;
    }
    else if (estado == INSTRUC) {
        // CORRECCIÓN DE LA COLISIÓN: unificamos la detección del botón ATRÁS (y = 110)
        // Damos un margen de seguridad amplio y centrado en la pantalla (X: 400 a 600, Y: 90 a 160)
        if (mvX >= 400 && mvX <= 600 && mvY >= 90 && mvY <= 160) {
            ratontexto = 2;
        }
        // Si no se está tocando el botón ATRÁS, comprobamos los 5 submenús (solo si estamos en la pantalla índice)
        else if (subMenuActual == INICIO_INSTRUC) {
            if (mvX >= 340 && mvX <= 660) {
                if (mvY >= 510 && mvY <= 555)      ratontexto = 10; // OBJETIVO
                else if (mvY >= 430 && mvY <= 475) ratontexto = 11; // MOVIMIENTO
                else if (mvY >= 350 && mvY <= 395) ratontexto = 12; // COMBATE
                else if (mvY >= 270 && mvY <= 315) ratontexto = 13; // PODERES
                else if (mvY >= 190 && mvY <= 235) ratontexto = 14; // PUNTUACION
                else ratontexto = -1;
            }
            else {
                ratontexto = -1;
            }
        }
        else {
            ratontexto = -1;
        }
    }

    glutPostRedisplay();
}

void Interfaz::dibujaMenu()
{
    dibujaFondo();
    dibujaTexto(180, 600, "ARCHON CONTRATACA", -1);
    dibujaTexto(410, 380, "JUGAR", 0);
    dibujaTexto(280, 220, "INSTRUCCIONES", 1);
}

void Interfaz::eligeModo()
{
    dibujaFondo();
    dibujaTexto(265, 600, "MODO DE JUEGO", -1);
    dibujaTexto(430, 380, "JEDI", 0);
    dibujaTexto(285, 220, "JEDI vs SITH", 1);
}

void Interfaz::dibujaInstrucciones()
{
    dibujaFondo();

    // === PANTALLA ÍNDICE: TU IMAGEN CON LOS 5 BOTONES ===
    if (subMenuActual == INICIO_INSTRUC) {
        dibujaTexto(260, 650, "INSTRUCCIONES", -1);

        dibujaTexto(390, 520, "OBJETIVO", 10);
        dibujaTexto(360, 440, "MOVIMIENTO", 11);
        dibujaTexto(385, 360, "COMBATE", 12);
        dibujaTexto(390, 280, "PODERES", 13);
        dibujaTexto(365, 200, "PUNTUACION", 14);

        dibujaTexto(440, 110, "ATRAS", 2);
    }
    // === PANTALLAS DE CONTENIDO: CUADRO EXPLICATIVO CON MARCO AZUL ===
    else {
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 1000, 0, 800);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
        glDisable(GL_LIGHTING); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // 1º TÍTULO DINÁMICO CENTRADO
        ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
        ETSIDI::setFont("fuentes/jedisf.ttf", 45);

        if (subMenuActual == OBJETIVO)        ETSIDI::printxy("OBJETIVO", 390, 650);
        else if (subMenuActual == MOVIMIENTO)  ETSIDI::printxy("MOVIMIENTO", 350, 650);
        else if (subMenuActual == COMBATE)    ETSIDI::printxy("COMBATE", 390, 650);
        else if (subMenuActual == PODERES)    ETSIDI::printxy("PODERES", 390, 650);
        else if (subMenuActual == PUNTUACION) ETSIDI::printxy("PUNTUACION", 350, 650);

        // Caja contenedora translúcida oscura
        glColor4f(0.05f, 0.05f, 0.1f, 0.85f);
        glBegin(GL_QUADS);
        glVertex2f(150, 570); glVertex2f(850, 570);
        glVertex2f(850, 220); glVertex2f(150, 220);
        glEnd();

        // Marco de fuerza azul brillante
        glColor3f(0.0f, 0.8f, 1.0f); glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(150, 570); glVertex2f(850, 570);
        glVertex2f(850, 220); glVertex2f(150, 220);
        glEnd();
        glLineWidth(1.0f);

        // 2º TEXTOS DESCRIPTIVOS INTERNOS
        ETSIDI::setTextColor(1.0f, 1.0f, 1.0f);
        ETSIDI::setFont("fuentes/jedisf.ttf", 20);
        int xTexto = 180;

        switch (subMenuActual) {
        case OBJETIVO: // [cite: 123, 124, 125]
            ETSIDI::printxy("- MISION PRINCIPAL: Lidera tus fuerzas hacia la victoria total.", xTexto, 510);
            ETSIDI::printxy("- CONDICION DE VICTORIA 1: Elimina por completo todas las piezas ", xTexto, 450);
            ETSIDI::printxy("  de la faccion enemiga en intensos duelos dentro de la arena.", xTexto, 415);
            ETSIDI::printxy("- CONDICION DE VICTORIA 2: Conquista y asegura de forma simultanea ", xTexto, 345);
            ETSIDI::printxy("  los 5 Puntos de Poder luminosos distribuidos por el mapa.", xTexto, 310);
            break;

        case MOVIMIENTO:
            ETSIDI::printxy("- Usa las teclas W, A, S, D para desplazar el cursor de seleccion.", xTexto, 510);
            ETSIDI::printxy("- Pulsa la tecla ESPACIO sobre una pieza propia para seleccionarla.", xTexto, 460);
            ETSIDI::printxy("- Mueve la unidad a una casilla valida y confirma pulsando ESPACIO.", xTexto, 410);
            ETSIDI::printxy("- Cada unidad (Jedi, Droide, Clon, Vader) posee un rango de", xTexto, 340);
            ETSIDI::printxy("  desplazamiento y restricciones de terreno unicos.", xTexto, 300);
            break;

        case COMBATE: // [cite: 109, 126]
            ETSIDI::printxy("- Al moverte a una casilla ocupada por un rival, iniciara el duelo.", xTexto, 510);
            ETSIDI::printxy("- Entraras de forma automatica en la Plataforma de la Arena 3D.", xTexto, 460);
            ETSIDI::printxy("- Mueve a tu campeon en tiempo real usando las FLECHAS del teclado.", xTexto, 410);
            ETSIDI::printxy("- Ejecuta tus ataques y disparos laser pulsando la barra ESPACIO.", xTexto, 340);
            ETSIDI::printxy("- Recordar el ciclo, las casillas de tu color te otorgan bonus de vitalidad", xTexto, 290);
            ETSIDI::printxy("- asi como las casillas de color contrario, te lo haran mas dificil ", xTexto, 240);
            break;

        case PODERES: // [cite: 152, 165]
            ETSIDI::printxy("- Coloca el cursor del juego sobre tu Hechicero principal.", xTexto, 510);
            ETSIDI::printxy("- Canaliza la Fuerza pulsando las teclas numericas del 1 al 6:", xTexto, 460);
            ETSIDI::printxy("  [1] Curacion  [2] Teletransporte  [3] Alteracion Cronologica ", xTexto, 420);
            ETSIDI::printxy("  [4] Confusion Mental  [5] Espiritu de la Fuerza  [6] Carbonita ", xTexto, 380);
            ETSIDI::printxy("- Mueve la mira dorada del hechizo y confirma con ENTER o ESPACIO.", xTexto, 310);
            ETSIDI::printxy("- Nota: Cada poder consume la accion y se usa una vez por partida.", xTexto, 260);
            break;

        case PUNTUACION: // [cite: 111, 137]
            ETSIDI::printxy("- El danio sufrido por tus tropas se conserva al regresar al tablero.", xTexto, 510);
            ETSIDI::printxy("- Las unidades regeneran vitalidad de forma pasiva descansando ", xTexto, 460);
            ETSIDI::printxy("  sobre los Puntos de Poder o casillas afines a su Fuerza.", xTexto, 410);
            ETSIDI::printxy("- Al finalizar el juego, tu score dependera del tiempo total,", xTexto, 340);
            ETSIDI::printxy("- Intenta batir el record y entrar en el TOP de la galaxia.", xTexto, 300);
            break;
        default:
            break;
        }

        glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);

        // 3º BOTÓN ATRÁS INTERNO
        dibujaTexto(440, 110, "ATRAS", 2);
    }
}