/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/server/main.c
 * Module                    : MemoraDB Server
 * Last Updating Author      : sch0penheimer
 * Last Update               : 07/22/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Main MemoraDB server.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "server.h"
#include "../utils/log.h"
#include "../utils/hashTable.h"

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

enum command_t identify_command(const char * cmd){
    if(strcasecmp(cmd, "PING") == 0) return CMD_PING;
    if(strcasecmp(cmd, "ECHO") == 0) return CMD_ECHO;
    if(strcasecmp(cmd, "SET") == 0) return CMD_SET;
    if(strcasecmp(cmd, "GET") == 0) return CMD_GET;
    return CMD_UNKNOWN;
}

void dispatch_command(int client_fd, char * tokens[], int token_count){
    if(token_count == 0){
        dprintf(client_fd, "[MemoraDB: ERROR] Empty Command\n");
        return;
    }

    enum command_t cmd = identify_command(tokens[0]);

    switch (cmd)
    {
    case CMD_PING:
        dprintf(client_fd, "PONG\r\n");
        break;
    case CMD_ECHO:
        if(token_count < 2){
            dprintf(client_fd, "[MemoraDB: WARN] ECHO needs one argument\n");
        } else {
            dprintf(client_fd, "%s\r\n", tokens[1]);
        }
        break;
    case CMD_SET:
        if(token_count < 3){
            dprintf(client_fd, "[MemoraDB: WARN] SET needs key and value\r\n");
        } else {
            set_value(tokens[1], tokens[2]);
            dprintf(client_fd, "OK\r\n");
        }
        break;
    case CMD_GET:
        if(token_count < 2){
            dprintf(client_fd, "[MemoraDB: WARN] GET needs key\r\n");
        } else {
            const char *value = get_value(tokens[1]);
            if(value)
                dprintf(client_fd, "%s\r\n", value);
            else
                dprintf(client_fd, "nil\r\n");
        }
        break;
    default:
        dprintf(client_fd, "[MemoraDB: WARN] Unknown command '%s'\n", tokens[0]);
        break;
    }
}

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
            dprintf(client_fd, "[MemoraDB: WARN] Invalid RESP format\r\n");
            continue;
        }
        dispatch_command(client_fd, tokens, token_count);
    }

    close(client_fd);
    log_message(LOG_INFO, "Client disconnected");
    return NULL;
}

int main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    printf("===============================================\n");
    printf("  MemoraDB - Lightweight In-Memory Database    \n");
    printf("             Running on Port: 6379             \n");
    printf("===============================================\n");
    log_message(LOG_INFO, "MemoraDB Server started. Awaiting connections...");

    int server_fd, client_addr_len;
    struct sockaddr_in client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        log_message(LOG_ERROR, "Socket creation failed: %s", strerror(errno));
        return 1;
    }
    log_message(LOG_INFO, "Socket created successfully.");

    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(6379),
        .sin_addr = { htonl(INADDR_ANY) },
    };

    if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
        log_message(LOG_ERROR, "Bind failed: %s", strerror(errno));
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        log_message(LOG_ERROR, "Listen failed: %s", strerror(errno));
        return 1;
    }

    log_message(LOG_INFO, "Waiting for clients to connect...");
    client_addr_len = sizeof(client_addr);

    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_fd < 0) {
            log_message(LOG_ERROR, "Accept failed: %s", strerror(errno));
            continue;
        }

        log_message(LOG_INFO, "Client connected");

        int *client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client_fd_ptr) != 0) {
            log_message(LOG_ERROR, "pthread_create failed: %s", strerror(errno));
            close(client_fd);
            free(client_fd_ptr);
            continue;
        }
        pthread_detach(thread);
    }

    log_message(LOG_INFO, "Server shutting down...");
    close(server_fd);
    return 0;
}
