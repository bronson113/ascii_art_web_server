#include "worker.h"
void handle_request(struct request* client_request, pid_t process_id){
    printf("[%05d] handling request: fd %d\n", process_id, client_request->client_fd);
    return;
}

void worker(pid_t parent_pid, struct queue *request_queue){

    // open parent process as pid file descriptor
    printf("pidfd_open: pid(%d)\n", parent_pid);
    pid_t process_id = getpid();
    int pidfd = syscall(SYS_pidfd_open, parent_pid, 0);
    if(pidfd == -1){
        perror("pidfd_open failed");
        return;
    }
    
    while(1){
        struct request* client_request;
        // wait for a client to be pushed onto the queue
        client_request = get_request(request_queue);
        if(client_request == NULL){
            continue;
        }

        // syscall 438 for pidfd_getfd
        printf("pidfd_getfd: pfd(%d)->fd(%d)\n", pidfd, client_request->client_fd);
        int client_fd = syscall(438, pidfd, client_request->client_fd, 0);
        if(client_fd == -1){
            perror("pidfd_getfd failed");
            continue;
        }
        client_request->client_fd = client_fd;

        handle_request(client_request, process_id);
        
        if (shutdown(client_request->client_fd, SHUT_RDWR) == -1) {
            perror("shutdown failed");
        }
        close(client_request->client_fd);
    }
    printf("worker process started\n");
    return;
}
