#include "material.h"

#include "stdio.h"


void print_material(Material* material)
{
    printf("<Material>\n");
    printf("-ambient_color=(%.3f, %.3f, %.3f)\n", material->ambient_color[0], material->ambient_color[1], material->ambient_color[2]);
    printf("-diffuse_color=(%.3f, %.3f, %.3f)\n", material->diffuse_color[0], material->diffuse_color[1], material->diffuse_color[2]);
    printf("-specular_color=(%.3f, %.3f, %.3f)\n", material->specular_color[0], material->specular_color[1], material->specular_color[2]);
    printf("-shininess=%d\n", material->shininess);
    printf("-alpha_channel=%.3f\n", material->alpha_channel);
}