#ifndef CORRIDOR_HEAP_HPP
#define CORRIDOR_HEAP_HPP

#include "dungeon.hpp"

#define DEFAULT_HEAP_CAPACITY 10

typedef struct corridor_heap {
  point *points;
  int *weights;
  int size;
  int capacity;
} corridor_heap;

int corridor_heap_init(corridor_heap *h);
int corridor_heap_destroy(corridor_heap *h);
int corridor_heap_push(corridor_heap *h, point p, int weight);
int corridor_heap_pop(corridor_heap *h, point *p, int *weight);
int corridor_heap_peek(corridor_heap *h, point *p, int *weight);
int corridor_heap_is_empty(corridor_heap *h);

#endif
