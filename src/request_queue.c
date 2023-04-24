#include "request_queue.h"

void initialize_queue(struct queue* request_queue){
    // initialize the queue
    request_queue->head = 0;
    request_queue->tail = 0;
    for(int i=0;i<QUEUE_SIZE;i++){
        request_queue->requests[i].client_fd = -1;
        request_queue->requests[i].handled = 1;
    }
    return;
}

void add_request(struct queue* request_queue, int client_fd){
    // create a new request
    request_queue->requests[request_queue->tail].client_fd = client_fd;
    request_queue->requests[request_queue->tail].handled = 0;
    request_queue->tail = (request_queue->tail + 1) % QUEUE_SIZE;
    return;
}

struct request* get_request(struct queue* request_queue){
    // retrieve the current head of the queue
    // helps avoid race condition
    int cur_head = request_queue->head;
    // check if the queue is empty
    if(cur_head == request_queue->tail){
        return NULL;
    }
    // mark the request as handled (kind of as a lock)
    _Atomic int handled = atomic_exchange(&request_queue->requests[cur_head].handled, 1);
    // if the atomic exchange get 1, the first request was already handled
    if(handled == 1){
        return NULL;
    }

    // debug
    // printf("handled: %d", handled);

    // get the request at the head of the queue
    struct request* client_request = &request_queue->requests[cur_head];
    // move the head of the queue
    request_queue->head = (request_queue->head + 1) % QUEUE_SIZE;
    return client_request;
}