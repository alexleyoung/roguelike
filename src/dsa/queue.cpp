#include <stdlib.h>

#include <queue.hpp>

int queue_init(queue *q) {
  q->front = q->back = NULL;
  q->length = 0;

  return 0;
}

int queue_destroy(queue *q) {
  /* not performant bc of procedure calls
  int value;

  while (!queue_dequeue(q, &value));

  return 0;
  */
  queue_node *tmp;
  while ((tmp = q->front)) {
    q->front = q->front->next;
    free(tmp);
  }

  return 0;
}

int queue_enqueue(queue *q, void *data) {
  queue_node *node;
  if (!(node = (queue_node *)(malloc(sizeof(*node))))) {
    return -1;
  };
  node->data = data;
  node->next = NULL;

  if (!q->front) {
    q->front = q->back = node;
  } else {
    q->back->next = node;
    q->back = node;
  }

  q->length++;
  return 0;
}

int queue_dequeue(queue *q, void **data) {
  if (!q->front) {
    return -1;
  }

  *data = q->front->data;
  queue_node *tmp = q->front;
  q->front = q->front->next;
  free(tmp);

  // if remove only element
  if (!q->front) {
    q->back = NULL;
  }

  q->length--;
  return 0;
};

int queue_front(queue *q, void **data) {
  if (!q->front) {
    return -1;
  }

  *data = q->front->data;

  return 0;
};

int queue_length(queue *q) { return q->length; };

int queue_is_empty(queue *q) { return (!q->length); };
