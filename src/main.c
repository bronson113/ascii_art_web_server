#include "main.h"

void signal_server(){
//	cleanup_server();
	exit(1);
}

void cleanup_server(){

    // kill add child and wait for all child processes to terminate
    for(int i=0;i<WORKERS;i++){
        kill(child_pids[i], SIGTERM);
    }
    while(wait(NULL) > 0);
    printf("All child processes terminated\n");
    
    printf("Process %d detached the segment %d\n", getpid(), queue_shared_memory_id);
    // detach from the shared memory segment
    if (shmdt(request_queue) == -1)
    {
        perror("Process shmdt returned -1\n");
        error(-1, errno, " ");
    }

    // remove the shared memory segment
    if (shmctl(queue_shared_memory_id, IPC_RMID, NULL) == -1)
    {
        perror("Process shmctl returned -1\n");
        error(-1, errno, " ");
    }
    else
        printf("Process %d removed the segment %d\n", getpid(), queue_shared_memory_id);
    
    return;
}

void signal_worker(){
//	cleanup_worker();
	exit(1);
}

void cleanup_worker(){
    // detach from the shared memory segment
    printf("Process %d detached the segment %d\n", getpid(), queue_shared_memory_id);
    if (shmdt(request_queue) == -1)
    {
        perror("Process shmdt returned -1\n");
        error(-1, errno, " ");
    }

    printf("Cleaning up worker\n");
    return;
}

int main(int argc, char *argv[]){
    int port = 1234;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
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

    // get the pid of the parent process
    pid_t parent_pid = getpid();


    // spawn worker processes
    for(int i=0;i<WORKERS;i++){
        int pid = fork();
        if(pid < 0){
            perror("fork failed");
            return 1;
        }

        if(pid == 0){
            // child process

            // register cleanup function for server
            signal(SIGINT, signal_worker);
            signal(SIGTERM, signal_worker);
            int res = atexit(cleanup_worker);
            if (res != 0) {
                perror("atexit failed");
                return 1;
            }

            // attach to the shared memory segment
            request_queue = (struct queue *)shmat(queue_shared_memory_id, NULL, 0);
            if (request_queue == (struct queue *)-1)
            {
                perror("Process shmat returned NULL\n");
                error(-1, errno, " ");
            }
            else
                printf("Process %d attached the segment %d\n", getpid(), queue_shared_memory_id);

			// close std in, so all the control command goes to the server
			close(0);

            worker(parent_pid, request_queue);
            return 0;
        } else {
            // parent process
            child_pids[i] = pid;
        }
    }

    // register cleanup function for server
    signal(SIGINT, signal_server);
    signal(SIGTERM, signal_server);
    int res = atexit(cleanup_server);
    if (res != 0) {
        perror("atexit failed");
        return 1;
    }
    // parent (server) process
    request_queue = (struct queue *)shmat(queue_shared_memory_id, NULL, 0);
    if (request_queue == (struct queue *)-1)
    {
        perror("Process shmat returned NULL\n");
        error(-1, errno, " ");
    }
    else
    {
        printf("Process %d attached the segment %d\n", getpid(), queue_shared_memory_id);
        // initialize the queue
        initialize_queue(request_queue);
    }
    return server(request_queue, port);
}
