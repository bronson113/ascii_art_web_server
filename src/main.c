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
    Bitmap* image = download_image_by_id("K6r1fSl");
    Asciimap ascii = Convert_to_ascii(*image);
    for(int j=0;j<ascii.Height;j++){
        for(int i=0;i<ascii.Width;i++){
            printf("%c", ascii.Pixels[j][i]);
        }
        printf("\n");
    }
}
