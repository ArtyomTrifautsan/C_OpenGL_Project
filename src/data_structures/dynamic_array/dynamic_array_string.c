#include "dynamic_array_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


DynamicArrayString create_darray_string(int capacity)
{
    DynamicArrayString array;

    array.size = 0;
    array.capacity = capacity;
    array.size_of_string = 15;

    if (capacity < 1)
    {
        fprintf(stderr, "[DynamicArrayString: create_dynamic_array]: can't create array with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }
    
    array.array = (char**)malloc(capacity * sizeof(char*));
    for (int i = 0; i < capacity; i++) {
        array.array[i] = (char*)malloc(array.size_of_string * sizeof(char));
    }

    return array;
}


void free_darray_string(DynamicArrayString* array)
{
    for (int i = 0; i < array->capacity; i++) {
        free(array->array[i]);
    }
    free(array->array);
    array->size = 0;
    array->capacity = 0;
}


void add_element_darray_string(DynamicArrayString* array, char* element)
{
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        array->array = (char**)realloc(array->array, array->capacity * sizeof(char*));
        for (int i = array->size; i < array->capacity; i++) {
            array->array[i] = (char*)malloc(array->size_of_string * sizeof(char));
        }
    }
    strncpy(array->array[array->size], element, array->size_of_string);
    array->size++;
}


void remove_element_darray_string(DynamicArrayString* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayString: remove_element]: can't remove element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;

    for (int i = index; i < array->size - 1; i++) {
        strncpy(array->array[i], array->array[i + 1], array->size_of_string);
    }
    array->size--;
}


void print_darray_string(DynamicArrayString* array)
{
    printf("[");

    if (array->size > 0)
    {
        printf("%s", array->array[0]);
    }

    if (array->size > 1)
    {
        for (int i = 1; i < array->size; i++)
        {
            printf(", %s", array->array[i]);
        }
    }

    printf("]\n");
}


void clear_darray_string(DynamicArrayString* array)
{
    array->size = 0;
}


char* get_element_darray_string(DynamicArrayString* array, int index)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayString: get_element]: can't get element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    return array->array[current_index];
}


void set_element_darray_string(DynamicArrayString* array, int index, char* element)
{
    if (index >= array->size)
    {
        fprintf(stderr, "[DynamicArrayString: set_element]: can't set element: index out of range.\n");
        exit(EXIT_FAILURE);
    }

    int current_index;
    if (index < 0)
        current_index = array->size + index;
    else
        current_index = index;
    
    strncpy(array->array[index], element, array->size_of_string);
}
