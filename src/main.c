#include "main.h"

int main(){
    // setup shared memorya for the request queue
    // read and write permissions for all users
    if ((queue_shared_memory_id = shmget(IPC_PRIVATE, sizeof(struct queue), IPC_CREAT | 0666)) < 0)
    {
        perror("smget returned -1\n");
        error(-1, errno, " ");
        exit(-1);
    }
    // debug
    printf("Allocated sharded request queue %d, at id %d\n", (int) sizeof(struct queue), queue_shared_memory_id);

    // spawn worker processes
    int child_pids[WORKERS] = {0};
    for(int i=0;i<WORKERS;i++){
        int pid = fork();
        if(pid < 0){
            perror("fork failed");
            return 1;
        }

        if(pid == 0){
            // child process
            // attach to the shared memory segment
            request_queue = (struct queue *)shmat(queue_shared_memory_id, NULL, 0);
            if (request_queue == (struct queue *)-1)
            {
                perror("Process shmat returned NULL\n");
                error(-1, errno, " ");
            }
            else
                printf("Process %d attached the segment %d\n", getpid(), queue_shared_memory_id);

            worker(request_queue);
            return 0;
        } else {
            // parent process
            child_pids[i] = pid;
        }
    }
    // parent (server) process
    request_queue = (struct queue *)shmat(queue_shared_memory_id, NULL, 0);
    if (request_queue == (struct queue *)-1)
    {
        perror("Process shmat returned NULL\n");
        error(-1, errno, " ");
    }
    else
        printf("Process %d attached the segment %d\n", getpid(), queue_shared_memory_id);

    return server(request_queue);
}