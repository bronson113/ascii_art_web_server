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
int recv_until_str(int fd, char* buffer, int len, char* end_str, int pat_len);
int recv_until(int fd, char* buffer, int len, char end);
void serve_static_file(int client_fd, char* file_name);
void handle_request(struct request* client_request, int client_fd, pid_t process_id);
void worker(pid_t parent_pid, struct queue *request_queue);
int char_search(char url[], char search)
  
#endif
#define WORKER_HEADER 1

