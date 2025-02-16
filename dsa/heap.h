#ifndef HEAP_H
#define HEAP_H

#include "../types/dungeon.h"

#define DEFAULT_HEAP_CAPACITY 10

typedef struct heap {
    int *array;
    int size;
    int capacity;
} heap;

int heap_init(heap *h);
int heap_destroy(heap *h);
int heap_push(heap *h, int data);
int heap_pop(heap *h, int *data);
int heap_peek(heap *h, int *data);
int heap_is_empty(heap *h);

#endif
