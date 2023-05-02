#include "worker.h"
int recv_until(int fd, char* buffer, int len, char end){
    char temp[4];
    int idx=0;
    while(read(fd, temp, 1)==1){
        if(temp[0] != end){
            buffer[idx++] = temp[0];
        }
        else{
            buffer[idx] = 0;
            return idx;
        }
		if(idx >= len-1){
			buffer[len-1] = 0;
			return len-1;
		}
    }
    return idx;
}

int recv_until_str(int fd, char* buffer, int len, char* end_str, int pat_len){
	char temp[4];
	int idx = 0;
	while(read(fd, temp, 1) == 1){
		buffer[idx++] = temp[0];
		if(idx >= len-1){
			buffer[len-1] = 0;
			return len-1;
		}
		if(idx < pat_len){
			continue;
		}
		if(!strncmp(buffer+idx-pat_len, end_str, pat_len)){
            buffer[idx--] = 0;
			return idx;
		}
	}
	return idx;
}

void handle_request(struct request* client_request, int client_fd, pid_t process_id){
    char buf[256];
	char headers[2048], body[2048];
	char *file_content;
    int len;
	FILE *fp;
	int file_len;
    printf("[%05d] handling request: fd %d\n", process_id, client_request->client_fd);
    // get request method
    len = recv_until(client_fd, buf, 256, ' ');
    printf("request method: %s (len=%d)", buf, len);
    if(!strncmp(buf, "GET", 3)){
        // get request url
        len = recv_until(client_fd, buf, 256, ' ');
        if(len == 1 && buf[0] == '/'){
            fp = fopen("site/frontPage.html", "r");
            fseek(fp, 0, SEEK_END);
            file_len = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            file_content = malloc(file_len);
            fread(file_content, file_len, 1, fp);
            sprintf(headers, "HTTP/1.0 200 OK\nServer: Ascii_art_server/0.0\nContent-type: text/html; charset=utf-8\nContent-Length: %d\n\n", file_len);
            write(client_fd, headers, strlen(headers));
            write(client_fd, file_content, file_len);
        }
    }
    else if (!strncmp(buf, "POST", 4)){
        len = recv_until(client_fd, buf, 256, '\n');
		printf("location: %s\n", buf);
		// need to fix \r\n and \n competibility
		recv_until(client_fd, headers, 2048, '\n');
		while(strncmp(headers, "Content-Length:", 15)){
			printf("[header] %s", headers);
			recv_until(client_fd, headers, 2048, '\n');
		}
		// get body length
		sscanf(headers, "Content-Length: %d", &len);

		// get the rest of the header
		recv_until_str(client_fd, headers, 2048, "\r\n\r\n", 4);
		read(client_fd, body, len);
		
		printf("other header: %s\nbody length: %d\nbody: %s\n", headers, len, body);
		fp = fopen("site/frontPage.html", "r");
		fseek(fp, 0, SEEK_END);
		file_len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		file_content = malloc(file_len);
		fread(file_content, file_len, 1, fp);
		sprintf(headers, "HTTP/1.0 200 OK\nServer: Ascii_art_server/0.0\nContent-type: text/html; charset=utf-8\nContent-Length: %d\n\n", file_len);
		write(client_fd, headers, strlen(headers));
		write(client_fd, file_content, file_len);
	}
    
    client_request->done = 1;
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

        handle_request(client_request, client_fd, process_id);
        
        if (shutdown(client_fd, SHUT_RDWR) == -1) {
            perror("shutdown failed");
        }
        close(client_fd);
    }
    printf("worker process started\n");
    return;
}
