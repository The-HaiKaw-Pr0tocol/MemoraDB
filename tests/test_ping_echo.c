/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_ping_echo.c
 * Module                    : Client-Server Socket Communication Tests
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 * 
 * Description:
 *  Unit tests for direct client-server socket communication using socketpair.
 *  Tests PING and ECHO commands through the server's handle_client function.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include "test_framework.h"

#define BUFFER_SIZE 1024
#define MAX_TOKENS 16

extern void* handle_client(void*);
extern int parse_command(char *input, char *tokens[], int max_tokens);
extern void dispatch_command(int client_fd, char *tokens[], int token_count);

void test_ping_echo() {
    printf("Testing PING and ECHO commands via socketpair...\n");
    int sv[2]; 
    char buffer[BUFFER_SIZE];

    TEST_ASSERT(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "Socketpair creation failed");

    pthread_t tid;
    int *server_fd = malloc(sizeof(int));
    *server_fd = sv[1];
    pthread_create(&tid, NULL, handle_client, server_fd);
    pthread_detach(tid);

    // --- PING test --- //
    const char *ping_cmd = "*1\r\n$4\r\nPING\r\n";
    write(sv[0], ping_cmd, strlen(ping_cmd));
    usleep(100000);
    int bytes = read(sv[0], buffer, sizeof(buffer) - 1);
    buffer[bytes] = '\0';
    TEST_ASSERT(strstr(buffer, "+PONG") != NULL, "PING command should return +PONG");

    // --- ECHO test --- //
    const char *echo_cmd = "*2\r\n$4\r\nECHO\r\n$5\r\nhello\r\n";
    write(sv[0], echo_cmd, strlen(echo_cmd));
    usleep(100000);
    bytes = read(sv[0], buffer, sizeof(buffer) - 1);
    buffer[bytes] = '\0';
    TEST_ASSERT(strstr(buffer, "hello") != NULL, "ECHO command should return hello");

    close(sv[0]);
    
    TEST_SUCCESS("PING and ECHO socket communication test passed");
}

int main() {
    init_test_framework();
    printf("=== Client-Server Socket Communication Tests ===\n");
    
    test_ping_echo();
    
    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}
