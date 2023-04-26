#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "server.h"
#include "worker.h"
#include "request_queue.h"

#define WORKERS 4

int queue_shared_memory_id;
struct queue *request_queue;
int child_pids[WORKERS];
void cleanup_server();
void signal_server();
void cleanup_worker();
void signal_worker();
int main(int argc, char *argv[]);
