#include "dynamic_array.h"

#include <stdio.h>
#include <stdlib.h>


DynamicArray create_darray(int capacity)
{
    DynamicArray array;

    if (capacity < 1)
    {
        fprintf(stderr, "[Dynamic Array: create_dynamic_array]: can't create array with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }

    array.array = (float*)malloc(capacity * sizeof(float));
    array.size = 0;
    array.capacity = capacity;

    return array;
}

void darray_add_element(DynamicArray* array, float element)
{
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        array->array = (float*)realloc(array->array, array->capacity * sizeof(float));
    }

    array->array[array->size++] = element;
}

float darray_remove_element(DynamicArray* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[Dynamic Array: remove_element]: can't remove element: index out of range.\n");
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

float darray_get_element(DynamicArray* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[Dynamic Array: get_element]: can't get element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    return array->array[current_index];
}
void darray_set_element(DynamicArray* array, int index, int element)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[Dynamic Array: set_element]: can't set element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    array->array[current_index] = element;
}

void print_darray(DynamicArray* array)
{
    printf("[");
    
    if (array->size > 0)
    {
        // printf("%.3f", get_element(array, 0));
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

void free_darray(DynamicArray* array)
{
    free(array->array);
    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}