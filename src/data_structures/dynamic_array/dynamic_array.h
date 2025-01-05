#pragma once


typedef struct {
    int size;
    int capacity;
    float* array;
} DynamicArray;


DynamicArray create_dynamic_array(int capacity);

void darray_add_element_(DynamicArray* array, float element);
float darray_remove_element(DynamicArray* array, int index);

void print_dynamic_array(DynamicArray* array);

void free_array(DynamicArray* array);


// NOT WORK
// void set_element(DynamicArray* array, int index, int element);
// int get_element(DynamicArray* array, int index);
