#ifndef QUEUE_H
#define QUEUE_H

#include <sys/types.h>

typedef struct msg {
    char *data;
    struct msg *next;
} msg;

typedef struct queue {
    struct msg *first;
    struct msg *last;
} queue;

/*
 * Allocate memory for an initialize a Message Queue
 * Returns a pointer to struct queue on success, NULL on error
 */

struct queue* queue_init();

/*
 * Free memory for a Message Queue, including all linked Messages
 * Returns void
 */

void queue_free(struct queue *q);

/*
 * Add a Message to the beginning of a queue
 * Returns void
 */

void queue_prepend(struct queue *q, char *str);

/*
 * Add a Message to the end of a queue
 * Returns void
 */

void queue_append(struct queue *q, char *str);

/*
 * Pop a Message off the queue into buff
 * Returns 0 on success, -1 on error
 */

int queue_pop(struct queue *q, char *buff, size_t buffsize);

#endif
