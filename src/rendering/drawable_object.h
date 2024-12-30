#pragma once


unsigned int load_shaders();


typedef struct 
{
    float *vertices;
    unsigned int *indices;
    unsigned int n_vertices, n_indices;

    unsigned int VBO, VAO, EBO;

    float x, y, z;
    float rotate_x, rotate_y, rotate_z;

} DrawableObject;


void create_drawable_object(DrawableObject* object);
void load_object_vertices(DrawableObject* object);
void set_render_properties(DrawableObject* object);
void render_drawable_object(DrawableObject* object, unsigned int shader_program);
void delete_drawable_object(DrawableObject* object);
