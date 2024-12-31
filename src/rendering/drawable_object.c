#include "drawable_object.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../middleware/algebra/algebra.h"


void send_model_matrix_to_shaders(DrawableObject* object, unsigned int shader_program);
void draw_object(DrawableObject* object, unsigned int shader_program);

Matrix get_translate_matrix_to_model(DrawableObject* object);
Matrix get_scale_matrix_to_model(DrawableObject* object);
Matrix get_rotate_matrix_to_model(DrawableObject* object);
Matrix get_rotate_x_matrix_to_model(DrawableObject* object);
Matrix get_rotate_y_matrix_to_model(DrawableObject* object);
Matrix get_rotate_z_matrix_to_model(DrawableObject* object);
Matrix create_model_matrix(Matrix *translate_matrix, Matrix *scale_matrix, Matrix *rotate_matrix);


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


unsigned int load_shaders()
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


void create_drawable_object(DrawableObject* object)
{
    // printf(">>>load_object_vertices started\n");

    load_object_vertices(object);
    set_render_properties(object);
    set_default_position(object);

    // printf("<<<load_object_vertices finished\n");
}

void load_object_vertices(DrawableObject* object)
{
    // printf(">>>load_object_vertices started\n");

    object->n_vertices = sizeof(vertices) / sizeof(float);
    object->n_indices = sizeof(indices) / sizeof(unsigned int);

    object->vertices = (float *)malloc(object->n_vertices * sizeof(float)); // 8 вершин по 3 значения (x, y, z)
    object->indices = (unsigned int *)malloc(object->n_indices * sizeof(unsigned int)); // 36 индексов для 12 треугольников

    for (int i = 0; i < object->n_vertices; i++) {
        object->vertices[i] = vertices[i];
    }

    for (int i = 0; i < object->n_indices; i++) {
        object->indices[i] = indices[i];
    }

    // printf("<<<load_object_vertices finished\n");
}

void set_render_properties(DrawableObject* object)
{
    // printf(">>>set_render_properties started\n");

    glGenVertexArrays(1, &object->VAO);
    glGenBuffers(1, &object->VBO);
    glGenBuffers(1, &object->EBO);

    glBindVertexArray(object->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
    glBufferData(GL_ARRAY_BUFFER, object->n_vertices * sizeof(float), object->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->n_indices * sizeof(unsigned int), object->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    // printf("<<<set_render_properties finished\n");
}


void set_default_position(DrawableObject* object)
{
    object->x = 0;
    object->y = 0;
    object->z = 0;

    object->scale_x = 1;
    object->scale_y = 1;
    object->scale_z = 1;

    object->rotate_x = 0;
    object->rotate_y = 0;
    object->rotate_z = 0;

    update_model_matrix(object);
}


void render_drawable_object(DrawableObject* object, unsigned int shader_program)
{
    // printf(">>>render_drawable_object started\n");

    send_model_matrix_to_shaders(object, shader_program);

    draw_object(object, shader_program);

    // printf("<<<render_drawable_object finished\n");
}

void send_model_matrix_to_shaders(DrawableObject* object, unsigned int shader_program)
{
    // send model matrix to shaders
    glUseProgram(shader_program);
    unsigned int transform_location = glGetUniformLocation(shader_program, "model_matrix");
    glUniformMatrix4fv(transform_location, 1, GL_TRUE, object->model_matrix.matrix);
}

void draw_object(DrawableObject* object, unsigned int shader_program)
{
    // draw object
    glUseProgram(shader_program);
    glBindVertexArray(object->VAO); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


Matrix get_translate_matrix_to_model(DrawableObject* object)
{
    Matrix translate_matrix = create_matrix(4, 4);

    set_matrix_element(&translate_matrix, 0, 0, 1.0f);
    set_matrix_element(&translate_matrix, 0, 1, 0.0f);
    set_matrix_element(&translate_matrix, 0, 2, 0.0f);
    set_matrix_element(&translate_matrix, 0, 3, object->x);
    set_matrix_element(&translate_matrix, 1, 0, 0.0f);
    set_matrix_element(&translate_matrix, 1, 1, 1.0f);
    set_matrix_element(&translate_matrix, 1, 2, 0.0f);
    set_matrix_element(&translate_matrix, 1, 3, object->y);
    set_matrix_element(&translate_matrix, 2, 0, 0.0f);
    set_matrix_element(&translate_matrix, 2, 1, 0.0f);
    set_matrix_element(&translate_matrix, 2, 2, 1.0f);
    set_matrix_element(&translate_matrix, 2, 3, object->z);
    set_matrix_element(&translate_matrix, 3, 0, 0.0f);
    set_matrix_element(&translate_matrix, 3, 1, 0.0f);
    set_matrix_element(&translate_matrix, 3, 2, 0.0f);
    set_matrix_element(&translate_matrix, 3, 3, 1.0f);

    return translate_matrix;
}

Matrix get_scale_matrix_to_model(DrawableObject* object)
{
    Matrix scale_matrix = create_matrix(4, 4);

    set_matrix_element(&scale_matrix, 0, 0, object->scale_x);
    set_matrix_element(&scale_matrix, 0, 1, 0.0f);
    set_matrix_element(&scale_matrix, 0, 2, 0.0f);
    set_matrix_element(&scale_matrix, 0, 3, 0.0f);
    set_matrix_element(&scale_matrix, 1, 0, 0.0f);
    set_matrix_element(&scale_matrix, 1, 1, object->scale_y);
    set_matrix_element(&scale_matrix, 1, 2, 0.0f);
    set_matrix_element(&scale_matrix, 1, 3, 0.0f);
    set_matrix_element(&scale_matrix, 2, 0, 0.0f);
    set_matrix_element(&scale_matrix, 2, 1, 0.0f);
    set_matrix_element(&scale_matrix, 2, 2, object->scale_z);
    set_matrix_element(&scale_matrix, 2, 3, 0.0f);
    set_matrix_element(&scale_matrix, 3, 0, 0.0f);
    set_matrix_element(&scale_matrix, 3, 1, 0.0f);
    set_matrix_element(&scale_matrix, 3, 2, 0.0f);
    set_matrix_element(&scale_matrix, 3, 3, 1.0f);

    return scale_matrix;
}

Matrix get_rotate_matrix_to_model(DrawableObject* object)
{
    Matrix rotate_x_matrix = get_rotate_x_matrix_to_model(object);
    Matrix rotate_y_matrix = get_rotate_y_matrix_to_model(object);
    Matrix rotate_z_matrix = get_rotate_z_matrix_to_model(object);

    Matrix rotate_matrix = create_matrix(4, 4);
    rotate_matrix = multiply_matrices(&rotate_y_matrix, &rotate_x_matrix);
    rotate_matrix = multiply_matrices(&rotate_z_matrix, &rotate_matrix);

    return rotate_matrix;
}

Matrix get_rotate_x_matrix_to_model(DrawableObject* object)
{
    float rotate_x_in_radians = object->rotate_x * M_PI / 180;

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

Matrix get_rotate_y_matrix_to_model(DrawableObject* object)
{
    float rotate_y_in_radians = object->rotate_y * M_PI / 180;

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

Matrix get_rotate_z_matrix_to_model(DrawableObject* object)
{
    float rotate_z_in_radians = object->rotate_z * M_PI / 180;

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

void update_model_matrix(DrawableObject* object)
{
    // Make model matrix
    Matrix translate_matrix = get_translate_matrix_to_model(object);
    Matrix scale_matrix = get_scale_matrix_to_model(object);
    Matrix rotate_matrix = get_rotate_matrix_to_model(object);
    Matrix model_matrix = create_model_matrix(&translate_matrix, &scale_matrix, &rotate_matrix);

    object->model_matrix = model_matrix;
}

Matrix get_model_matrix(DrawableObject* object)
{
    return object->model_matrix;
}

void delete_drawable_object(DrawableObject* object)
{
    // printf(">>>delete_drawable_object started\n");

    glDeleteVertexArrays(1, &object->VAO);
    glDeleteBuffers(1, &object->VBO);
    glDeleteBuffers(1, &object->EBO);

    free(object->vertices);
    free(object->indices);

    // printf("<<<delete_drawable_object finished\n");
}


void set_object_coords(DrawableObject* object, float x, float y, float z)
{
    object->x = x;
    object->y = y;
    object->z = z;

    update_model_matrix(object);
}
void set_object_coord_x(DrawableObject* object, float x)
{
    object->x = x;

    update_model_matrix(object);
}

void set_object_coord_y(DrawableObject* object, float y)
{
    object->y = y;

    update_model_matrix(object);
}

void set_object_coord_z(DrawableObject* object, float z)
{
    object->z = z;

    update_model_matrix(object);
}


void set_object_scale(DrawableObject* object, float scale_x, float scale_y, float scale_z)
{
    object->scale_x = scale_x;
    object->scale_y = scale_y;
    object->scale_z = scale_z;

    update_model_matrix(object);
}

void set_object_scale_x(DrawableObject* object, float scale_x)
{
    object->scale_x = scale_x;

    update_model_matrix(object);
}

void set_object_scale_y(DrawableObject* object, float scale_y)
{
    object->scale_y = scale_y;

    update_model_matrix(object);
}

void set_object_scale_z(DrawableObject* object, float scale_z)
{
    object->scale_z = scale_z;

    update_model_matrix(object);
}


void set_object_rotate(DrawableObject* object, float rotate_x, float rotate_y, float rotate_z)
{
    object->rotate_x = rotate_x;
    object->rotate_y = rotate_y;
    object->rotate_z = rotate_z;

    update_model_matrix(object);
}

void set_object_rotate_x(DrawableObject* object, float rotate_x)
{
    object->rotate_x = rotate_x;

    update_model_matrix(object);
}

void set_object_rotate_y(DrawableObject* object, float rotate_y)
{
    object->rotate_y = rotate_y;

    update_model_matrix(object);
}

void set_object_rotate_z(DrawableObject* object, float rotate_z)
{
    object->rotate_z = rotate_z;

    update_model_matrix(object);
}
