/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/client/client.c
 * Module                    : Client Utilities
 * Last Updating Author      : shady0503
 * Last Update               : 01/23/2026
 * Version                   : 1.0.0
 * 
 * Description:
 *  Simple client to test the MemoraDB server.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

//-- Request POSIX.1-2008 APIs from system headers --//
#define _POSIX_C_SOURCE 200809L
#include "client.h"
#include "resp_parser.h"
#include "../utils/logo.h"
#include "history/history.h"
#include <sys/time.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    const char *server_ip = "127.0.0.1";
    
    //-- Override server IP if provided as argument --//
    if (argc > 1) {
        server_ip = argv[1];
    }
    
    display_memoradb_logo();
    printf("\n\n");
    
    printf("===============================================\n");
    printf("     MemoraDB Client - Testing Interface      \n");
    printf("        Connecting to: %s:%d               \n", server_ip, SERVER_PORT);
    printf("===============================================\n");
    
    //-- Create socket --//
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        printf("[Client: ERROR] Socket creation failed: %s\n", strerror(errno));
        return 1;
    }
    
    printf("[Client: INFO] Socket created successfully.\n");
    
    //-- Configure server address --//
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf("[Client: ERROR] Invalid address: %s\n", server_ip);
        close(client_fd);
        return 1;
    }

    //-- Connect to server --//
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("[Client: ERROR] Connection failed: %s\n", strerror(errno));
        close(client_fd);
        return 1;
    }

    printf("[Client: INFO] Connected to MemoraDB server\n\n");
    printf("[Client: INFO] Type commands (EXIT or QUIT to close the connection):\n");
    printf("=======================================================================\n");

    //-- Initialize command history --//
    history_init();

    //-- Interactive command loop --//
    char *line;
    while ((line = history_readline("MemoraDB> ")) != NULL) {
        //-- Copy to command buffer for compatibility with existing code --//
        strncpy(command, line, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';
        
        //-- Free the line returned by history_readline --//
        history_free(line);
        
        //-- Handle quit/exit commands --//
        if (strcasecmp(command, "quit") == 0 || strcasecmp(command, "exit") == 0) {
            printf("[Client: INFO] Disconnecting...\n");
            break;
        }
        
        //-- Skip empty commands --//
        if (strlen(command) == 0) {
            continue;
        }

        //-- Add command to history --//
        history_add(command);
        
        char * argv[BUFFER_SIZE];
        int argc = 0;

        char * token = strtok(command, " ");
        while(token != NULL && argc < BUFFER_SIZE){
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        int offset = 0;
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "*%d\r\n", argc);
        for(int i = 0; i < argc; i++){
            offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "$%lu\r\n%s\r\n", strlen(argv[i]), argv[i]);
        }

        //-- Capture start time before sending --//
        struct timeval tv_start, tv_end;
        gettimeofday(&tv_start, NULL);

        if (send(client_fd, buffer, offset, 0) < 0) {
            printf("[Client: ERROR] Failed to send command: %s\n", strerror(errno));
            break;
        }

        //-- Receive response --//
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        //-- Capture end time after receiving --//
        gettimeofday(&tv_end, NULL);
        
        if (bytes_received < 0) {
            printf("[Client: ERROR] Failed to receive response: %s\n", strerror(errno));
            break;
        } else if (bytes_received == 0) {
            printf("[Client: INFO] Server closed connection.\n");
            break;
        }

        //-- Display response --//
        buffer[bytes_received] = '\0';
        
        //-- Calculate round-trip latency --//
        double latency_ms = (tv_end.tv_sec - tv_start.tv_sec) * 1000.0
                          + (tv_end.tv_usec - tv_start.tv_usec) / 1000.0;

        //-- Check if response starts with RESP protocol markers --//
        if (buffer[0] == '+' || buffer[0] == '-' || buffer[0] == ':' ||
            buffer[0] == '$' || buffer[0] == '*') {
            //-- Parse and display RESP response --//
            int parsed = parse_and_display_resp(buffer);
            if (parsed == -1) {
                printf("Error parsing RESP response\n");
                printf("Raw response: %s", buffer);
            }
        } else {
            //-- Display raw response (for MemoraDB logging messages) --//
            printf("%s", buffer);
        }

        //-- Display wall-clock timestamp and round-trip latency --//
        struct tm tm_storage;
        struct tm *tm_info = localtime_r(&tv_end.tv_sec, &tm_storage);
        char time_buf[32];
        strftime(time_buf, sizeof(time_buf), "%H:%M:%S", tm_info);
        printf("[%s.%03ld] (%.2f ms)\n", time_buf, tv_end.tv_usec / 1000, latency_ms);
    }
    
    //-- Cleanup history --//
    history_cleanup();
    
    close(client_fd);
    printf("[Client: INFO] Connection closed.\n");
    return 0;
}
