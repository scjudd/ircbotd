#include <stdio.h>
#include "queue.h"

#define MAX_MSG_LEN 1024

int main(int argc, char *argv[])
{
    struct queue *q = queue_init();
    char buff[MAX_MSG_LEN];

    queue_append(q, "bar");
    queue_append(q, "baz");
    queue_prepend(q, "foo");
    
    if (queue_pop(q, buff, MAX_MSG_LEN) == 0)
        puts(buff);
    
    if (queue_pop(q, buff, MAX_MSG_LEN) == 0)
        puts(buff);
    
    if (queue_pop(q, buff, MAX_MSG_LEN) == 0)
        puts(buff);
    
    if (queue_pop(q, buff, MAX_MSG_LEN) == 0)
        puts(buff);

    queue_free(q);

    return 0;

}
