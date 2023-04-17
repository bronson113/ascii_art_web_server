#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "worker.h"
#include "request_queue.h"

#define WORKERS 4

int queue_shared_memory_id;
struct queue *request_queue;
int main();