#include "camera.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>


Matrix multiply_view_projection_matrices(Camera* camera);

Matrix get_translate_matrix_to_view(Camera* camera);
Matrix get_scale_matrix_to_view(Camera* camera);
Matrix get_rotate_matrix_to_view(Camera* camera);
Matrix get_rotate_x_matrix_to_view(Camera* camera);
Matrix get_rotate_y_matrix_to_view(Camera* camera);
Matrix get_rotate_z_matrix_to_view(Camera* camera);
Matrix create_view_matrix(Matrix *translate_matrix, Matrix *scale_matrix, Matrix *rotate_matrix);


Camera create_camera()
{
    // printf(">>>delete_drawable_object started\n");

    Camera camera;

    camera.x = 0;
    camera.y = 0;
    camera.z = 0;

    camera.scale_x = 1;
    camera.scale_y = 1;
    camera.scale_z = 1;

    camera.rotate_x = 0;
    camera.rotate_y = 0;
    camera.rotate_z = 0;

    camera.right_side_frustum = 0.1f;
    camera.top_side_frustum = 0.1f;
    camera.near_side_frustum = 0.1f;
    camera.front_side_frustum = 10.0f;

    update_view_matrix(&camera);
    update_projection_matrix(&camera);
    update_view_projection_matrix(&camera);

    // printf("<<<delete_drawable_object finished\n");

    return camera;
}


void send_view_projection_matrix_to_shaders(Camera* camera, unsigned int shader_program)
{
    // send model matrix to shaders
    glUseProgram(shader_program);
    unsigned int view_projection_matrix_location = glGetUniformLocation(shader_program, "view_projection_matrix");
    glUniformMatrix4fv(view_projection_matrix_location, 1, GL_TRUE, get_view_projection_matrix(camera).matrix);
}


void update_view_matrix(Camera* camera)
{
    // Make view matrix
    Matrix translate_matrix = get_translate_matrix_to_view(camera);
    Matrix scale_matrix = get_scale_matrix_to_view(camera);
    Matrix rotate_matrix = get_rotate_matrix_to_view(camera);
    Matrix view_matrix = create_view_matrix(&translate_matrix, &scale_matrix, &rotate_matrix);

    camera->view_matrix = view_matrix;
}


Matrix get_translate_matrix_to_view(Camera* camera)
{
    Matrix translate_matrix = create_matrix(4, 4);

    set_matrix_element(&translate_matrix, 0, 0, 1.0f);
    set_matrix_element(&translate_matrix, 0, 1, 0.0f);
    set_matrix_element(&translate_matrix, 0, 2, 0.0f);
    set_matrix_element(&translate_matrix, 0, 3, camera->x);
    set_matrix_element(&translate_matrix, 1, 0, 0.0f);
    set_matrix_element(&translate_matrix, 1, 1, 1.0f);
    set_matrix_element(&translate_matrix, 1, 2, 0.0f);
    set_matrix_element(&translate_matrix, 1, 3, camera->y);
    set_matrix_element(&translate_matrix, 2, 0, 0.0f);
    set_matrix_element(&translate_matrix, 2, 1, 0.0f);
    set_matrix_element(&translate_matrix, 2, 2, 1.0f);
    set_matrix_element(&translate_matrix, 2, 3, camera->z);
    set_matrix_element(&translate_matrix, 3, 0, 0.0f);
    set_matrix_element(&translate_matrix, 3, 1, 0.0f);
    set_matrix_element(&translate_matrix, 3, 2, 0.0f);
    set_matrix_element(&translate_matrix, 3, 3, 1.0f);

    return translate_matrix;
}

Matrix get_scale_matrix_to_view(Camera* camera)
{
    Matrix scale_matrix = create_matrix(4, 4);

    set_matrix_element(&scale_matrix, 0, 0, camera->scale_x);
    set_matrix_element(&scale_matrix, 0, 1, 0.0f);
    set_matrix_element(&scale_matrix, 0, 2, 0.0f);
    set_matrix_element(&scale_matrix, 0, 3, 0.0f);
    set_matrix_element(&scale_matrix, 1, 0, 0.0f);
    set_matrix_element(&scale_matrix, 1, 1, camera->scale_y);
    set_matrix_element(&scale_matrix, 1, 2, 0.0f);
    set_matrix_element(&scale_matrix, 1, 3, 0.0f);
    set_matrix_element(&scale_matrix, 2, 0, 0.0f);
    set_matrix_element(&scale_matrix, 2, 1, 0.0f);
    set_matrix_element(&scale_matrix, 2, 2, camera->scale_z);
    set_matrix_element(&scale_matrix, 2, 3, 0.0f);
    set_matrix_element(&scale_matrix, 3, 0, 0.0f);
    set_matrix_element(&scale_matrix, 3, 1, 0.0f);
    set_matrix_element(&scale_matrix, 3, 2, 0.0f);
    set_matrix_element(&scale_matrix, 3, 3, 1.0f);

    return scale_matrix;
}

Matrix get_rotate_matrix_to_view(Camera* camera)
{
    Matrix rotate_x_matrix = get_rotate_x_matrix_to_view(camera);
    Matrix rotate_y_matrix = get_rotate_y_matrix_to_view(camera);
    Matrix rotate_z_matrix = get_rotate_z_matrix_to_view(camera);

    Matrix rotate_matrix = create_matrix(4, 4);
    rotate_matrix = multiply_matrices(&rotate_y_matrix, &rotate_x_matrix);
    rotate_matrix = multiply_matrices(&rotate_z_matrix, &rotate_matrix);

    return rotate_matrix;
}

Matrix get_rotate_x_matrix_to_view(Camera* camera)
{
    float rotate_x_in_radians = camera->rotate_x * M_PI / 180;

    Matrix rotate_x_matrix = create_matrix(4, 4);

    set_matrix_element(&rotate_x_matrix, 0, 0, 1.0f);
    set_matrix_element(&rotate_x_matrix, 0, 1, 0.0f);
    set_matrix_element(&rotate_x_matrix, 0, 2, 0.0f);
    set_matrix_element(&rotate_x_matrix, 0, 3, 0.0f);
    set_matrix_element(&rotate_x_matrix, 1, 0, 0.0f);
    set_matrix_element(&rotate_x_matrix, 1, 1, (float)cos(rotate_x_in_radians));
    set_matrix_element(&rotate_x_matrix, 1, 2, -(float)sin(rotate_x_in_radians));
    set_matrix_element(&rotate_x_matrix, 1, 3, 0.0f);
    set_matrix_element(&rotate_x_matrix, 2, 0, 0.0f);
    set_matrix_element(&rotate_x_matrix, 2, 1, (float)sin(rotate_x_in_radians));
    set_matrix_element(&rotate_x_matrix, 2, 2, (float)cos(rotate_x_in_radians));
    set_matrix_element(&rotate_x_matrix, 2, 3, 0.0f);
    set_matrix_element(&rotate_x_matrix, 3, 0, 0.0f);
    set_matrix_element(&rotate_x_matrix, 3, 1, 0.0f);
    set_matrix_element(&rotate_x_matrix, 3, 2, 0.0f);
    set_matrix_element(&rotate_x_matrix, 3, 3, 1.0f);

    return rotate_x_matrix;
}

Matrix get_rotate_y_matrix_to_view(Camera* camera)
{
    float rotate_y_in_radians = camera->rotate_y * M_PI / 180;

    Matrix rotate_y_matrix = create_matrix(4, 4);

    set_matrix_element(&rotate_y_matrix, 0, 0, (float)cos(rotate_y_in_radians));
    set_matrix_element(&rotate_y_matrix, 0, 1, 0.0f);
    set_matrix_element(&rotate_y_matrix, 0, 2, (float)sin(rotate_y_in_radians));
    set_matrix_element(&rotate_y_matrix, 0, 3, 0.0f);
    set_matrix_element(&rotate_y_matrix, 1, 0, 0.0f);
    set_matrix_element(&rotate_y_matrix, 1, 1, 1.0f);
    set_matrix_element(&rotate_y_matrix, 1, 2, 0.0f);
    set_matrix_element(&rotate_y_matrix, 1, 3, 0.0f);
    set_matrix_element(&rotate_y_matrix, 2, 0, -(float)sin(rotate_y_in_radians));
    set_matrix_element(&rotate_y_matrix, 2, 1, 0.0f);
    set_matrix_element(&rotate_y_matrix, 2, 2, (float)cos(rotate_y_in_radians));
    set_matrix_element(&rotate_y_matrix, 2, 3, 0.0f);
    set_matrix_element(&rotate_y_matrix, 3, 0, 0.0f);
    set_matrix_element(&rotate_y_matrix, 3, 1, 0.0f);
    set_matrix_element(&rotate_y_matrix, 3, 2, 0.0f);
    set_matrix_element(&rotate_y_matrix, 3, 3, 1.0f);

    return rotate_y_matrix;
}

Matrix get_rotate_z_matrix_to_view(Camera* camera)
{
    float rotate_z_in_radians = camera->rotate_z * M_PI / 180;

    Matrix rotate_z_matrix = create_matrix(4, 4);

    set_matrix_element(&rotate_z_matrix, 0, 0, (float)cos(rotate_z_in_radians));
    set_matrix_element(&rotate_z_matrix, 0, 1, -(float)sin(rotate_z_in_radians));
    set_matrix_element(&rotate_z_matrix, 0, 2, 0.0f);
    set_matrix_element(&rotate_z_matrix, 0, 3, 0.0f);
    set_matrix_element(&rotate_z_matrix, 1, 0, (float)sin(rotate_z_in_radians));
    set_matrix_element(&rotate_z_matrix, 1, 1, (float)cos(rotate_z_in_radians));
    set_matrix_element(&rotate_z_matrix, 1, 2, 0.0f);
    set_matrix_element(&rotate_z_matrix, 1, 3, 0.0f);
    set_matrix_element(&rotate_z_matrix, 2, 0, 0.0f);
    set_matrix_element(&rotate_z_matrix, 2, 1, 0.0f);
    set_matrix_element(&rotate_z_matrix, 2, 2, 1.0f);
    set_matrix_element(&rotate_z_matrix, 2, 3, 0.0f);
    set_matrix_element(&rotate_z_matrix, 3, 0, 0.0f);
    set_matrix_element(&rotate_z_matrix, 3, 1, 0.0f);
    set_matrix_element(&rotate_z_matrix, 3, 2, 0.0f);
    set_matrix_element(&rotate_z_matrix, 3, 3, 1.0f);

    return rotate_z_matrix;
}

Matrix create_view_matrix(Matrix *translate_matrix, Matrix *scale_matrix, Matrix *rotate_matrix)
{
    Matrix view_matrix = create_matrix(4, 4);

    view_matrix = multiply_matrices(scale_matrix, rotate_matrix);
    view_matrix = multiply_matrices(translate_matrix, &view_matrix);

    return view_matrix;
}


void update_projection_matrix(Camera* camera)
{
    Matrix projection_matrix = create_matrix(4, 4);

    set_matrix_element(&projection_matrix, 0, 0, camera->near_side_frustum / camera->right_side_frustum);
    set_matrix_element(&projection_matrix, 0, 1, 0.0f);
    set_matrix_element(&projection_matrix, 0, 2, 0.0f);
    set_matrix_element(&projection_matrix, 0, 3, 0.0f);
    set_matrix_element(&projection_matrix, 1, 0, 0.0f);
    set_matrix_element(&projection_matrix, 1, 1, camera->near_side_frustum / camera->top_side_frustum);
    set_matrix_element(&projection_matrix, 1, 2, 0.0f);
    set_matrix_element(&projection_matrix, 1, 3, 0.0f);
    set_matrix_element(&projection_matrix, 2, 0, 0.0f);
    set_matrix_element(&projection_matrix, 2, 1, 0.0f);
    set_matrix_element(&projection_matrix, 2, 2, -(camera->front_side_frustum + camera->near_side_frustum)/(camera->front_side_frustum - camera->near_side_frustum));
    set_matrix_element(&projection_matrix, 2, 3, -2*camera->front_side_frustum * camera->near_side_frustum/(camera->front_side_frustum - camera->near_side_frustum));
    set_matrix_element(&projection_matrix, 3, 0, 0.0f);
    set_matrix_element(&projection_matrix, 3, 1, 0.0f);
    set_matrix_element(&projection_matrix, 3, 2, -1.0f);
    set_matrix_element(&projection_matrix, 3, 3, 0.0f);

    camera->projection_matrix = projection_matrix;
}


void update_view_projection_matrix(Camera* camera)
{
    camera->view_projection_matrix = multiply_view_projection_matrices(camera);
}

Matrix multiply_view_projection_matrices(Camera* camera)
{
    Matrix view_projection_matrix;

    view_projection_matrix = multiply_matrices(&camera->projection_matrix, &camera->view_matrix);
    return view_projection_matrix;
}


Matrix get_view_matrix(Camera* camera)
{
    return camera->view_matrix;
}

Matrix get_projection_matrix(Camera* camera)
{
    return camera->projection_matrix;
}

Matrix get_view_projection_matrix(Camera* camera)
{
    return camera->view_projection_matrix;
}

void set_camera_coords(Camera* camera, float x, float y, float z)
{
    camera->x = x;
    camera->y = y;
    camera->z = z;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}

void set_camera_coord_x(Camera* camera, float x)
{
    camera->x = x;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}

void set_camera_coord_y(Camera* camera, float y)
{
    camera->y = y;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
    
}

void set_camera_coord_z(Camera* camera, float z)
{
    camera->z = z;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}


void set_camera_rotate(Camera* camera, float rotate_x, float rotate_y, float rotate_z)
{
    camera->rotate_x = rotate_x;
    camera->rotate_y = rotate_y;
    camera->rotate_z = rotate_z;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}

void set_camera_rotate_x(Camera* camera, float rotate_x)
{
    camera->rotate_x = rotate_x;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}

void set_camera_rotate_y(Camera* camera, float rotate_y)
{
    camera->rotate_y = rotate_y;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}

void set_camera_rotate_z(Camera* camera, float rotate_z)
{
    camera->rotate_z = rotate_z;

    update_view_matrix(camera);
    update_view_projection_matrix(camera);
}