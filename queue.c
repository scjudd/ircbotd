#include "queue.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

struct queue* queue_init()
{
    struct queue *q;
    if ((q = malloc(sizeof(*q))) == NULL)
    {
        queue_free(q);
        return NULL;
    }

    q->first = NULL;
    q->last = NULL;

    return q;
}

void queue_free(struct queue *q)
{
    struct msg *next;

    if (q != NULL)
    {
        while (q->first != NULL)
        {
            next = q->first->next;
            free(q->first->data);
            free(q->first);
            q->first = next;
        }
        free(q);
    }
}

static void msg_free(struct msg *msg)
{
    if (msg != NULL)
    {
        if (msg->data != NULL)
            free(msg->data);
        free(msg);
    }
}

static struct msg* msg_init(char *str)
{
    struct msg *msg;

    if ((msg = malloc(sizeof(*msg))) == NULL ||
        (msg->data = malloc(sizeof(char) * (strlen(str) + 1))) == NULL)
    {
        msg_free(msg);
        return NULL;
    }

    strcpy(msg->data, str);
    msg->next = NULL;

    return msg;
}

void queue_prepend(struct queue *q, char *str)
{
    struct msg *msg = msg_init(str);

    if (q->last == NULL)        // If the queue is empty
        q->last = msg;
    else
        msg->next = q->first;

    q->first = msg;
}

void queue_append(struct queue *q, char *str)
{
    struct msg *msg = msg_init(str);

    if (q->first == NULL)       // If the queue is empty
        q->first = msg;
    else
        q->last->next = msg;

    q->last = msg;
}

int queue_pop(struct queue *q, char *buff, size_t buffsize)
{
    struct msg *old_first = q->first;

    if (q->first == NULL)
        return -1;

    strncpy(buff, q->first->data, buffsize);
    q->first = q->first->next;

    msg_free(old_first);

    return 0;
}
