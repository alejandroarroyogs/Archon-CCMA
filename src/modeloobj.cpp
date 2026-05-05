#include "modeloobj.h"
#include <freeglut.h> 
#include <fstream>
#include <sstream>
#include <iostream>

ModeloOBJ::ModeloOBJ(const std::string& ruta_archivo) {
    std::ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        std::cerr << "ERROR: No se pudo abrir el archivo " << ruta_archivo << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream iss(linea);
        std::string tipo;
        iss >> tipo;

        if (tipo == "v") {
            // Es un vértice
            Vertice3D v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (tipo == "f") {
            // Es una cara
            Cara c;
            std::string dato_vertice;
            while (iss >> dato_vertice) {
                std::istringstream viss(dato_vertice);
                std::string indice_str;
                std::getline(viss, indice_str, '/'); // Cortamos por la barra '/' si la hay
                if (!indice_str.empty()) {
                    // En .obj los índices empiezan en 1, en C++ en 0. Restamos 1.
                    c.indices_vertices.push_back(std::stoi(indice_str) - 1);
                }
            }
            caras.push_back(c);
        }
    }
    archivo.close();
}

void ModeloOBJ::dibuja() const {
    // Dibujamos todas las caras leídas
    for (const auto& cara : caras) {
        glBegin(GL_POLYGON);
        for (int indice : cara.indices_vertices) {
            if (indice >= 0 && indice < vertices.size()) {
                const Vertice3D& v = vertices[indice];
                glVertex3d(v.x, v.y, v.z);
            }
        }
        glEnd();
    }
}