#include "camera.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>


Matrix multiply_view_projection_matrices(Camera* camera);

void get_translate_matrix_to_view(Camera* camera);
void get_scale_matrix_to_view(Camera* camera);
void get_rotate_matrix_to_view(Camera* camera);
void get_rotate_x_matrix_to_view(Camera* camera);
void get_rotate_y_matrix_to_view(Camera* camera);
void get_rotate_z_matrix_to_view(Camera* camera);
void make_view_matrix(Camera* camera);


Camera create_camera()
{
    // printf(">>>delete_drawable_object started\n");

    Camera camera;

    camera.view_matrix = create_matrix(4, 4);
    camera.projection_matrix = create_matrix(4, 4);
    camera.view_projection_matrix = create_matrix(4, 4);

    camera._translate_matrix = create_matrix(4, 4);
    camera._scale_matrix = create_matrix(4, 4);
    camera._rotate_matrix = create_matrix(4, 4);

    camera._rotate_x_matrix = create_matrix(4, 4);
    camera._rotate_y_matrix = create_matrix(4, 4);
    camera._rotate_z_matrix = create_matrix(4, 4);

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
    camera.front_side_frustum = 100.0f;

    update_view_matrix(&camera);
    update_projection_matrix(&camera);
    update_view_projection_matrix(&camera);

    // printf("<<<delete_drawable_object finished\n");

    return camera;
}


void send_view_projection_matrix_to_shaders(Camera* camera, ShaderProgram shader_program)
{
    // send model matrix to shaders
    // update_view_projection_matrix(camera);

    glUseProgram(shader_program.shader_program);
    unsigned int view_projection_matrix_location = glGetUniformLocation(shader_program.shader_program, "view_projection_matrix");
    glUniformMatrix4fv(view_projection_matrix_location, 1, GL_TRUE, get_view_projection_matrix(camera).matrix);
}


void send_scene_parameters_to_shaders(Camera* camera, ShaderProgram shader_program)
{
    glUseProgram(shader_program.shader_program);

    glUniform3f(
        glGetUniformLocation(shader_program.shader_program, "camera_position"), 
        camera->x, camera->y, camera->z
    );

    glUniform3f(
        glGetUniformLocation(shader_program.shader_program, "light_position"), 
        0.0f, 10.0f, 0.0f
    );

    glUniform3f(
        glGetUniformLocation(shader_program.shader_program, "light_color"), 
        1.0f, 1.0f, 1.0f
    );

    glUniform1f(
        glGetUniformLocation(shader_program.shader_program, "ambient_factor"), 
        0.1f
    );

    glUniform1f(
        glGetUniformLocation(shader_program.shader_program, "diffuse_factor"), 
        1.0f
    );

    glUniform1f(
        glGetUniformLocation(shader_program.shader_program, "specular_factor"), 
        0.5f
    );
}


void update_view_matrix(Camera* camera)
{
    // Make view matrix
    get_translate_matrix_to_view(camera);
    get_scale_matrix_to_view(camera);
    get_rotate_matrix_to_view(camera);

    camera->view_matrix = multiply_matrices(&camera->_scale_matrix, &camera->_rotate_matrix);
    camera->view_matrix = multiply_matrices(&camera->_translate_matrix, &camera->view_matrix);

    // camera->view_matrix = create_view_matrix(&camera->_translate_matrix, &camera->_scale_matrix, &camera->_rotate_matrix);
}


void get_translate_matrix_to_view(Camera* camera)
{
    set_matrix_element(&camera->_translate_matrix, 0, 0, 1.0f);
    set_matrix_element(&camera->_translate_matrix, 0, 1, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 0, 2, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 0, 3, camera->x);
    set_matrix_element(&camera->_translate_matrix, 1, 0, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 1, 1, 1.0f);
    set_matrix_element(&camera->_translate_matrix, 1, 2, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 1, 3, camera->y);
    set_matrix_element(&camera->_translate_matrix, 2, 0, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 2, 1, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 2, 2, 1.0f);
    set_matrix_element(&camera->_translate_matrix, 2, 3, camera->z);
    set_matrix_element(&camera->_translate_matrix, 3, 0, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 3, 1, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 3, 2, 0.0f);
    set_matrix_element(&camera->_translate_matrix, 3, 3, 1.0f);
}

void get_scale_matrix_to_view(Camera* camera)
{
    set_matrix_element(&camera->_scale_matrix, 0, 0, camera->scale_x);
    set_matrix_element(&camera->_scale_matrix, 0, 1, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 0, 2, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 0, 3, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 1, 0, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 1, 1, camera->scale_y);
    set_matrix_element(&camera->_scale_matrix, 1, 2, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 1, 3, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 2, 0, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 2, 1, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 2, 2, camera->scale_z);
    set_matrix_element(&camera->_scale_matrix, 2, 3, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 3, 0, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 3, 1, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 3, 2, 0.0f);
    set_matrix_element(&camera->_scale_matrix, 3, 3, 1.0f);
}

void get_rotate_matrix_to_view(Camera* camera)
{
    // camera->_rotate_x_matrix = get_rotate_x_matrix_to_view(camera);
    get_rotate_x_matrix_to_view(camera);
    // camera->_rotate_y_matrix = get_rotate_y_matrix_to_view(camera);
    get_rotate_y_matrix_to_view(camera);
    // camera->_rotate_z_matrix = get_rotate_z_matrix_to_view(camera);
    get_rotate_z_matrix_to_view(camera);

    // camera->_rotate_matrix = create_matrix(4, 4);
    camera->_rotate_matrix = multiply_matrices(&camera->_rotate_y_matrix, &camera->_rotate_x_matrix);
    camera->_rotate_matrix = multiply_matrices(&camera->_rotate_z_matrix, &camera->_rotate_matrix);
}

void get_rotate_x_matrix_to_view(Camera* camera)
{
    float rotate_x_in_radians = camera->rotate_x * M_PI / 180;

    set_matrix_element(&camera->_rotate_x_matrix, 0, 0, 1.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 0, 1, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 0, 2, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 0, 3, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 1, 0, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 1, 1, (float)cos(rotate_x_in_radians));
    set_matrix_element(&camera->_rotate_x_matrix, 1, 2, -(float)sin(rotate_x_in_radians));
    set_matrix_element(&camera->_rotate_x_matrix, 1, 3, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 2, 0, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 2, 1, (float)sin(rotate_x_in_radians));
    set_matrix_element(&camera->_rotate_x_matrix, 2, 2, (float)cos(rotate_x_in_radians));
    set_matrix_element(&camera->_rotate_x_matrix, 2, 3, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 3, 0, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 3, 1, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 3, 2, 0.0f);
    set_matrix_element(&camera->_rotate_x_matrix, 3, 3, 1.0f);
}

void get_rotate_y_matrix_to_view(Camera* camera)
{
    float rotate_y_in_radians = camera->rotate_y * M_PI / 180;

    set_matrix_element(&camera->_rotate_y_matrix, 0, 0, (float)cos(rotate_y_in_radians));
    set_matrix_element(&camera->_rotate_y_matrix, 0, 1, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 0, 2, (float)sin(rotate_y_in_radians));
    set_matrix_element(&camera->_rotate_y_matrix, 0, 3, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 1, 0, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 1, 1, 1.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 1, 2, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 1, 3, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 2, 0, -(float)sin(rotate_y_in_radians));
    set_matrix_element(&camera->_rotate_y_matrix, 2, 1, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 2, 2, (float)cos(rotate_y_in_radians));
    set_matrix_element(&camera->_rotate_y_matrix, 2, 3, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 3, 0, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 3, 1, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 3, 2, 0.0f);
    set_matrix_element(&camera->_rotate_y_matrix, 3, 3, 1.0f);
}

void get_rotate_z_matrix_to_view(Camera* camera)
{
    float rotate_z_in_radians = camera->rotate_z * M_PI / 180;

    set_matrix_element(&camera->_rotate_z_matrix, 0, 0, (float)cos(rotate_z_in_radians));
    set_matrix_element(&camera->_rotate_z_matrix, 0, 1, -(float)sin(rotate_z_in_radians));
    set_matrix_element(&camera->_rotate_z_matrix, 0, 2, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 0, 3, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 1, 0, (float)sin(rotate_z_in_radians));
    set_matrix_element(&camera->_rotate_z_matrix, 1, 1, (float)cos(rotate_z_in_radians));
    set_matrix_element(&camera->_rotate_z_matrix, 1, 2, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 1, 3, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 2, 0, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 2, 1, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 2, 2, 1.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 2, 3, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 3, 0, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 3, 1, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 3, 2, 0.0f);
    set_matrix_element(&camera->_rotate_z_matrix, 3, 3, 1.0f);
}

// void make_view_matrix(Camera* camera)
// {
//     camera->view_matrix = multiply_matrices(&camera->_scale_matrix, &camera->_rotate_matrix);
//     camera->view_matrix = multiply_matrices(&camera->_translate_matrix, &camera->view_matrix);
// }


void update_projection_matrix(Camera* camera)
{
    set_matrix_element(&camera->projection_matrix, 0, 0, camera->near_side_frustum / camera->right_side_frustum);
    set_matrix_element(&camera->projection_matrix, 0, 1, 0.0f);
    set_matrix_element(&camera->projection_matrix, 0, 2, 0.0f);
    set_matrix_element(&camera->projection_matrix, 0, 3, 0.0f);
    set_matrix_element(&camera->projection_matrix, 1, 0, 0.0f);
    set_matrix_element(&camera->projection_matrix, 1, 1, camera->near_side_frustum / camera->top_side_frustum);
    set_matrix_element(&camera->projection_matrix, 1, 2, 0.0f);
    set_matrix_element(&camera->projection_matrix, 1, 3, 0.0f);
    set_matrix_element(&camera->projection_matrix, 2, 0, 0.0f);
    set_matrix_element(&camera->projection_matrix, 2, 1, 0.0f);
    set_matrix_element(&camera->projection_matrix, 2, 2, -(camera->front_side_frustum + camera->near_side_frustum)/(camera->front_side_frustum - camera->near_side_frustum));
    set_matrix_element(&camera->projection_matrix, 2, 3, -2*camera->front_side_frustum * camera->near_side_frustum/(camera->front_side_frustum - camera->near_side_frustum));
    set_matrix_element(&camera->projection_matrix, 3, 0, 0.0f);
    set_matrix_element(&camera->projection_matrix, 3, 1, 0.0f);
    set_matrix_element(&camera->projection_matrix, 3, 2, -1.0f);
    set_matrix_element(&camera->projection_matrix, 3, 3, 0.0f);
}


void update_view_projection_matrix(Camera* camera)
{
    // camera->view_projection_matrix = multiply_view_projection_matrices(camera);
    camera->view_projection_matrix = multiply_matrices(&camera->projection_matrix, &camera->view_matrix);
}

// Matrix multiply_view_projection_matrices(Camera* camera)
// {
//     Matrix view_projection_matrix;

//     view_projection_matrix = multiply_matrices(&camera->projection_matrix, &camera->view_matrix);
//     return view_projection_matrix;
// }


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