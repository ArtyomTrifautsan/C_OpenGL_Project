#include "dynamic_array_int.h"

#include <stdio.h>
#include <stdlib.h>


DynamicArrayInt create_darray_int(int capacity)
{
    DynamicArrayInt array;

    if (capacity < 1)
    {
        fprintf(stderr, "[DynamicArrayInt: create_dynamic_array]: can't create array with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }

    array.array = (int*)malloc(capacity * sizeof(int));
    array.size = 0;
    array.capacity = capacity;

    return array;
}


void free_darray_int(DynamicArrayInt* array)
{
    free(array->array);
    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}


void add_element_darray_int(DynamicArrayInt* array, int element)
{
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        array->array = (int*)realloc(array->array, array->capacity * sizeof(int));
    }

    array->array[array->size++] = element;
}


int remove_element_darray_int(DynamicArrayInt* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayInt: remove_element]: can't remove element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;

    int removed_element = array->array[current_index];
    
    for (int i = current_index; i < array->size - 1; i++)
    {
        array->array[i] = array->array[i + 1];
    }
    array->size--;

    return removed_element;
}


void print_darray_int(DynamicArrayInt* array)
{
    printf("[");
    
    if (array->size > 0)
    {
        printf("%d", array->array[0]);
    }

    if (array->size > 1)
    {
        for (int i = 1; i < array->size; i++)
        {
            printf(", %d", array->array[i]);
        }
    }

    printf("]\n");
}


void clear_darray_int(DynamicArrayInt* array)
{
    for (int i = 0; i < array->capacity; i++)
    {
        array->array[i] = 0;
    }

    array->size = 0;
}


int get_element_darray_int(DynamicArrayInt* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayInt: get_element]: can't get element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    return array->array[current_index];
}


void set_element_darray_int(DynamicArrayInt* array, int index, int element)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayInt: set_element]: can't set element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    array->array[current_index] = element;
}
