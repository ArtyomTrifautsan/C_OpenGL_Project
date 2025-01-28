#include "mesh.h"

#include "stdio.h"


void print_mesh(Mesh* mesh)
{
    printf("<Mesh>\n");
    printf("-vertices=");
    // print_darray_float(&mesh->vertices);
    printf("-indices=");
    // print_darray_int(&mesh->indices);
    printf("-Material=");
    print_material(&mesh->material);
}