#include "shader_program.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>


char* read_file(char* filename);

ShaderProgram load_shaders(char* vertex_shader_filename, char* fragment_shader_filename)
{
    // printf("load_shaders started\n");

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    char* vertex_shader_source = read_file(vertex_shader_filename);
    glShaderSource(vertex_shader, 1, (const char**)&vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }

    // fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader_source = read_file(fragment_shader_filename);
    glShaderSource(fragment_shader, 1, (const char**)&fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    // check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    // link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    ShaderProgram shader_program_structure;
    shader_program_structure.shader_program = shader_program;

    // printf("load_shaders finished\n");

    return shader_program_structure;
}


char* read_file(char* filename)
{
    // printf("read_file started\n");

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("ERROR::FILE::CANNOT_OPEN_FILE:\n");
        printf("%s\n", filename);
        return NULL;
    }

    // Seek to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory for the file content
    char* buffer = (char*)malloc(sizeof(char) * (fileSize + 1));
    if (!buffer) {
        printf("ERROR::MEMORY::CANNOT_ALLOCATE_MEMORY\n");
        fclose(file);
        return NULL;
    }

    // Read the file into the buffer
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0'; // Null-terminate the string

    fclose(file);

    // printf("read_file finished\n");

    return buffer;
}