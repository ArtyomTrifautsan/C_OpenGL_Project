#pragma once

#include "material.h"

#include "../data_structures/dynamic_array/dynamic_array_float.h"
#include "../data_structures/dynamic_array/dynamic_array_int.h"


typedef struct Mesh {
    Material material;
    DynamicArrayFloat vertices;
    DynamicArrayInt indices;

    unsigned int VBO, VAO, EBO;
} Mesh;


void print_mesh(Mesh* mesh);
