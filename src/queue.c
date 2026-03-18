#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Queue *queue_new(uint32_t capacity, uint32_t elem_type) {
    if (capacity > 1) {
        Queue *new_queue = malloc(sizeof(Queue));
        if (NULL == new_queue) {
            perror("malloc new_queue failure");
            return NULL;
        }

        new_queue->capacity = capacity;
        new_queue->elem_type = elem_type;
        new_queue->front = 0;
        new_queue->end = 0;
        new_queue->size = 0;

        new_queue->elems = malloc(elem_type * capacity);
        if (!new_queue->elems) {
            perror("malloc new_queue->elems failure");
            free(new_queue);
            return NULL;
        }

        return new_queue;
    } else {
        printf("队列容量需要大于1，因为是循环队列");
    }

    return NULL;
}

bool is_empty(Queue *queue) {
    return queue->front == queue->end ? true : false;
}

bool is_full(Queue *queue) {
    return (queue->end + 1) % queue->capacity == queue->front ? true : false;
}

void push(Queue *queue, void *elem) {
    if (is_full(queue)) {
        printf("队列满，push 失败\n");
        return;
    }

    memcpy(queue->elems + queue->end * queue->elem_type, elem, queue->elem_type);
    queue->end = (queue->end + 1) % queue->capacity;
    queue->size++;
}

void pop(Queue *queue) {
    if (is_empty(queue)) {
        printf("队列为空，pop 失败\n");
        return;
    }

    queue->front = (queue->front + 1) % queue->capacity;

    queue->size--;
}

void queue_free(Queue *queue) {
    if (queue) {
        if (queue->elems) {
            free(queue->elems);
        }
        free(queue);
    }
}
