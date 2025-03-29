#include <stdlib.h>
#include <string.h>

#include <heap.hpp>

int percolate_down(heap *h, int parent) {
  int child = 2 * parent + 1;
  char *base =
      (char *)
          h->a; // need to cast a to char * bc void * pointer arith is undefined

  void *tmp = malloc(h->element_size);
  if (!tmp) {
    return -1;
  }

  while (child < h->size) {
    if (child + 1 < h->size &&
        h->cmp(base + (child * h->element_size),
               base + ((child + 1) * h->element_size)) > 0) {
      child++;
    }

    if (h->cmp(base + (parent * h->element_size),
               base + (child * h->element_size)) <= 0) {
      break;
    }

    memcpy(tmp, base + (parent * h->element_size), h->element_size);
    memcpy(base + (parent * h->element_size), base + (child * h->element_size),
           h->element_size);
    memcpy(base + (child * h->element_size), tmp, h->element_size);

    parent = child;
    child = 2 * parent + 1;
  }

  free(tmp);
  return 0;
}

int percolate_up(heap *h, int child) {
  int parent = (child - 1) / 2;
  char *base = (char *)h->a;
  void *tmp = malloc(h->element_size);
  if (!tmp) {
    free(tmp);
    return -1;
  };

  while (child > 0 && h->cmp(base + (parent * h->element_size),
                             base + (child * h->element_size)) > 0) {
    // swap
    memcpy(tmp, base + (parent * h->element_size), h->element_size);
    memcpy(base + parent * h->element_size, base + child * h->element_size,
           h->element_size);
    memcpy(base + child * h->element_size, tmp, h->element_size);

    child = parent;
    parent = (child - 1) / 2;
  }

  free(tmp);
  return 0;
}

int heap_init(heap *h, size_t element_size,
              int (*cmp)(const void *, const void *)) {
  h->a = malloc(element_size * DEFAULT_HEAP_CAPACITY);
  if (!h->a) {
    return -1;
  }
  h->size = 0;
  h->capacity = DEFAULT_HEAP_CAPACITY;
  h->element_size = element_size;
  h->cmp = cmp;

  return 0;
}

int heap_destroy(heap *h) {
  free(h->a);
  h->a = NULL;

  return 0;
}

int heap_push(heap *h, void *data) {
  if (h->size == h->capacity) {
    h->capacity *= 2;
    void *new_alloc = realloc(h->a, h->element_size * h->capacity);
    if (!new_alloc)
      return -1;
    h->a = new_alloc;
  }

  char *base = (char *)h->a;
  memcpy(base + (h->size * h->element_size), data, h->element_size);
  if (percolate_up(h, h->size))
    return -1;

  h->size++;
  return 0;
}

int heap_pop(heap *h, void *data) {
  char *base = (char *)h->a;

  if (h->size <= 0) {
    return -1;
  }

  memcpy(data, base, h->element_size);

  h->size--;
  memcpy(base, base + ((h->size) * h->element_size), h->element_size);

  if (percolate_down(h, 0))
    return -1;

  return 0;
}

int heap_peek(heap *h, void *data) {
  if (h->size <= 0) {
    return -1;
  }

  memcpy(data, h->a, h->element_size);

  return 0;
}

int heap_is_empty(heap *h) { return !h->size; }
