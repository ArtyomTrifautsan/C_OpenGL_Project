#pragma once

#include <stdbool.h>


typedef struct {
    int size;
    int capacity;
    int* array;
} Array;


Array create_array(int capacity);
void free_array(Array* array);

void array_push_back(Array* array, int element);
int array_clear(Array* array);
bool is_in_array(Array* array, int element);

void print_array(Array* array);

void array_set_element(Array* array, int index, int element);
int array_get_element(Array* array, int index);

