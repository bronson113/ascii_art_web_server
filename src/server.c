#include "server.h"

int server(struct queue* request_queue, int port){
    // https://progbook.org/httpserv.html
    
    // socket information descriptor 
    struct sockaddr_in sa;
    
    // socket info initialization
	memset(&sa, 0, sizeof sa);
    // constant for ipv4
	sa.sin_family = AF_INET;
    // listen to port 1234
	sa.sin_port = htons(port);
    // iisten to local addresses
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

    // create socket
	int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // catch error
	if (server_fd == -1) {
		perror("cannot create socket");
		exit(1);
	}

    // bind socket to port using the socket information descriptor
	if (bind(server_fd, (struct sockaddr *)&sa, sizeof sa) == -1) {
		perror("bind failed");
        // clean up if error
		close(server_fd);
		return 1;
	}

    // configure socket to listen for incoming connections, allow 5 backup connections
	if (listen(server_fd, 5) == -1) {
		perror("listen failed");
        // clean up if error
		close(server_fd);
		return 1;
	}

    // main loop
	while (1) {
        // accept incoming connections
		int client_fd = accept(server_fd, NULL, NULL);

		if (client_fd < 0) {
			perror("accept failed");
			close(server_fd);
			return 1;
		}
        printf("client connected\n");
        add_request(request_queue, client_fd);
		while(request_queue->requests[request_queue->done_head].done == 1 && request_queue->requests[request_queue->done_head].handled == 1){
			// wait for the request to be handled
			printf("closing client fd: %d\n", request_queue->requests[request_queue->done_head].client_fd);
			close(request_queue->requests[request_queue->done_head].client_fd);
			request_queue->done_head = (request_queue->done_head + 1) % QUEUE_SIZE;
		}
	}

	close(server_fd);
    return 0;
}