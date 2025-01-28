#pragma once


typedef struct DynamicArrayInt {
    int size;
    int capacity;
    int* array;
} DynamicArrayInt;


DynamicArrayInt create_darray_int(int capacity);
void free_darray_int(DynamicArrayInt* array);

void add_element_darray_int(DynamicArrayInt* array, int element);
int remove_element_darray_int(DynamicArrayInt* array, int index);

void print_darray_int(DynamicArrayInt* array);
void clear_darray_int(DynamicArrayInt* array);

int get_element_darray_int(DynamicArrayInt* array, int index);
void set_element_darray_int(DynamicArrayInt* array, int index, int element);
