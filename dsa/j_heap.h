#ifndef J_HEAP_H
# define J_HEAP_H

# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>

struct heap_node;
typedef struct heap_node heap_node_t;

typedef struct j_heap {
  heap_node_t *min;
  uint32_t size;
  int32_t (*compare)(const void *key, const void *with);
  void (*datum_delete)(void *);
} j_heap_t;

void j_heap_init(j_heap_t *h,
               int32_t (*compare)(const void *key, const void *with),
               void (*datum_delete)(void *));
void heap_delete(j_heap_t *h);
heap_node_t *heap_insert(j_heap_t *h, void *v);
void *heap_peek_min(j_heap_t *h);
void *heap_remove_min(j_heap_t *h);
int heap_combine(j_heap_t *h, j_heap_t *h1, j_heap_t *h2);
int heap_decrease_key(j_heap_t *h, heap_node_t *n, void *v);
int heap_decrease_key_no_replace(j_heap_t *h, heap_node_t *n);

# ifdef __cplusplus
}
# endif

#endif
