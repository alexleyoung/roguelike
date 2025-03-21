#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

#define DEFAULT_HEAP_CAPACITY 10

typedef struct heap {
    void *a;
    int size;
    int capacity;
    size_t element_size;
    int (*cmp)(const void *, const void *);
} heap;

int heap_init(heap *h, size_t element_size, int (*cmp)(const void *, const void *));
int heap_destroy(heap *h);
int heap_push(heap *h, void *data);
int heap_pop(heap *h, void *data);
int heap_peek(heap *h, void *data);
int heap_is_empty(heap *h);

#endif
