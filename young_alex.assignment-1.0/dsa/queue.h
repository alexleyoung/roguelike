#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

typedef struct queue {
    queue_node *front, *back;
    int length;
} queue;

int queue_init(queue *q);
int queue_destroy(queue *q);
int queue_enqueue(queue *q, void *data);
int queue_dequeue(queue *q, void **data);
int queue_front(queue *q, void **data);
int queue_length(queue *q);
int queue_is_empty(queue *q);

#endif