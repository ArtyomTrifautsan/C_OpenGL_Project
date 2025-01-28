#pragma once


typedef struct {
    int size;
    int capacity;
    float* array;
} DynamicArray;


DynamicArray create_darray(int capacity);
void free_darray(DynamicArray* array);

void darray_add_element(DynamicArray* array, float element);
float darray_remove_element(DynamicArray* array, int index);

void print_darray(DynamicArray* array);

void darray_set_element(DynamicArray* array, int index, float element);
float darray_get_element(DynamicArray* array, int index);
