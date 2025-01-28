#pragma once


#include "../data_structures/matrix/matrix.h"


typedef struct DrawableObject
{
    float *vertices;
    unsigned int *indices;
    unsigned int n_vertices, n_indices;

    unsigned int VBO, VAO, EBO;

    float x, y, z;
    float scale_x, scale_y, scale_z;
    float rotate_x, rotate_y, rotate_z;

    Matrix model_matrix;

} DrawableObject;


unsigned int load_shaders();

void create_drawable_object(DrawableObject* object);
void load_object_vertices(DrawableObject* object);
void set_render_properties(DrawableObject* object);
void set_default_position(DrawableObject* object);
void render_drawable_object(DrawableObject* object, unsigned int shader_program);
void delete_drawable_object(DrawableObject* object);

void set_object_coords(DrawableObject* object, float x, float y, float z);
void set_object_coord_x(DrawableObject* object, float x);
void set_object_coord_y(DrawableObject* object, float y);
void set_object_coord_z(DrawableObject* object, float z);

void set_object_scale(DrawableObject* object, float scale_x, float scale_y, float scale_z);
void set_object_scale_x(DrawableObject* object, float scale_x);
void set_object_scale_y(DrawableObject* object, float scale_y);
void set_object_scale_z(DrawableObject* object, float scale_z);

void set_object_rotate(DrawableObject* object, float rotate_x, float rotate_y, float rotate_z);
void set_object_rotate_x(DrawableObject* object, float rotate_x);
void set_object_rotate_y(DrawableObject* object, float rotate_y);
void set_object_rotate_z(DrawableObject* object, float rotate_z);


void update_model_matrix(DrawableObject* object);
Matrix get_model_matrix(DrawableObject* object);
