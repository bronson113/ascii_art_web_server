#include "worker.h"
void handle_request(struct request* client_request){
    printf("handling request: fd %d\n", client_request->client_fd);
    return;
}

void worker(struct queue *request_queue){
    
    while(1){
        struct request* client_request;
        // wait for a client to be pushed onto the queue
        client_request = get_request(request_queue);
        if(client_request == NULL){
            continue;
        }

        handle_request(client_request);
        
        if (shutdown(client_request->client_fd, SHUT_RDWR) == -1) {
            perror("shutdown failed");
        }
        close(client_request->client_fd);
    }
    printf("worker process started\n");
    return;
}