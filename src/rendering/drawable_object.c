#include "drawable_object.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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


void render_drawable_object(DrawableObject* object, unsigned int shader_program)
{
    // printf(">>>render_drawable_object started\n");

    // draw our first triangle
    glUseProgram(shader_program);
    glBindVertexArray(object->VAO); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // printf("<<<render_drawable_object finished\n");
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