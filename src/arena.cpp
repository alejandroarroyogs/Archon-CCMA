#include "arena.h"
#include "piezas.h"
#include "tablero.h"

//Constructor arena
Arena::Arena()
{
    atacante = nullptr;
    defensor = nullptr;

    flashAtacante = 0;
    flashDefensor = 0;

    hechizosAzules.push_back(new HechizoHeal());
    hechizosRojos.push_back(new HechizoHeal());

    if (estrellas.empty()) {
        for (int i = 0; i < 200; i++) {
            Estrella* e = new Estrella;
            e->x = (float)(rand() % 40 - 20);
            e->y = (float)(rand() % 30 - 15);
            e->z = (float)-(rand() % 10); // Estrellas al fondo
            e->brillo = (float)(rand() % 100) / 100.0f;
            estrellas.push_back(e);
        }
    }


}
//Destructor arena
Arena::~Arena() {
    for (auto h : hechizosAzules) delete h;
    for (auto h : hechizosRojos) delete h;
    hechizosAzules.clear();
    hechizosRojos.clear();
    //limpiar mem estrellas
    for (auto e : estrellas) {
        delete e;
    }
    estrellas.clear();
}

void Arena::inicializa(Pieza* a, Pieza* b, int turnoInicial)
{
    atacante = a; //pieza del mismo color que la casilla
    defensor = b; //otra pieza
    turno = turnoInicial;
   

}

void Arena::ponMusica()
{
    ETSIDI::playMusica("sonidos/marcha.mp3", false);
}

void Arena::dibuja()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //2D PARA ESTRELLAS
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -15, 15); // Lienzo plano para las estrellas

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // Las estrellas no bloquean nada
    dibujaFondo();            // Aquí tus estrellas azules
    glEnable(GL_DEPTH_TEST);

    //3DPIEZAS Y PLATAFORMA
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perspectiva: 60 grados, relación de aspecto 4:3, cerca 0.1, lejos 1000
    gluPerspective(60.0, 4.0 / 3.0, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Posicionamos la cámara para ver la plataforma desde arriba
    gluLookAt(0.0, 8.0, 25.0,  // Ojo (arriba y atrás)
        0.0, -4.0, 0.0,  // Punto de mira (hacia el suelo)
        0.0, 1.0, 0.0);  // Vector arriba

    // Luz para que brille el metal
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    dibujaPlataforma();
    //DIBUJAR LO DEMÁS DE AQUÍ ABAJO

}

void Arena::dibujaPlataforma()
{
    // 1. CUERPO DE LA PLATAFORMA (Metálico)
    GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // Gris oscuro
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glPushMatrix();
    glTranslatef(0, -4, 0); // La bajamos para que no tape el centro
    glScalef(15, 0.6, 8);   // Gran baldosa metálica
    glutSolidCube(1.0);
    glPopMatrix();

    // 2. BORDE DE NEÓN (Cambia según turno)
    glDisable(GL_LIGHTING); // El neón brilla solo, no necesita luces

    if (turno == 1) glColor3f(0.0f, 0.6f, 1.0f); // Azul Jedi
    else glColor3f(1.0f, 0.0f, 0.0f);            // Rojo Sith

    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    // Elevado un pelín sobre el cubo (-3.6) para que no parpadee
    glVertex3f(-7.5, -3.6, 4.0);
    glVertex3f(7.5, -3.6, 4.0);
    glVertex3f(7.5, -3.6, -4.0);
    glVertex3f(-7.5, -3.6, -4.0);
    glEnd();
    glEnable(GL_LIGHTING);
    
}
//Falta que la barra de vida se dibuje bbien
void Arena::BarraVida()
{
    if (!atacante || !defensor) return;

    // Calculamos porcentajes
    float vidaA = (float)atacante->GetVida() / 100.0f;
    float vidaD = (float)defensor->GetVida() / 100.0f;

    // Dibujamos en coordenadas de pantalla (2D)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    // Barra Atacante (Izquierda)
    glColor3f(0.2f, 0.2f, 0.2f); glRectf(50, 720, 350, 740); // Fondo
    glColor3f(0.0f, 1.0f, 0.0f); glRectf(50, 720, 50 + (300 * vidaA), 740); // Vida

    // Barra Defensor (Derecha)
    glColor3f(0.2f, 0.2f, 0.2f); glRectf(650, 720, 950, 740); // Fondo
    glColor3f(0.0f, 1.0f, 0.0f); glRectf(650, 720, 650 + (300 * vidaD), 740); // Vida

    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
//falta dibujar bien los hechizos
void Arena::dibujoHechizos()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    std::vector<Hechizo*>& lista = (turno == 1) ? hechizosAzules : hechizosRojos;

    int x = (turno == 1) ? 50 : 550;
    int y = 550;

    glColor3ub(255, 255, 0);
    dibujaTexto(x, y, (turno == 1) ? "HECHIZOS JEDI:" : "HECHIZOS SITH:");

    y -= 40;
    for (int i = 0; i < (int)lista.size(); i++) {
        // Ahora 'lista' ya existe porque la definimos arriba
        if (lista[i]->estaUsado()) glColor3ub(150, 150, 150);
        else glColor3ub(255, 255, 255);

        std::string texto = std::to_string(i + 1) + ". " +
            (lista[i]->estaUsado() ? "AGOTADO" : lista[i]->getNombre());
        dibujaTexto(x, y, texto.c_str());
        y -= 30;
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

}

void Arena::dibujaTexto(float x, float y, const char* texto)
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

    // Modo de mezcla para evitar bordes raros
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);
    ETSIDI::setFont("fuentes/jedisf.ttf", 55);
    ETSIDI::printxy(texto, x, y);
    // Mejor desactivarlo al salir
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Arena::lanzarHechizo(int indice)
{
    // CORRECCIÓN C2065: Usamos 'turno' de Arena, no 'tablero'
    std::vector<Hechizo*>& lista = (turno == 1) ? hechizosAzules : hechizosRojos;

 
}
//NO TOCAR
void Arena::inicializaEstrellas(int cantidad)
{
    // Limpiamos si ya había algo
    for (auto e : estrellas) delete e;
    estrellas.clear();

    for (int i = 0; i < cantidad; i++) {
        Estrella* e = new Estrella();
        e->x = (rand() % 2000 - 1000) / 100.0f; // Coordenadas espaciales
        e->y = (rand() % 2000 - 1000) / 100.0f;
        e->brillo = (rand() % 100) / 100.0f;
        estrellas.push_back(e);
    }
}
//NO TOCAR
void Arena::dibujaFondo()
{
    glDisable(GL_LIGHTING);
    glPointSize(4.0f); // Más grandes como pediste

    glBegin(GL_POINTS);
    for (auto e : estrellas) {
        // Mezclamos su brillo con un tono azulado claro
        glColor3f(e->brillo * 0.6f, e->brillo * 0.6f, e->brillo * 1.0f);
        glVertex2f(e->x, e->y);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

