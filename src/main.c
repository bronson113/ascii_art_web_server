#include "main.h"

int main(){
    // setup shared memory
    // setup_process_queue();

    // spawn worker processes
    int child_pids[8] = {0};
    for(int i=0;i<8;i++){
        int pid = fork();
        if(pid == 0){
            // child process
            worker();
            return 0;
        }
        else{
            // parent process
            child_pids[i] = pid;
        }
    }
    // parent (server) process
    server();
    return 0;
}