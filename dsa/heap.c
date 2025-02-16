#include <stdlib.h>

#include "heap.h"

void percolate_down(heap *h, int parent) {
    int child = 2 * parent + 1;
    while (child < h->size) {
        if (child + 1 < h->size && h->array[child] > h->array[child+1]) {
            child++;
        }
        if (h->array[parent] > h->array[child]) {
            // swap
            int tmp = h->array[parent];
            h->array[parent] = h->array[child];
            h->array[child] = tmp;
        } else {
            break;
        }
        parent = child;
        child = 2 * parent + 1;
    }
}

void percolate_up(heap *h, int child) {
    int parent = (child - 1) / 2;
    while (parent >= 0 && h->array[parent] > h->array[child]) {
        // swap
        int tmp = h->array[parent];
        h->array[parent] = h->array[child];
        h->array[child] = tmp;
        child = parent;
        parent = (child - 1) / 2;
    }
}

int heap_init(heap *h) {
    h->array = malloc(sizeof (*h->array) * DEFAULT_HEAP_CAPACITY);
    h->size = 0;
    h->capacity = DEFAULT_HEAP_CAPACITY;

    return 0;
}

int heap_destroy(heap *h) {
    free(h->array);

    return 0;
}

int heap_push(heap *h, int data) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->array = realloc(h->array, sizeof (*h->array) * h->capacity);
    }

    h->array[h->size] = data;
    percolate_up(h, h->size);

    h->size++;
    return 0;
}

int heap_pop(heap *h, int *data) {
    if (h->size <= 0) {
        return -1;
    }

    *data = h->array[0];

    h->array[0] = h->array[h->size - 1];
    percolate_down(h, 0);

    h->size--;
    return 0;
}

int heap_peek(heap *h, int *data) {
    if (h->size <= 0) {
        return -1;
    }

    *data = h->array[0];

    return 0;
}

int heap_is_empty(heap *h) {
    return h->size == 0;
}
