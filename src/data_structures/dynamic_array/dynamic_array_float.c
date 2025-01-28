#include "dynamic_array_float.h"

#include <stdio.h>
#include <stdlib.h>


DynamicArrayFloat create_darray_float(int capacity)
{
    DynamicArrayFloat array;

    if (capacity < 1)
    {
        fprintf(stderr, "[DynamicArrayFloat: create_dynamic_array]: can't create array with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }

    array.array = (float*)malloc(capacity * sizeof(float));
    array.size = 0;
    array.capacity = capacity;

    return array;
}


void free_darray_float(DynamicArrayFloat* array)
{
    free(array->array);
    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}


void add_element_darray_float(DynamicArrayFloat* array, float element)
{
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        array->array = (float*)realloc(array->array, array->capacity * sizeof(float));
    }

    array->array[array->size++] = element;
}


float remove_element_darray_float(DynamicArrayFloat* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayFloat: remove_element]: can't remove element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;

    float removed_element = array->array[current_index];
    
    for (int i = current_index; i < array->size - 1; i++)
    {
        array->array[i] = array->array[i + 1];
    }
    array->size--;

    return removed_element;
}


void print_darray_float(DynamicArrayFloat* array)
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


void clear_darray_float(DynamicArrayFloat* array)
{
    for (int i = 0; i < array->capacity; i++)
    {
        array->array[i] = 0;
    }

    array->size = 0;
}


float get_element_darray_float(DynamicArrayFloat* array, int index)
{
    if (index >= array->size)
    {
        printf("index = {%d}, size = {%d}", index, array->size);
        fprintf(stderr, "[DynamicArrayFloat: get_element]: can't get element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    return array->array[current_index];
}


void set_element_darray_float(DynamicArrayFloat* array, int index, float element)
{
    if (index >= array->size)
    {
        printf("index = {%d}, size = {%d}", index, array->size);
        fprintf(stderr, "[DynamicArrayFloat: set_element]: can't set element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    array->array[current_index] = element;
}
