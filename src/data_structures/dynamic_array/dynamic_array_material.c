#include "dynamic_array_material.h"

#include <stdio.h>
#include <stdlib.h>


DynamicArrayMaterial create_darray_material(int capacity)
{
    DynamicArrayMaterial array;

    if (capacity < 1)
    {
        fprintf(stderr, "[DynamicArrayMaterial: create_dynamic_array]: can't create array with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }

    array.array = (Material*)malloc(capacity * sizeof(Material));
    array.size = 0;
    array.capacity = capacity;

    return array;
}


void free_darray_material(DynamicArrayMaterial* array)
{
    free(array->array);
    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}


void add_element_darray_material(DynamicArrayMaterial* array, Material* element)
{
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        array->array = (Material*)realloc(array->array, array->capacity * sizeof(Material));
    }

    array->array[array->size++] = *element;
}


Material remove_element_darray_material(DynamicArrayMaterial* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayMaterial: remove_element]: can't remove element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;

    Material removed_element = array->array[current_index];
    
    for (int i = current_index; i < array->size - 1; i++)
    {
        array->array[i] = array->array[i + 1];
    }
    array->size--;

    return removed_element;
}

/*
void print_darray_mesh(DynamicArrayMaterial* array)
{
    printf("[");
    
    if (array->size > 0)
    {
        printf("%.3f", array->array[0]);
    }

    if (array->size > 1)
    {
        for (int i = 1; i < array->size; i++)
        {
            printf(", %.3f", array->array[i]);
        }
    }

    printf("]\n");
}
*/


void clear_darray_material(DynamicArrayMaterial* array)
{
    /*
    for (int i = 0; i < array->capacity; i++)
    {
        array->array[i] = 0;
    }
    */

    array->size = 0;
}


Material get_element_darray_material(DynamicArrayMaterial* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayMaterial: get_element]: can't get element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    return array->array[current_index];
}


void set_element_darray_material(DynamicArrayMaterial* array, int index, Material* element)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayMaterial: set_element]: can't set element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    array->array[current_index] = *element;
}
