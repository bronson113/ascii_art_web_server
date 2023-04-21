#include <stdio.h> 
#include <sys/shm.h>
#include <sys/wait.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "request_queue.h"

#ifndef WORKER_HEADER
void handle_request(struct request* client_request, pid_t process_id);
void worker(pid_t parent_pid, struct queue *request_queue);

#endif
#define WORKER_HEADER 1

