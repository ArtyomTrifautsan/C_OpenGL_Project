#pragma once


typedef struct DynamicArrayString {
    int size;
    int capacity;
    int size_of_string;
    char** array;
} DynamicArrayString;


DynamicArrayString create_darray_string(int capacity);
void free_darray_string(DynamicArrayString* array);

void add_element_darray_string(DynamicArrayString* array, char* element);
void remove_element_darray_string(DynamicArrayString* array, int index);

void print_darray_string(DynamicArrayString* array);
void clear_darray_string(DynamicArrayString* array);

char* get_element_darray_string(DynamicArrayString* array, int index);
void set_element_darray_string(DynamicArrayString* array, int index, char* element);
