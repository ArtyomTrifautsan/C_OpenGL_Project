#pragma once


typedef struct 
{
    int low;
    int high;
    int size;
    int capacity;
    int* queue;
} Queue;


Queue create_queue(int capacity);
void queue_add_element(Queue* queue, int element);
int queue_remove_element(Queue* queue);
void print_queue(Queue* queue);
void free_queue(Queue* queue);
