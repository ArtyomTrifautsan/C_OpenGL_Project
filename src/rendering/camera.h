#pragma once


#include "../data_structures/matrix/matrix.h"

#include "shader_program.h"


typedef struct {
    float x, y, z;
    float scale_x, scale_y, scale_z;
    float rotate_x, rotate_y, rotate_z;

    float right_side_frustum;
    float top_side_frustum;
    float near_side_frustum;
    float front_side_frustum;

    Matrix view_matrix, projection_matrix;
    Matrix view_projection_matrix;

    Matrix _translate_matrix, _scale_matrix, _rotate_matrix;
    Matrix _rotate_x_matrix, _rotate_y_matrix, _rotate_z_matrix;
} Camera;


Camera create_camera();
void send_view_projection_matrix_to_shaders(Camera* camera, ShaderProgram shader_program);
void send_scene_parameters_to_shaders(Camera* camera, ShaderProgram shader_program);

void update_view_matrix(Camera* camera);
void update_projection_matrix(Camera* camera);
void update_view_projection_matrix(Camera* camera);

Matrix get_view_matrix(Camera* camera);
Matrix get_projection_matrix(Camera* camera);
Matrix get_view_projection_matrix(Camera* camera);

void set_camera_coords(Camera* camera, float x, float y, float z);
void set_camera_coord_x(Camera* camera, float x);
void set_camera_coord_y(Camera* camera, float y);
void set_camera_coord_z(Camera* camera, float z);

void set_camera_rotate(Camera* camera, float rotate_x, float rotate_y, float rotate_z);
void set_camera_rotate_x(Camera* camera, float rotate_x);
void set_camera_rotate_y(Camera* camera, float rotate_y);
void set_camera_rotate_z(Camera* camera, float rotate_z);