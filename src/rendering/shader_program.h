#pragma once

typedef struct ShaderProgram {
    unsigned int shader_program;
} ShaderProgram;

ShaderProgram load_shaders(char* vertex_shader_filename, char* fragment_shader_filename);