#ifndef HEAP_H
#define HEAP_H

#include "gen.h"

#define DEFAULT_HEAP_CAPACITY 100

typedef struct heap {
    point *points;
    int *weights;
    int size;
    int capacity;
} heap;

int heap_init(heap *h);
int heap_destroy(heap *h);
int heap_push(heap *h, point p, int weight);
int heap_pop(heap *h, point *p, int *weight);
int heap_peek(heap *h, point *p, int *weight);
int heap_is_empty(heap *h);

#endif