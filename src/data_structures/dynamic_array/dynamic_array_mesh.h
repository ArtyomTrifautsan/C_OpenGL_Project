#pragma once

#include "../../rendering/mesh.h"

typedef struct DynamicArrayMesh {
    int size;
    int capacity;
    Mesh* array;
} DynamicArrayMesh;


DynamicArrayMesh create_darray_mesh(int capacity);
void free_darray_mesh(DynamicArrayMesh* array);

void add_element_darray_mesh(DynamicArrayMesh* array, Mesh* element);
Mesh remove_element_darray_mesh(DynamicArrayMesh* array, int index);

// void print_darray_mesh(DynamicArrayMesh* array);
void clear_darray_mesh(DynamicArrayMesh* array);

Mesh get_element_darray_mesh(DynamicArrayMesh* array, int index);
void set_element_darray_mesh(DynamicArrayMesh* array, int index, Mesh* element);
