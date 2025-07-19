/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : main.c
 * Module                    : MemoraDB Server
 * Last Updating Author      : sch0penheimer
 * Last Update               : 07/19/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Main MemoraDB server.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */


#include "main.h"

int parse_command(char * input, char * tokens[], int max_tokens){
    int counter = 0;
    char * cur = input;

    if(*cur != '*') return -1;

    int num_args = atoi(cur+1);
    cur = strstr(cur, "\r\n");
    if(!cur) return -1;

    cur += 2;

    for(int i = 0; i < num_args && counter < max_tokens; i++){
        if(*cur != '$') return -1;

        int len = atoi (cur+1);
        cur = strstr(cur, "\r\n");
        if (!cur) return -1;
        cur += 2;
        tokens[counter] = cur;
        tokens[counter][len] = '\0';
        cur += len + 2;
        counter++;
    }
    return counter;
}

enum Commands identify_command(const char * cmd){
    if(strcasecmp(cmd, "PING") == 0) return CMD_PING;
    if(strcasecmp(cmd, "ECHO") == 0) return CMD_ECHO;
    return CMD_UNKNOWN;
}

void dispatch_command(int client_fd, char * tokens[], int token_count){
    if(token_count == 0){
        dprintf(client_fd, "Empty Command");
        return;
    }

    enum Commands cmd = identify_command(tokens[0]);

    switch (cmd)
    {
    case CMD_PING:
        dprintf(client_fd, "+PONG\r\n");
        break;
    case CMD_ECHO:
        if(token_count < 2){
            dprintf(client_fd, "ECHO needs one argument\r\n");
        } else {
           dprintf(client_fd, "$%lu\r\n%s\r\n", strlen(tokens[1]), tokens[1]);
        }
        break;
    default:
    dprintf(client_fd, "Unknown command '%s'\r\n", tokens[0]);
        break;
    }
}

//-- Thread Starting Function to handle client connections --//
void *handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    char *tokens[MAX_TOKENS];
    
    while (1) {
        ssize_t bytes = recv(client_fd, buffer, sizeof(buffer)-1, 0);
        if (bytes <= 0) {
            break;
        }
        buffer[bytes] = '\0';
        int token_count = parse_command(buffer, tokens, MAX_TOKENS);
        if(token_count < 1){
            dprintf(client_fd, "Invalid RESP format\r\n");
            continue;
        }
        dispatch_command(client_fd, tokens, token_count);
    }

    close(client_fd);
    printf("Client disconnected\n");
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