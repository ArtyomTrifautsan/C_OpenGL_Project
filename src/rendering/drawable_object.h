#pragma once


#include "../middleware/algebra/algebra.h"


typedef struct 
{
    float *vertices;
    unsigned int *indices;
    unsigned int n_vertices, n_indices;

    unsigned int VBO, VAO, EBO;

    float x, y, z;
    float scale_x, scale_y, scale_z;
    float rotate_x, rotate_y, rotate_z;

} DrawableObject;


unsigned int load_shaders();

void create_drawable_object(DrawableObject* object);
void load_object_vertices(DrawableObject* object);
void set_render_properties(DrawableObject* object);
void set_default_position(DrawableObject* object);
void render_drawable_object(DrawableObject* object, unsigned int shader_program);
void delete_drawable_object(DrawableObject* object);


Matrix get_translate_matrix(DrawableObject* object);
Matrix get_scale_matrix(DrawableObject* object);
Matrix get_rotate_matrix(DrawableObject* object);
Matrix get_model_matrix(Matrix *translate_matrix, Matrix *scale_matrix, Matrix *rotate_matrix);
Matrix get_rotate_x_matrix(DrawableObject* object);
Matrix get_rotate_y_matrix(DrawableObject* object);
Matrix get_rotate_z_matrix(DrawableObject* object);
