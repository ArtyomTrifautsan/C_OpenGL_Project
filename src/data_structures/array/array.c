#include "array.h"

#include <stdio.h>
#include <stdlib.h>


Array create_array(int capacity)
{
    Array array;

    if (capacity < 1)
    {
        fprintf(stderr, "[Array: create_array]: can't create array with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }

    array.array = (int*)malloc(capacity * sizeof(int));
    array.size = 0;
    array.capacity = capacity;

    return array;
}

void free_array(Array* array)
{
    free(array->array);
    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}

void array_push_back(Array* array, int element)
{
    if (array->size == array->capacity)
    {
        array->capacity *= 2;
        array->array = (int*)realloc(array->array, array->capacity * sizeof(int));
    }

    array->array[array->size++] = element;
}

int array_clear(Array* array)
{
    for (int i = 0; i < array->capacity; i++)
    {
        array->array[i] = -3;
    }

    array->size = 0;
}

bool is_in_array(Array* array, int element)
{
    for (int i = 0; i < array->size; i++)
    {
        if (array->array[i] == element)
            return true;
    }

    return false;
}

void print_array(Array* array)
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