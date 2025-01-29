#pragma once

#include "material.h"
#include "mesh.h"
#include "shader_program.h"

#include "../data_structures/dynamic_array/dynamic_array_float.h"
#include "../data_structures/dynamic_array/dynamic_array_int.h"
#include "../data_structures/dynamic_array/dynamic_array_mesh.h"
#include "../data_structures/dynamic_array/dynamic_array_material.h"

#include "../data_structures/matrix/matrix.h"


typedef struct Model3D {
    DynamicArrayMesh meshes;

    float x, y, z;
    float scale_x, scale_y, scale_z;
    float rotate_x, rotate_y, rotate_z;

    Matrix model_matrix;
} Model3D;


Model3D create_model3D(DynamicArrayMesh* meshes);
void _set_render_properties(Model3D* model);
void _set_default_position(Model3D* model);
void render_model3D(Model3D* model, ShaderProgram shader_program);

void set_model_coords(Model3D* model, float x, float y, float z);
void set_model_coord_x(Model3D* model, float x);
void set_model_coord_y(Model3D* model, float y);
void set_model_coord_z(Model3D* model, float z);

void set_model_scale(Model3D* model, float scale_x, float scale_y, float scale_z);
void set_model_scale_x(Model3D* model, float scale_x);
void set_model_scale_y(Model3D* model, float scale_y);
void set_model_scale_z(Model3D* model, float scale_z);

void set_model_rotate(Model3D* model, float rotate_x, float rotate_y, float rotate_z);
void set_model_rotate_x(Model3D* model, float rotate_x);
void set_model_rotate_y(Model3D* model, float rotate_y);
void set_model_rotate_z(Model3D* model, float rotate_z);


void update_model_matrix(Model3D* model);
Matrix get_model_matrix(Model3D* model);