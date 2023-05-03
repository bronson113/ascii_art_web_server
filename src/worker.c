#include "worker.h"

int char_search(char url[], char search)
{
	char *result;
	result = strchr(url, search);
	if (result == NULL)
	       //if search character is not in url return false	
		return 0;
	else
	       //if search character is in url return true	
		return 1;
}

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

void serve_static_file(int client_fd, char* file_name){
    char *file_content;
    FILE *fp;
    int file_len;
    char headers[2048];
    fp = fopen(file_name, "r");
    fseek(fp, 0, SEEK_END);
    file_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    file_content = malloc(file_len);
    fread(file_content, file_len, 1, fp);
    sprintf(headers, "HTTP/1.0 200 OK\nServer: Ascii_art_server/0.0\nContent-type: text/html; charset=utf-8\nContent-Length: %d\n\n", file_len);
    write(client_fd, headers, strlen(headers));
    write(client_fd, file_content, file_len);
    free(file_content);
    fclose(fp);
}

void handle_request(struct request* client_request, int client_fd, pid_t process_id){
    char buf[256];
	char headers[2048], body[2048];
	char *file_content;
    int len;
	FILE *fp;
	int file_len;
    static int request_id = 0;
//    printf("[%05d] handling request: fd %d\n", process_id, client_request->client_fd);
    // get request method
    len = recv_until(client_fd, buf, 256, ' ');
    printf("request method: %s (len=%d)\n", buf, len);
    if(!strncmp(buf, "GET", 3)){
        // get request url
        len = recv_until(client_fd, buf, 256, ' ');
        printf("Get %s\n", buf);
        if(len == 1 && buf[0] == '/'){
            serve_static_file(client_fd, "site/frontPage.html");
        }
        if(!strncmp(buf, "/ASCII.html", 11)){
            if(!strncmp(buf+11, "?id=", 4)){
                int target_id = atoi(buf+15);
                printf("target_id: %d\n", target_id);
                char cmd[256];
                sprintf(cmd, "cp /tmp/ascii_%d.html site/snippet.html", target_id);
                system(cmd);
                serve_static_file(client_fd, "site/ASCII.html");
            }
            else{
                serve_static_file(client_fd, "site/ASCII.html");
            }
        }
        if(!strncmp(buf, "/snippet.html", 13)){
            serve_static_file(client_fd, "site/snippet.html");
        }
        if(!strncmp(buf, "/favicon.ico", 12)){
            fp = fopen("site/favicon.ico", "r");
            fseek(fp, 0, SEEK_END);
            file_len = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            file_content = malloc(file_len);
            fread(file_content, file_len, 1, fp);
            sprintf(headers, "HTTP/1.0 200 OK\nServer: Ascii_art_server/0.0\nContent-type: image/x-icon; charset=utf-8\nContent-Length: %d\n\n", file_len);
            write(client_fd, headers, strlen(headers));
            write(client_fd, file_content, file_len);
            free(file_content);
            fclose(fp);
        }
    }
    else if (!strncmp(buf, "POST", 4)){
        len = recv_until(client_fd, buf, 256, '\n');
		printf("location: %s\n", buf);
        if(!strncmp(buf, "/ASCII.html", 11)){
            // need to fix \r\n and \n competibility
            recv_until(client_fd, headers, 2048, '\n');
            while(strncmp(headers, "Content-Length:", 15)){
                printf("[header] %s\n", headers);
                recv_until(client_fd, headers, 2048, '\n');
            }
            // get body length
            sscanf(headers, "Content-Length: %d", &len);

            // get the rest of the header
            recv_until_str(client_fd, headers, 2048, "\r\n\r\n", 4);
            memset(body, 0, 2048);
            read(client_fd, body, len);
            body[len] = 0;
            printf("len: %d, body: %s\n", len, body);
            char *decoded_string = urlDecode(body);
            const char url_ch = '=';
            char *url_string = strchr(decoded_string, url_ch);
            url_string++; // remove the token char
            printf("url_string: %s\n", url_string);
            int value = char_search(url_string,'/');
            Bitmap *image;
            if (value == 1)
            {
               image = download_image(url_string);
            }
            else
            {
               image = download_image_by_id(url_string);
            }
            free(decoded_string);
            printf("image height: %d, width: %d\n", image->Height, image->Width);
            Asciimap ascii = Convert_to_ascii(*image);
            printf("ascii height: %d, width: %d\n", ascii.Height, ascii.Width);
            char file_name[100];
            sprintf(file_name, "/tmp/ascii_%d.html", request_id+getpid()*1000);

            FILE *fp = fopen(file_name, "w");
            for(int j=0;j<ascii.Height;j++){
                for(int i=0;i<ascii.Width;i++){
                    fprintf(fp, "%c", ascii.Pixels[j][i]);
                }
                fprintf(fp, "\n");
            }
            fclose(fp);
            //printf("other header: %s\nbody length: %d\nbody: %s\n", headers, len, body);
            sprintf(headers, "HTTP/1.1 302 Found\nLocation: ASCII.html?id=%d", request_id+getpid()*1000);
            write(client_fd, headers, strlen(headers));
            request_id++;
        }
	}
    
    client_request->done = 1;
    return;
}

void worker(pid_t parent_pid, struct queue *request_queue){

    // open parent process as pid file descriptor
    //printf("pidfd_open: pid(%d)\n", parent_pid);
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
        //printf("pidfd_getfd: pfd(%d)->fd(%d)\n", pidfd, client_request->client_fd);
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
