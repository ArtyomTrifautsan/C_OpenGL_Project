#include "queue.h"

#include <stdio.h>
#include <stdlib.h>


Queue create_queue(int capacity)
{
    Queue queue;

    if (capacity < 1)
    {
        fprintf(stderr, "[Queue: create_queue]: can't create queue with capacity < 1.\n");
        exit(EXIT_FAILURE);
    }

    queue.queue = (int*)malloc(capacity * sizeof(int));
    queue.low = 0;
    queue.high = -1;
    queue.size = 0;
    queue.capacity = capacity;

    return queue;
}

void queue_add_element(Queue* queue, int element)
{
    if (queue->size == queue->capacity)
    {
        printf("[Queue: queue_add_element]: can't add element: Queue is full.\n");
    }

    queue->high = (queue->high + 1) % queue->capacity;
    queue->queue[queue->high] = element;
    queue->size++;
}

int queue_remove_element(Queue* queue)
{
    if (queue->size == 0) {
        printf("[Queue: queue_remove_element]: can't remove element: Queue is empty.\n");
        return -1;
    }

    int element = queue->queue[queue->low];
    queue->low = (queue->low + 1) % queue->capacity;
    queue->size--;
    return element;
}

void print_queue(Queue* queue)
{
    printf("<");
    
    if (queue->size > 0)
    {
        // printf("%.3f", get_element(array, 0));
        int index = (queue->low) % queue->capacity;
        printf("%d", queue->queue[index]);
    }

    if (queue->size > 1)
    {
        for (int i = 1; i < queue->size; i++)
        {
            int index = (queue->low + i) % queue->capacity;
            printf(", %d", queue->queue[index]);
        }
    }

    printf(">\n");
}

void free_queue(Queue* queue)
{
    free(queue->queue);
    queue->queue = NULL;
    queue->low = -1;
    queue->high = -1;
    queue->size = -1;
    queue->capacity = -1;
}
