#pragma once


typedef struct DynamicArrayFloat {
    int size;
    int capacity;
    float* array;
} DynamicArrayFloat;


DynamicArrayFloat create_darray_float(int capacity);
void free_darray_float(DynamicArrayFloat* array);

void add_element_darray_float(DynamicArrayFloat* array, float element);
float remove_element_darray_float(DynamicArrayFloat* array, int index);

void print_darray_float(DynamicArrayFloat* array);
void clear_darray_float(DynamicArrayFloat* array);

float get_element_darray_float(DynamicArrayFloat* array, int index);
void set_element_darray_float(DynamicArrayFloat* array, int index, float element);
