#pragma once

#include "../../rendering/material.h"

typedef struct DynamicArrayMaterial {
    int size;
    int capacity;
    Material* array;
} DynamicArrayMaterial;


DynamicArrayMaterial create_darray_material(int capacity);
void free_darray_material(DynamicArrayMaterial* array);

void add_element_darray_material(DynamicArrayMaterial* array, Material* element);
Material remove_element_darray_material(DynamicArrayMaterial* array, int index);

// void print_darray_material(DynamicArrayMaterial* array);
void clear_darray_material(DynamicArrayMaterial* array);

Material get_element_darray_material(DynamicArrayMaterial* array, int index);
void set_element_darray_material(DynamicArrayMaterial* array, int index, Material* element);
