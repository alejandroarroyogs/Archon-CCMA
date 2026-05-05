#pragma once
#include <string>
#include <vector>

// Estructura auxiliar para guardar un punto en 3D
struct Vertice3D {
    double x, y, z;
};

// Estructura auxiliar para guardar las caras 
struct Cara {
    std::vector<int> indices_vertices;
};

class ModeloOBJ {
private:
    std::vector<Vertice3D> vertices;
    std::vector<Cara> caras;

public:
    // Constructor que recibe la ruta del archivo
    ModeloOBJ(const std::string& ruta_archivo);

    // Método de dibujo obligatorio por normativa (no modifica el objeto)
    void dibuja() const;
};