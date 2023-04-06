#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "worker.h"

struct request{
    int client_fd;
    int handled;
    char* message;
};

struct node{
    struct node* next;
    struct request* request;
};

struct queue{
    struct node* requests;
    struct node* tail;
    int size;
};

struct queue* request_queue;