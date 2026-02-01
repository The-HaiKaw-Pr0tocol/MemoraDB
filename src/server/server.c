/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/server/server.c
 * Module                    : MemoraDB Server
 * Last Updating Author      : Youssef Bouraoui
 * Last Update               : 02/01/2026
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
#include "../parser/parser.h"
#include "../utils/logo.h"
#include "client_context.h"

void *handle_client(void *arg) {
    ClientContext *client_context = (ClientContext*)arg;
    int client_fd = client_context->client_fd;
    char client_ip[16];
    strncpy(client_ip, client_context->ip_address, sizeof(client_ip));
    int client_port = client_context->port;
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
    log_message(LOG_INFO, "Client disconnected from %s:%d", client_ip, client_port);
    return NULL;
}

static int parse_port_env(const char *name, int def_port) {
    const char *s = getenv(name);
    if (!s || !*s) return def_port;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || *end != '\0' || v < 1 || v > 65535) {
        log_message(LOG_WARN, "Invalid %s='%s', falling back to %d", name, s, def_port);
        return def_port;
    }
    return (int)v;
}

#ifndef TESTING
int main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    display_memoradb_logo();
    printf("\n");

    log_message(LOG_INFO, "MemoraDB Server started successfully.");

    int server_fd;
    socklen_t client_addr_len;
    struct sockaddr_in client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        log_message(LOG_ERROR, "Socket creation failed: %s", strerror(errno));
        return 1;
    }
    
    int one = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));


    int port = parse_port_env("MEMORADB_PORT", 6379);
    const char *bind_ip = getenv("MEMORADB_BIND");
    if (!bind_ip || !*bind_ip) bind_ip = "0.0.0.0";

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, bind_ip, &serv_addr.sin_addr) != 1) {
        log_message(LOG_ERROR, "Invalid MEMORADB_BIND='%s'", bind_ip);
        return 1;
    }

    if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
        log_message(LOG_ERROR, "Bind failed: %s", strerror(errno));
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        log_message(LOG_ERROR, "Listen failed: %s", strerror(errno));
        return 1;
    }

    log_message(LOG_INFO, "Awaiting connections...");

    for(;;) {
        client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_fd < 0) {
            log_message(LOG_ERROR, "Accept failed: %s", strerror(errno));
            continue;
        }

        ClientContext *client_context = malloc(sizeof(ClientContext));
        if(!client_context){
          log_message(LOG_ERROR, "Failed to allocate client context");
          close(client_fd);
          continue;
        }

        client_context->client_fd = client_fd;

        if (inet_ntop(AF_INET, &client_addr.sin_addr, client_context->ip_address, sizeof(client_context->ip_address)) == NULL) {
          log_message(LOG_ERROR, "Failed to convert client IP: %s", strerror(errno));
          strncpy(client_context->ip_address, "unknown", sizeof(client_context->ip_address));
        }

        client_context->port = ntohs(client_addr.sin_port);

        log_message(LOG_INFO, "Client connected from %s:%d",client_context->ip_address,client_context->port);

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client_context) != 0) {
            log_message(LOG_ERROR, "pthread_create failed: %s", strerror(errno));
            close(client_fd);
            free(client_context);
            continue;
        }
        pthread_detach(thread);
    }

    log_message(LOG_INFO, "Server shutting down...");
    close(server_fd);
    return 0;
}
#endif