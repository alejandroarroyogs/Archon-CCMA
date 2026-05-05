#include "arena.h"
#include "piezas.h"
#include "tablero.h"
#include "ETSIDI.h"

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

    // 1. DIBUJAR FONDO 2D (ESTRELLAS)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -15, 15);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    dibujaFondo();
    glEnable(GL_DEPTH_TEST);

    // 2. DIBUJAR ARENA Y COMBATE 3D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1000.0 / 800.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 6.0, 18.0,
        0.0, -1.5, 0.0,
        0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    dibujaPlataforma();

    // --- RENDERIZADO DE LAS PIEZAS ENFRENTADAS Y MÁS GRANDES ---
    if (atacante != nullptr && defensor != nullptr) {
        // A) ATACANTE (Lado Izquierdo: x = -4.0f)
        glPushMatrix();
        glTranslatef(-4.0f, -3.7f, 0.0f);
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);

        // Aumentamos significativamente el tamaño en combate
        glScalef(2.8f, 2.8f, 2.8f);
        atacante->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();

        // B) DEFENSOR (Lado Derecho: x = 4.0f)
        glPushMatrix();
        glTranslatef(4.0f, -3.7f, 0.0f);
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f); // ¡Girado 180º respecto al atacante para mirarse!

        glScalef(2.8f, 2.8f, 2.8f);
        defensor->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();
    }

    // Dibujamos las barras de vida y hechizos sobre la pantalla
    BarraVida();
    dibujoHechizos();

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
void Arena::BarraVida()
{
    if (!atacante || !defensor) return;

    // 1. OBTENER VIDAS (Forzamos un mínimo de 0 y máximo de 100 por seguridad)
    int vidaAtacante = atacante->GetVida();
    int vidaDefensor = defensor->GetVida();

    if (vidaAtacante < 0) vidaAtacante = 0;
    if (vidaAtacante > 100) vidaAtacante = 100;
    if (vidaDefensor < 0) vidaDefensor = 0;
    if (vidaDefensor > 100) vidaDefensor = 100;

    float vidaA = (float)vidaAtacante / 100.0f;
    float vidaD = (float)vidaDefensor / 100.0f;

    // 2. CONFIGURACIÓN DE VISTA 2D (ORTO)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 3. APAGADO ABSOLUTO DE ESTADOS EN OPENGL
    glDisable(GL_LIGHTING);      // Desactivamos luces (evita que se vuelvan grises por sombra)
    glDisable(GL_TEXTURE_2D);    // Desactivamos texturas (evita que se pinte la fuente del texto)
    glBindTexture(GL_TEXTURE_2D, 0); // Desvinculamos cualquier textura
    glDisable(GL_BLEND);         // Desactivamos mezclas transparentes
    glDisable(GL_DEPTH_TEST);    // Hacemos que se dibuje por encima de todo sin importar la profundidad

    // Reset de color interno de OpenGL (limpia arrastres del texto amarillo)
    glColor3f(1.0f, 1.0f, 1.0f);

    // ==========================================
    // --- BARRA ATACANTE (Izquierda: x de 50 a 350) ---
    // ==========================================
    // Fondo (Gris oscuro)
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(50, 720, 350, 740);

    // Vida Activa (Verde brillante) - Solo si tiene vida
    if (vidaA > 0.0f) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(50, 720, 50 + (300.0f * vidaA), 740);
    }

    // Borde de la barra (Línea blanca para verificar el encuadre)
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(50, 720);
    glVertex2f(350, 720);
    glVertex2f(350, 740);
    glVertex2f(50, 740);
    glEnd();

    // ==========================================
    // --- BARRA DEFENSOR (Derecha: x de 650 a 950) ---
    // ==========================================
    // Fondo (Gris oscuro)
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(650, 720, 950, 740);

    // Vida Activa (Verde brillante) - Solo si tiene vida
    if (vidaD > 0.0f) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(650, 720, 650 + (300.0f * vidaD), 740);
    }

    // Borde de la barra (Línea blanca)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(650, 720);
    glVertex2f(950, 720);
    glVertex2f(950, 740);
    glVertex2f(650, 740);
    glEnd();

    // 4. RESTAURAR ESTADOS
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    ETSIDI::setTextColor(1.0f, 1.0f, 0.0f);

    // ¡REDUCIDO EL TAMAÑO DE LA FUENTE AQUÍ! (De 55 a 24 para que quepa perfectamente)
    ETSIDI::setFont("fuentes/jedisf.ttf", 24);

    ETSIDI::printxy(texto, x, y);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
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

