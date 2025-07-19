/**
 * File : MemoraDB/src/main.c
 * Last Update Author : sch0penheimer
 * Last Update : 07/18/2025
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

//-- Thread Starting Function to handle client connections --//
void* handle_client(void* arg) {
    int client_fd = *(int*) arg;
    free(arg);
    
    char buffer[1024];
    const char *response = "+PONG\r\n";

    while (recv(client_fd, buffer, sizeof(buffer)-1, 0) > 0) {
		//-- Ensure null-termination --//
		buffer[sizeof(buffer)-1] = '\0'; 
		printf("[MemoraDB: INFO] Received command: %s", buffer);
        send(client_fd, response, strlen(response), 0);
    }
    
    close(client_fd);
    return NULL;
}

int main() {
    //-- Disable output buffering --//
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
	printf("===============================================\n");
	printf("   MemoraDB - Lightweight In-Memory Database   \n");
	printf("             Running on Port: 6379            \n");
	printf("===============================================\n");

	printf("MemoraDB Server started. Awaiting connections...\n");
    
    int server_fd, client_addr_len;
    struct sockaddr_in client_addr;
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("[MemoraDB: ERROR] Socket creation failed: %s...\n", strerror(errno));
        return 1;
    }
	
	printf("[MemoraDB: INFO] Socket created successfully.\n");
    
    struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
                                     .sin_port = htons(6379),
                                     .sin_addr = { htonl(INADDR_ANY) },
                                    };
    
    if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("[MemoraDB: ERROR] Bind failed: %s\n", strerror(errno));
        return 1;
    }
    
    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        printf("[MemoraDB: ERROR] Listen failed: %s \n", strerror(errno));
        return 1;
    }

    printf("[MemoraDB: INFO] Waiting for clients to connect...\n");
    client_addr_len = sizeof(client_addr);

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_fd < 0) {
            printf("[MemoraDB: ERROR] Accept failed: %s \n", strerror(errno));
            continue;
        }

        printf("[MemoraDB: INFO] Client connected\n");

        int *client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client_fd_ptr) != 0) {
            printf("[MemoraDB: ERROR] pthread_create failed: %s\n", strerror(errno));
            close(client_fd);
            free(client_fd_ptr);
            continue;
        }
        pthread_detach(thread);
    }

	printf("[MemoraDB: INFO] Server shutting down...\n");
    close(server_fd);
    return 0;
}