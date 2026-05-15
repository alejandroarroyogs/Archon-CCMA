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
        else if (tipo == "vn") { // --- NUEVO: Normal ---
            Vertice3D n;
            iss >> n.x >> n.y >> n.z;
            normales.push_back(n);
        }
        else if (tipo == "f") {
            // Es una cara
            Cara c;
            std::string bloque;
            while (iss >> bloque) {
                std::stringstream ss(bloque);
                std::string v_idx, t_idx, n_idx;
                std::getline(ss, v_idx, '/');
                std::getline(ss, t_idx, '/');
                std::getline(ss, n_idx, '/');

                if (!v_idx.empty()) c.indices_vertices.push_back(std::stoi(v_idx) - 1);
                if (!n_idx.empty()) c.indices_normales.push_back(std::stoi(n_idx) - 1);
               
            }
            caras.push_back(c);
        }
    }
}

void ModeloOBJ::dibuja() const {
    for (const auto& cara : caras) {
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < cara.indices_vertices.size(); ++i) {
            // Enviamos la normal para que la luz funcione
            if (i < cara.indices_normales.size()) {
                int nIdx = cara.indices_normales[i];
                glNormal3d(normales[nIdx].x, normales[nIdx].y, normales[nIdx].z);
            }
            int vIdx = cara.indices_vertices[i];
            glVertex3d(vertices[vIdx].x, vertices[vIdx].y, vertices[vIdx].z);
        }
        glEnd();
    }
}