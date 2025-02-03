#include <stdlib.h>

#include "corridor_heap.h"

/*
Custom min-heap implementation which holds points and weights.
Needed for dijkstra's in generate_corridors.
*/

/*
helper method to percolate down
*/
void percolate_down(heap *h, int parent) {
    int child = 2 * parent + 1;
    while (child < h->size) {
            if (child + 1 < h->size && h->weights[child] > h->weights[child+1]) {
            child++;
        }
        if (h->weights[parent] > h->weights[child]) {
            // swap
            point tmp_p = h->points[parent];
            int tmp_w = h->weights[parent];
            h->points[parent] = h->points[child];
            h->weights[parent] = h->weights[child];
            h->points[child] = tmp_p;
            h->weights[child] = tmp_w;
        } else {
            break;
        }
        parent = child;
        child = 2 * parent + 1;
    }
}

/*
init empty heap
*/
int heap_init(heap *h) {
    h->points = malloc(sizeof (*h->points) * DEFAULT_HEAP_CAPACITY);
    h->weights = malloc(sizeof (*h->weights) * DEFAULT_HEAP_CAPACITY);
    h->size = 0;
    h->capacity = DEFAULT_HEAP_CAPACITY;

    return 0;
}

/*
destroy and free memory used by heap
*/
int heap_destroy(heap *h) {
    free(h->points);
    free(h->weights);

    return 0;
}

/*
push point and weight to heap
*/
int heap_push(heap *h, point p, int weight) {
    // dynamically expand adhoc
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->points = realloc(h->points, sizeof (*h->points) * h->capacity);
        h->weights = realloc(h->weights, sizeof (*h->weights) * h->capacity);
    }

    h->points[h->size] = p;
    h->weights[h->size] = weight;
    h->size++;

    percolate_down(h, h->size);

    return 0;
}

/*
pop point and weight from heap
*/
int heap_pop(heap *h, point *p, int *weight) {
    if (h->size == 0) {
        return -1; // heap is empty
    }

    // get min values
    *p = h->points[0];
    *weight = h->weights[0];

    // replace min with last element and percolate down
    h->points[0] = h->points[h->size - 1];
    h->weights[0] = h->weights[h->size - 1];
    h->size--;
    percolate_down(h, 0);

    return 0;
}

/*
get min value and weight from heap
*/
int heap_peek(heap *h, point *p, int *weight) {
    if (h->size == 0) {
        return -1;
    }

    *p = h->points[0];
    *weight = h->weights[0];

    return 0;
}

/*
returns 1 if empty, 0 if not
*/
int heap_is_empty(heap *h) {
    return (!h->size);
}