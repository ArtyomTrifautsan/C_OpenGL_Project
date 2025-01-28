#pragma once


typedef struct Material
{
    char* name;
    float ambient_color[3];
    float diffuse_color[3];
    float specular_color[3];
    int shininess;
    char* diffuse_map;
    float alpha_channel;
} Material;


void print_material(Material* material);
