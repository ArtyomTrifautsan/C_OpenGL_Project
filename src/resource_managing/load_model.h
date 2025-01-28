#pragma once

#include "../rendering/mesh.h"
#include "../rendering/material.h"
#include "../rendering/Model3D.h"

#include "../data_structures/dynamic_array/dynamic_array_string.h"


Model3D load_OBJ(const char *filename, const char *path_to_file);
void load_MTL(char *filename, const char *path_to_file, DynamicArrayMaterial* materials, DynamicArrayString* material_names);
