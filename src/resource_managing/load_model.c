#include "load_model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../data_structures/dynamic_array/dynamic_array_float.h"
#include "../data_structures/dynamic_array/dynamic_array_int.h"
#include "../data_structures/dynamic_array/dynamic_array_mesh.h"
#include "../data_structures/dynamic_array/dynamic_array_material.h"
#include "../data_structures/array/array.h"


#define MAX_LINE_LENGTH 256


char* concat_strings(const char *s1, const char *s2);
char* cut_last_symbol(char *string);


Model3D load_OBJ(const char *filename, const char *path_to_file)
{
    // printf("load_OBJ started\n");

    char* file_full_name = concat_strings(path_to_file, filename);
    FILE *file = fopen(file_full_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        printf("name of MTL file: {%s}\n", file_full_name);
        exit(-1);
    }

    Model3D new_model;

    unsigned int number_of_meshes = 0;
    // Mesh meshes[100];
    DynamicArrayMesh meshes;
    meshes = create_darray_mesh(3);
    Mesh current_mesh;

    // Material materials[100];
    DynamicArrayMaterial materials;
    unsigned int number_of_materials = 0;
    materials = create_darray_material(3);
    Material current_material;
    DynamicArrayString material_names;
    material_names = create_darray_string(3);

    DynamicArrayInt indices;
    indices = create_darray_int(5);
    DynamicArrayFloat vertices;
    vertices = create_darray_float(5);

    DynamicArrayFloat vertex_coords;
    vertex_coords = create_darray_float(5);
    DynamicArrayFloat normals;
    normals = create_darray_float(5);
    DynamicArrayFloat texture_coords;
    texture_coords = create_darray_float(5);

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Have a line
        // printf("%s", line);

        char *token = strtok(line, " ");
        /*while (token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }*/

        if (strcmp(token, "mtllib") == 0)
        {
            token = strtok(NULL, " ");
            // printf("Имя файла MTL^ {%s}\n", token);
            // Здесь нужно обрезать символ переноса строки
            char* name_of_MTL = cut_last_symbol(token);
            load_MTL(name_of_MTL, path_to_file, &materials, &material_names);
            free(name_of_MTL);
        }

        else if (strcmp(token, "usemtl") == 0)
        {
            // Если уже есть Меш, то добавляем его в список
            if (number_of_meshes)
            {
                Mesh new_mesh;

                new_mesh.vertices = create_darray_float(5);
                for (int i = 0; i < vertices.size; i++)
                    add_element_darray_float(&new_mesh.vertices, get_element_darray_float(&vertices, i));
                clear_darray_float(&vertices);

                new_mesh.indices = create_darray_int(5);
                for (int i = 0; i < indices.size; i++)
                    add_element_darray_int(&new_mesh.indices, get_element_darray_int(&indices, i));
                clear_darray_int(&indices);

                new_mesh.material.alpha_channel = current_material.alpha_channel;
                new_mesh.material.ambient_color[0] = current_material.ambient_color[0];
                new_mesh.material.ambient_color[1] = current_material.ambient_color[1];
                new_mesh.material.ambient_color[2] = current_material.ambient_color[2];
                new_mesh.material.diffuse_color[0] = current_material.diffuse_color[0];
                new_mesh.material.diffuse_color[1] = current_material.diffuse_color[1];
                new_mesh.material.diffuse_color[2] = current_material.diffuse_color[2];
                new_mesh.material.specular_color[0] = current_material.specular_color[0];
                new_mesh.material.specular_color[1] = current_material.specular_color[1];
                new_mesh.material.specular_color[2] = current_material.specular_color[2];
                new_mesh.material.shininess = current_material.shininess;

                add_element_darray_mesh(&meshes, &new_mesh);
            }

            number_of_meshes += 1;

            token = strtok(NULL, " ");
            for (int i = 0; i < materials.size; i++)
            {
                if (strcmp(token, get_element_darray_string(&material_names, i)) == 0)
                    current_material = get_element_darray_material(&materials, i);
            }
        }

        else if (strcmp(token, "v") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr_1;
            float x = strtof(token, &endptr_1);
            if (endptr_1 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&vertex_coords, x);

            token = strtok(NULL, " ");
            char *endptr_2;
            float y = strtof(token, &endptr_2);
            if (endptr_2 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&vertex_coords, y);

            token = strtok(NULL, " ");
            char *endptr_3;
            float z = strtof(token, &endptr_3);
            if (endptr_3 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&vertex_coords, z);

            // printf("v: %.3f, %.3f, %.3f\n", x, y, z);
        }

        else if (strcmp(token, "vn") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr_1;
            float xn = strtof(token, &endptr_1);
            if (endptr_1 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&normals, xn);

            token = strtok(NULL, " ");
            char *endptr_2;
            float yn = strtof(token, &endptr_2);
            if (endptr_2 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&normals, yn);

            token = strtok(NULL, " ");
            char *endptr_3;
            float zn = strtof(token, &endptr_3);
            if (endptr_3 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&normals, zn);

            // printf("vn: %.3f, %.3f, %.3f\n", xn, yn, zn);
        }

        else if (strcmp(token, "vt") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr_1;
            float xt = strtof(token, &endptr_1);
            if (endptr_1 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&texture_coords, xt);

            token = strtok(NULL, " ");
            char *endptr_2;
            float yt = strtof(token, &endptr_2);
            if (endptr_2 == token) {
                printf("Conversion failed, no digits were found\n");
            }
            add_element_darray_float(&texture_coords, yt);

            // printf("vt: %.3f, %.3f\n", xt, yt);
        }

        else if (strcmp(token, "f") == 0)
        {
            token = strtok(NULL, " ");
            char v1_str[30];
            strcpy(v1_str, token);
            token = strtok(NULL, " ");
            char v2_str[30];
            strcpy(v2_str, token);
            token = strtok(NULL, " ");
            char v3_str[30];
            strcpy(v3_str, token);
            // printf("f: %s, %s, %s", v1_str, v2_str, v3_str);

            char* local_v[3] = {v1_str, v2_str, v3_str};
            for (int i = 0; i < 3; i++)
            {
                char *token = strtok(local_v[i], "/");
                char *endptr;
                // printf("token: %s\n", token);

                long int vertex_index = strtol(token, &endptr, 10) - 1;
                if (endptr == token) {
                    printf("Conversion failed, no digits were found\n");
                }

                token = strtok(NULL, "/");
                long int texture_index = strtol(token, &endptr, 10) - 1;
                if (endptr == token) {
                    printf("Conversion failed, no digits were found\n");
                }

                token = strtok(NULL, "/");
                long int normal_index = strtol(token, &endptr, 10) - 1;
                if (endptr == token) {
                    printf("Conversion failed, no digits were found\n");
                }

                // printf("vertex_index: %d, normal_index: %d, texture_index: %d\n", vertex_index, normal_index, texture_index);
                // printf("length vertex_coords: {%d}\n", vertex_coords.size);
                // printf("length normals: {%d}\n", normals.size);
                // printf("length texture_coords: {%d}\n", texture_coords.size);

                add_element_darray_float(&vertices, get_element_darray_float(&vertex_coords, 3 * vertex_index));
                add_element_darray_float(&vertices, get_element_darray_float(&vertex_coords, 3 * vertex_index + 1));
                add_element_darray_float(&vertices, get_element_darray_float(&vertex_coords, 3 * vertex_index + 2));

                add_element_darray_float(&vertices, get_element_darray_float(&normals, 3 * normal_index));
                add_element_darray_float(&vertices, get_element_darray_float(&normals, 3 * normal_index + 1));
                add_element_darray_float(&vertices, get_element_darray_float(&normals, 3 * normal_index + 2));

                add_element_darray_float(&vertices, get_element_darray_float(&texture_coords, 2 * texture_index));
                add_element_darray_float(&vertices, get_element_darray_float(&texture_coords, 2 * texture_index + 1));

                add_element_darray_int(&indices, indices.size);
            }
        }
    }

    if (number_of_meshes)
    {
        Mesh new_mesh;

        new_mesh.vertices = create_darray_float(5);
        for (int i = 0; i < vertices.size; i++)
            add_element_darray_float(&new_mesh.vertices, get_element_darray_float(&vertices, i));
        clear_darray_float(&vertices);

        new_mesh.indices = create_darray_int(5);
        for (int i = 0; i < indices.size; i++)
            add_element_darray_int(&new_mesh.indices, get_element_darray_int(&indices, i));
        clear_darray_int(&indices);

        new_mesh.material.alpha_channel = current_material.alpha_channel;
        new_mesh.material.ambient_color[0] = current_material.ambient_color[0];
        new_mesh.material.ambient_color[1] = current_material.ambient_color[1];
        new_mesh.material.ambient_color[2] = current_material.ambient_color[2];
        new_mesh.material.diffuse_color[0] = current_material.diffuse_color[0];
        new_mesh.material.diffuse_color[1] = current_material.diffuse_color[1];
        new_mesh.material.diffuse_color[2] = current_material.diffuse_color[2];
        new_mesh.material.specular_color[0] = current_material.specular_color[0];
        new_mesh.material.specular_color[1] = current_material.specular_color[1];
        new_mesh.material.specular_color[2] = current_material.specular_color[2];
        new_mesh.material.shininess = current_material.shininess;

        add_element_darray_mesh(&meshes, &new_mesh);
    }

    // printf("Before creating Model\n");

    // // Выведем все Мэши
    // for (int i = 0; i < meshes.size; i++)
    // {
    //     Mesh mesh = get_element_darray_mesh(&meshes, i);
    //     print_mesh(&mesh);
    // }

    new_model = create_model3D(&meshes);

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
    free(file_full_name);

    // printf("load_OBJ finished\n");

    return new_model;
}


void load_MTL(char *filename, const char *path_to_file, DynamicArrayMaterial* materials, DynamicArrayString* material_names)
{
    // printf("load_MTL started\n");
    // printf("name of MTL file: {%s}\n", filename);

    int number_of_materials = 0;
    char material_name[15];
    Material current_material;

    char* file_full_name = concat_strings(path_to_file, filename);
    FILE *file = fopen(file_full_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        printf("name of MTL file: {%s}\n", file_full_name);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {

        char *token = strtok(line, " ");

        if (strcmp(token, "newmtl") == 0)
        {
            if (number_of_materials)
            {
                // Добавляем готовый материал
                Material new_material;
                new_material.alpha_channel = 1.0f;
                new_material.ambient_color[0] = current_material.ambient_color[0];
                new_material.ambient_color[1] = current_material.ambient_color[1];
                new_material.ambient_color[2] = current_material.ambient_color[2];
                new_material.diffuse_color[0] = current_material.diffuse_color[0];
                new_material.diffuse_color[1] = current_material.diffuse_color[1];
                new_material.diffuse_color[2] = current_material.diffuse_color[2];
                new_material.specular_color[0] = current_material.specular_color[0];
                new_material.specular_color[1] = current_material.specular_color[1];
                new_material.specular_color[2] = current_material.specular_color[2];
                new_material.shininess = current_material.shininess;
                add_element_darray_material(materials, &new_material);

                // strcpy(material_names[number_of_materials-1], material_name);
                add_element_darray_string(material_names, material_name);

                // printf("Новый материал добавлен (%d):\n", number_of_materials);
                // print_material(&new_material);
            }

            number_of_materials += 1;

            token = strtok(NULL, " ");
            strcpy(material_name, token);    
        }

        else if (strcmp(token, "Ns") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr;
            int shininess = strtol(token, &endptr, 10);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.shininess = shininess;
        }

        else if (strcmp(token, "Ka") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr;
            float r = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.ambient_color[0] = r;

            token = strtok(NULL, " ");
            float g = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.ambient_color[1] = g;

            token = strtok(NULL, " ");
            float b = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.ambient_color[2] = b;

            // printf("ambiet_color added: (%f, %f, %f)", r, g, b);
        }

        else if (strcmp(token, "Kd") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr;
            // int r = strtol(token, &endptr, 10);
            float r = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.diffuse_color[0] = r;

            token = strtok(NULL, " ");
            // int g = strtol(token, &endptr, 10);
            float g = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.diffuse_color[1] = g;

            token = strtok(NULL, " ");
            float b = strtof(token, &endptr);
            // int b = strtol(token, &endptr, 10);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.diffuse_color[2] = b;

            // printf("diffuse_color added: (%f, %f, %f)", r, g, b);
        }

        else if (strcmp(token, "Ks") == 0)
        {
            token = strtok(NULL, " ");
            char *endptr;
            // int r = strtol(token, &endptr, 10);
            float r = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.specular_color[0] = r;

            token = strtok(NULL, " ");
            // int g = strtol(token, &endptr, 10);
            float g = strtof(token, &endptr);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.specular_color[1] = g;

            token = strtok(NULL, " ");
            float b = strtof(token, &endptr);
            // int b = strtol(token, &endptr, 10);
            if (endptr == token) {
                printf("Conversion failed, no digits were found\n");
            }
            current_material.specular_color[2] = b;

            // printf("specular_color added: (%f, %f, %f)", r, g, b);
        }

        else if (strcmp(token, "map_Kd") == 0)
        {
            // Здесь происходит загрузка картинки, но под это нужно пилить кучу функций, я не успею
        }
    
    }

    // Загружаем последний материал в массив
    if (number_of_materials)
        {
            // Добавляем готовый материал
            Material new_material;
            new_material.alpha_channel = 1.0f;
            new_material.ambient_color[0] = current_material.ambient_color[0];
            new_material.ambient_color[1] = current_material.ambient_color[1];
            new_material.ambient_color[2] = current_material.ambient_color[2];
            new_material.diffuse_color[0] = current_material.diffuse_color[0];
            new_material.diffuse_color[1] = current_material.diffuse_color[1];
            new_material.diffuse_color[2] = current_material.diffuse_color[2];
            new_material.specular_color[0] = current_material.specular_color[0];
            new_material.specular_color[1] = current_material.specular_color[1];
            new_material.specular_color[2] = current_material.specular_color[2];
            new_material.shininess = current_material.shininess;
            add_element_darray_material(materials, &new_material);

            // strcpy(material_names[number_of_materials-1], material_name);
            add_element_darray_string(material_names, material_name);

            // printf("Новый материал добавлен (%d)\n", number_of_materials);
            // print_material(&new_material);
        }

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
    free(file_full_name);

    // printf("load_MTL finished, number_of_materials = %d\n", materials->size);
}


char* concat_strings(const char *s1, const char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);                      

    char *result = malloc(len1 + len2 + 1);

    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);    

    return result;
}


char* cut_last_symbol(char *string)
{
    size_t len = strlen(string);
    char *result = malloc(len);
    memcpy(result, string, len);

    if (len > 0) {
        result[len - 1] = '\0'; // Replace the last character with null terminator
    }

    return result;
}