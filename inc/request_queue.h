#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>

#define QUEUE_SIZE 100

#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H 1
struct request{
    int client_fd;
    _Atomic int handled;
};

struct queue{
    struct request requests[QUEUE_SIZE];
    _Atomic int head;
    _Atomic int tail;
};
#endif

struct request *get_request(struct queue *queue);
void add_request(struct queue *queue, int client_fd);