#include "Model3D.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


void _send_model_matrix_to_shaders(Model3D* model, ShaderProgram shader_program);
// void _draw_model(Model3D* model, ShaderProgram shader_program);
void _draw_model3D(Model3D* model, ShaderProgram shader_program);

Matrix get_translate_matrix_to_model(Model3D* model);
Matrix get_scale_matrix_to_model(Model3D* model);
Matrix get_rotate_matrix_to_model(Model3D* model);
Matrix get_rotate_x_matrix_to_model(Model3D* model);
Matrix get_rotate_y_matrix_to_model(Model3D* model);
Matrix get_rotate_z_matrix_to_model(Model3D* model);
Matrix create_model_matrix(Matrix *translate_matrix, Matrix *scale_matrix, Matrix *rotate_matrix);


/*
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 vertex_position;\n"
    "uniform mat4 model_matrix;\n"
    "uniform mat4 view_projection_matrix;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};
*/

Model3D create_model3D(DynamicArrayMesh* meshes)
{
    // printf(">>>create_model3D started, length meshes = {%d}\n", meshes->size);
    Model3D model;
    model.meshes = create_darray_mesh(3);

    Mesh aux_mesh;
    for (int i = 0; i < meshes->size; i++)
    {
        // printf("i = {%d}\n", i);
        aux_mesh = get_element_darray_mesh(meshes, i);
        // printf("aux_mesh copied\n");
        add_element_darray_mesh(&model.meshes, &aux_mesh);
    }
    // printf("Meshes copied\n");
    // load_model_vertices(model);

    _set_render_properties(&model);
    // printf("_set_render_properties completed\n");
    _set_default_position(&model);
    // printf("_set_default_position completed\n");

    // printf("<<<create_model3D finished\n");

    return model;
}


void _set_render_properties(Model3D* model)
{
    // printf(">>>set_render_properties started\n");

    Mesh* p_mesh = model->meshes.array;
    for (int i = 0; i < model->meshes.size; i++)
    {
        glGenVertexArrays(1, &p_mesh->VAO);
        glGenBuffers(1, &p_mesh->VBO);
        glGenBuffers(1, &p_mesh->EBO);

        glBindVertexArray(p_mesh->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, p_mesh->VBO); 
        glBufferData(GL_ARRAY_BUFFER, p_mesh->vertices.size * sizeof(float), p_mesh->vertices.array, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_mesh->indices.size * sizeof(unsigned int), p_mesh->indices.array, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);

        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)3);

        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)6);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glBindVertexArray(0); 

        p_mesh++;
    }

    // printf("<<<set_render_properties finished\n");
}


void _set_default_position(Model3D* model)
{
    model->x = 0;
    model->y = 0;
    model->z = 0;

    model->scale_x = 1;
    model->scale_y = 1;
    model->scale_z = 1;

    model->rotate_x = 0;
    model->rotate_y = 0;
    model->rotate_z = 0;

    update_model_matrix(model);
}


void render_model3D(Model3D* model, ShaderProgram shader_program)
{
    // printf(">>>render_drawable_object started\n");

    _send_model_matrix_to_shaders(model, shader_program);

    _draw_model3D(model, shader_program);

    // printf("<<<render_drawable_object finished\n");
}


void _send_model_matrix_to_shaders(Model3D* model, ShaderProgram shader_program)
{
    // send model matrix to shaders
    glUseProgram(shader_program.shader_program);
    unsigned int transform_location = glGetUniformLocation(shader_program.shader_program, "model_matrix");
    glUniformMatrix4fv(transform_location, 1, GL_TRUE, model->model_matrix.matrix);
}


void _send_material_to_shaders(Material* material, ShaderProgram shader_program)
{
    glUseProgram(shader_program.shader_program);

    // unsigned int ambient_color_location = glGetUniformLocation(shader_program.shader_program, "ambient_color");
    glUniform3f(
        glGetUniformLocation(shader_program.shader_program, "ambient_color"), 
        material->ambient_color[0], material->ambient_color[1], material->ambient_color[2]
    );

    glUniform3f(
        glGetUniformLocation(shader_program.shader_program, "diffuse_color"), 
        material->diffuse_color[0], material->diffuse_color[1], material->diffuse_color[2]
    );

    glUniform3f(
        glGetUniformLocation(shader_program.shader_program, "specular_color"), 
        material->specular_color[0], material->specular_color[1], material->specular_color[2]
    );

    glUniform1f(
        glGetUniformLocation(shader_program.shader_program, "shininess"), 
        material->shininess
    );

    glUniform1f(
        glGetUniformLocation(shader_program.shader_program, "alpha_channel"), 
        material->alpha_channel
    );
}


void _draw_model3D(Model3D* model, ShaderProgram shader_program)
{
    // draw object
    Mesh* p_mesh = model->meshes.array;
    for (int i = 0; i < model->meshes.size; i++)
    {
        _send_material_to_shaders(&p_mesh->material, shader_program);

        glUseProgram(shader_program.shader_program);
        glBindVertexArray(p_mesh->VAO); 
        glDrawElements(GL_TRIANGLES, p_mesh->indices.size, GL_UNSIGNED_INT, NULL);

        p_mesh++;
    }
}


Matrix get_translate_matrix_to_model(Model3D* model)
{
    Matrix translate_matrix = create_matrix(4, 4);

    set_matrix_element(&translate_matrix, 0, 0, 1.0f);
    set_matrix_element(&translate_matrix, 0, 1, 0.0f);
    set_matrix_element(&translate_matrix, 0, 2, 0.0f);
    set_matrix_element(&translate_matrix, 0, 3, model->x);
    set_matrix_element(&translate_matrix, 1, 0, 0.0f);
    set_matrix_element(&translate_matrix, 1, 1, 1.0f);
    set_matrix_element(&translate_matrix, 1, 2, 0.0f);
    set_matrix_element(&translate_matrix, 1, 3, model->y);
    set_matrix_element(&translate_matrix, 2, 0, 0.0f);
    set_matrix_element(&translate_matrix, 2, 1, 0.0f);
    set_matrix_element(&translate_matrix, 2, 2, 1.0f);
    set_matrix_element(&translate_matrix, 2, 3, model->z);
    set_matrix_element(&translate_matrix, 3, 0, 0.0f);
    set_matrix_element(&translate_matrix, 3, 1, 0.0f);
    set_matrix_element(&translate_matrix, 3, 2, 0.0f);
    set_matrix_element(&translate_matrix, 3, 3, 1.0f);

    return translate_matrix;
}

Matrix get_scale_matrix_to_model(Model3D* model)
{
    Matrix scale_matrix = create_matrix(4, 4);

    set_matrix_element(&scale_matrix, 0, 0, model->scale_x);
    set_matrix_element(&scale_matrix, 0, 1, 0.0f);
    set_matrix_element(&scale_matrix, 0, 2, 0.0f);
    set_matrix_element(&scale_matrix, 0, 3, 0.0f);
    set_matrix_element(&scale_matrix, 1, 0, 0.0f);
    set_matrix_element(&scale_matrix, 1, 1, model->scale_y);
    set_matrix_element(&scale_matrix, 1, 2, 0.0f);
    set_matrix_element(&scale_matrix, 1, 3, 0.0f);
    set_matrix_element(&scale_matrix, 2, 0, 0.0f);
    set_matrix_element(&scale_matrix, 2, 1, 0.0f);
    set_matrix_element(&scale_matrix, 2, 2, model->scale_z);
    set_matrix_element(&scale_matrix, 2, 3, 0.0f);
    set_matrix_element(&scale_matrix, 3, 0, 0.0f);
    set_matrix_element(&scale_matrix, 3, 1, 0.0f);
    set_matrix_element(&scale_matrix, 3, 2, 0.0f);
    set_matrix_element(&scale_matrix, 3, 3, 1.0f);

    return scale_matrix;
}

Matrix get_rotate_matrix_to_model(Model3D* model)
{
    Matrix rotate_x_matrix = get_rotate_x_matrix_to_model(model);
    Matrix rotate_y_matrix = get_rotate_y_matrix_to_model(model);
    Matrix rotate_z_matrix = get_rotate_z_matrix_to_model(model);

    Matrix rotate_matrix = create_matrix(4, 4);
    rotate_matrix = multiply_matrices(&rotate_y_matrix, &rotate_x_matrix);
    rotate_matrix = multiply_matrices(&rotate_z_matrix, &rotate_matrix);

    return rotate_matrix;
}

Matrix get_rotate_x_matrix_to_model(Model3D* model)
{
    float rotate_x_in_radians = model->rotate_x * M_PI / 180;

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

Matrix get_rotate_y_matrix_to_model(Model3D* model)
{
    float rotate_y_in_radians = model->rotate_y * M_PI / 180;

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

Matrix get_rotate_z_matrix_to_model(Model3D* model)
{
    float rotate_z_in_radians = model->rotate_z * M_PI / 180;

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

Matrix create_model_matrix(Matrix *translate_matrix, Matrix *scale_matrix, Matrix *rotate_matrix)
{
    Matrix model_matrix = create_matrix(4, 4);

    model_matrix = multiply_matrices(scale_matrix, rotate_matrix);
    model_matrix = multiply_matrices(translate_matrix, &model_matrix);

    return model_matrix;
}


void update_model_matrix(Model3D* model)
{
    // Make model matrix
    Matrix translate_matrix = get_translate_matrix_to_model(model);
    Matrix scale_matrix = get_scale_matrix_to_model(model);
    Matrix rotate_matrix = get_rotate_matrix_to_model(model);
    Matrix model_matrix = create_model_matrix(&translate_matrix, &scale_matrix, &rotate_matrix);

    model->model_matrix = model_matrix;
}


Matrix get_model_matrix(Model3D* model)
{
    return model->model_matrix;
}


void set_model_coords(Model3D* model, float x, float y, float z)
{
    model->x = x;
    model->y = y;
    model->z = z;

    update_model_matrix(model);
}
void set_model_coord_x(Model3D* model, float x)
{
    model->x = x;

    update_model_matrix(model);
}

void set_model_coord_y(Model3D* model, float y)
{
    model->y = y;

    update_model_matrix(model);
}

void set_model_coord_z(Model3D* model, float z)
{
    model->z = z;

    update_model_matrix(model);
}


void set_model_scale(Model3D* model, float scale_x, float scale_y, float scale_z)
{
    model->scale_x = scale_x;
    model->scale_y = scale_y;
    model->scale_z = scale_z;

    update_model_matrix(model);
}

void set_model_scale_x(Model3D* model, float scale_x)
{
    model->scale_x = scale_x;

    update_model_matrix(model);
}

void set_model_scale_y(Model3D* model, float scale_y)
{
    model->scale_y = scale_y;

    update_model_matrix(model);
}

void set_model_scale_z(Model3D* model, float scale_z)
{
    model->scale_z = scale_z;

    update_model_matrix(model);
}


void set_model_rotate(Model3D* model, float rotate_x, float rotate_y, float rotate_z)
{
    model->rotate_x = rotate_x;
    model->rotate_y = rotate_y;
    model->rotate_z = rotate_z;

    update_model_matrix(model);
}

void set_model_rotate_x(Model3D* model, float rotate_x)
{
    model->rotate_x = rotate_x;

    update_model_matrix(model);
}

void set_model_rotate_y(Model3D* model, float rotate_y)
{
    model->rotate_y = rotate_y;

    update_model_matrix(model);
}

void set_model_rotate_z(Model3D* model, float rotate_z)
{
    model->rotate_z = rotate_z;

    update_model_matrix(model);
}
