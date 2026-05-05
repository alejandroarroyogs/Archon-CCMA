#include "arena.h"
#include "piezas.h"
#include "tablero.h"
#include "ETSIDI.h"

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
            e->z = (float)-(rand() % 10);
            e->brillo = (float)(rand() % 100) / 100.0f;
            estrellas.push_back(e);
        }
    }
}

Arena::~Arena() {
    for (auto h : hechizosAzules) delete h;
    for (auto h : hechizosRojos) delete h;
    hechizosAzules.clear();
    hechizosRojos.clear();
    for (auto e : estrellas) {
        delete e;
    }
    estrellas.clear();
}

void Arena::inicializa(Pieza* a, Pieza* b, int turnoInicial)
{
    atacante = a;
    defensor = b;
    turno = turnoInicial;

    // Reseteamos el uso de los hechizos para la nueva batalla
    for (auto h : hechizosAzules) h->setUsado(false);
    for (auto h : hechizosRojos) h->setUsado(false);

    // --- DAÑO DE PRUEBA: Empiezan dañados al 50% ---
    if (atacante != nullptr) {
        atacante->SetVida(50);
    }
    if (defensor != nullptr) {
        defensor->SetVida(50);
    }
}

void Arena::ponMusica()
{
    ETSIDI::playMusica("sonidos/marcha.mp3", false);
}

void Arena::dibuja()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- ¡MECANISMO DE SEGURIDAD PARA PRUEBAS! ---
    // Si entramos a la arena y no hay piezas reales asignadas (nullptr),
    // creamos dos drones de prueba temporales dañados para poder testear el hechizo.
    if (atacante == nullptr || defensor == nullptr) {
        static Drone dronePruebaJedi(1);
        static Drone dronePruebaSith(2);

        atacante = &dronePruebaJedi;
        defensor = &dronePruebaSith;

        // Les forzamos la vida al 50% para ver cómo se curan
        atacante->SetVida(50);
        defensor->SetVida(50);
    }

    // 1. ESTRELLAS 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -15, 15);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    dibujaFondo();
    glEnable(GL_DEPTH_TEST);

    // 2. ESCENA DE COMBATE 3D
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

    // --- ENFRENTAMIENTO CARA A CARA ---
    if (atacante != nullptr && defensor != nullptr) {
        // A) ATACANTE (Izquierda: Rota a 270º para mirar al centro)
        glPushMatrix();
        glTranslatef(-4.0f, -3.7f, 0.0f);
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        glScalef(2.8f, 2.8f, 2.8f);
        atacante->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();

        // B) DEFENSOR (Derecha: Rota a 90º para mirar al centro)
        glPushMatrix();
        glTranslatef(4.0f, -3.7f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glScalef(2.8f, 2.8f, 2.8f);
        defensor->DibujarCombate(0.0f, 0.0f, false);
        glPopMatrix();
    }

    BarraVida();
    dibujoHechizos();
}

void Arena::dibujaPlataforma()
{
    GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glPushMatrix();
    glTranslatef(0, -4, 0);
    glScalef(15, 0.6, 8);
    glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    // 2. BORDE DE NEÓN 
    glDisable(GL_LIGHTING);

    // --- RESTAURAMOS EL COLOR DEL TURNO ---
    if (turno == 1) glColor3f(0.0f, 0.6f, 1.0f); // Azul Jedi
    else glColor3f(1.0f, 0.0f, 0.0f);            // Rojo Sith

    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
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

    int vidaAtacante = atacante->GetVida();
    int vidaDefensor = defensor->GetVida();

    if (vidaAtacante < 0) vidaAtacante = 0;
    if (vidaAtacante > 100) vidaAtacante = 100;
    if (vidaDefensor < 0) vidaDefensor = 0;
    if (vidaDefensor > 100) vidaDefensor = 100;

    float vidaA = (float)vidaAtacante / 100.0f;
    float vidaD = (float)vidaDefensor / 100.0f;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1.0f, 1.0f, 1.0f);

    // Barra Atacante
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(50, 720, 350, 740);

    if (vidaA > 0.0f) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(50, 720, 50 + (300.0f * vidaA), 740);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(50, 720);
    glVertex2f(350, 720);
    glVertex2f(350, 740);
    glVertex2f(50, 740);
    glEnd();

    // Barra Defensor
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(650, 720, 950, 740);

    if (vidaD > 0.0f) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRectf(650, 720, 650 + (300.0f * vidaD), 740);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(650, 720);
    glVertex2f(950, 720);
    glVertex2f(950, 740);
    glVertex2f(650, 740);
    glEnd();

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
    gluOrtho2D(0, 1000, 0, 800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    std::vector<Hechizo*>& lista = (turno == 1) ? hechizosAzules : hechizosRojos;

    int x = (turno == 1) ? 50 : 550;
    int y = 550;

    // Título de la sección
    glColor3f(1.0f, 1.0f, 0.0f);
    dibujaTexto(x, y, (turno == 1) ? "HECHIZOS JEDI:" : "HECHIZOS SITH:");

    y -= 40;
    for (int i = 0; i < (int)lista.size(); i++) {
        std::string texto = std::to_string(i + 1) + ". ";

        if (lista[i]->estaUsado()) {
            glColor3f(0.5f, 0.5f, 0.5f); // Gris si está agotado
            texto += "AGOTADO";
        }
        else {
            glColor3f(1.0f, 1.0f, 0.0f); // Amarillo si está disponible
            texto += lista[i]->getNombre();
        }

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

    // Mantenemos el color configurado previamente en dibujoHechizos
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
    std::vector<Hechizo*>& lista = (turno == 1) ? hechizosAzules : hechizosRojos;

    if (indice < 0 || indice >= (int)lista.size()) return;

    Hechizo* hechizoSeleccionado = lista[indice];

    if (hechizoSeleccionado->estaUsado()) {
        return;
    }

    Pieza* piezaAliada = (turno == 1) ? atacante : defensor;

    if (piezaAliada != nullptr) {
        piezaAliada->SetVida(100); // Curamos la salud de la pieza
        hechizoSeleccionado->setUsado(true); // Marcamos el hechizo como AGOTADO

        extern Mundo mundo;
        hechizoSeleccionado->aplica(mundo, piezaAliada);

        ETSIDI::play("sonidos/laser.wav");
    }

    glutPostRedisplay();
}

void Arena::inicializaEstrellas(int cantidad)
{
    for (auto e : estrellas) delete e;
    estrellas.clear();

    for (int i = 0; i < cantidad; i++) {
        Estrella* e = new Estrella();
        e->x = (rand() % 2000 - 1000) / 100.0f;
        e->y = (rand() % 2000 - 1000) / 100.0f;
        e->brillo = (rand() % 100) / 100.0f;
        estrellas.push_back(e);
    }
}

void Arena::dibujaFondo()
{
    glDisable(GL_LIGHTING);
    glPointSize(4.0f);

    glBegin(GL_POINTS);
    for (auto e : estrellas) {
        glColor3f(e->brillo * 0.6f, e->brillo * 0.6f, e->brillo * 1.0f);
        glVertex2f(e->x, e->y);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}