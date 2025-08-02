/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/integration_test.c
 * Module                    : Network Integration Tests
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 * 
 * Description:
 *  Integration tests for network client-server communication over TCP.
 *  Automatically starts server, runs tests, and cleans up processes.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "test_framework.h"

#define TEST_PORT 6379
#define BUFFER_SIZE 1024

static pid_t server_pid = -1;

void cleanup_processes() {
    if (server_pid > 0) {
        printf("Shutting down server (PID: %d)...\n", server_pid);
        kill(server_pid, SIGTERM);
        waitpid(server_pid, NULL, 0);
        server_pid = -1;
    }
}

void signal_handler(int sig) {
    cleanup_processes();
    exit(1);
}

int start_server() {
    printf("Starting MemoraDB server on port %d...\n", TEST_PORT);
    
    server_pid = fork();
    if (server_pid == 0) {
        //-- Child process - start server --//
        execl("./server", "server", NULL);
        perror("Failed to start server");
        exit(1);
    } else if (server_pid < 0) {
        perror("Failed to fork server process");
        return -1;
    }
    
    //-- Give server more time to start and bind to port --//
    sleep(3);
    
    //-- Test if server is actually listening --//
    int test_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in test_addr = {0};
    test_addr.sin_family = AF_INET;
    test_addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &test_addr.sin_addr);
    
    if (connect(test_fd, (struct sockaddr*)&test_addr, sizeof(test_addr)) < 0) {
        close(test_fd);
        TEST_ERROR("Server failed to bind to port - check if port is already in use");
        return -1;
    }
    close(test_fd);
    
    return 0;
}

int create_test_client() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) return -1;
    
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(client_fd);
        return -1;
    }
    
    return client_fd;
}

void test_set_get_integration() {
    printf("Testing SET/GET network integration...\n");
    
    int client_fd = create_test_client();
    if (client_fd == -1) {
        TEST_ERROR("Failed to connect to server for SET/GET test");
        return;
    }
    
    //-- Send SET command --//
    char set_cmd[] = "*3\r\n$3\r\nSET\r\n$8\r\ntest_key\r\n$10\r\ntest_value\r\n";
    send(client_fd, set_cmd, strlen(set_cmd), 0);
    
    char buffer[BUFFER_SIZE];
    recv(client_fd, buffer, sizeof(buffer), 0);
    TEST_ASSERT(strstr(buffer, "OK") != NULL, "SET command should return OK");
    
    //-- Send GET command --//
    char get_cmd[] = "*2\r\n$3\r\nGET\r\n$8\r\ntest_key\r\n";
    send(client_fd, get_cmd, strlen(get_cmd), 0);
    
    memset(buffer, 0, sizeof(buffer));
    recv(client_fd, buffer, sizeof(buffer), 0);
    TEST_ASSERT(strstr(buffer, "test_value") != NULL, "GET should return test_value");
    
    close(client_fd);
    TEST_SUCCESS("SET/GET integration test passed");
}

void test_list_operations_integration() {
    printf("Testing list operations network integration...\n");
    
    int client_fd = create_test_client();
    if (client_fd == -1) {
        TEST_ERROR("Failed to connect to server for list operations test");
        return;
    }
    
    //-- Test RPUSH --//
    char rpush_cmd[] = "*3\r\n$5\r\nRPUSH\r\n$9\r\ntest_list\r\n$6\r\nweasel\r\n";
    send(client_fd, rpush_cmd, strlen(rpush_cmd), 0);
    
    char buffer[BUFFER_SIZE];
    recv(client_fd, buffer, sizeof(buffer), 0);
    TEST_ASSERT(strstr(buffer, ":1") != NULL, "RPUSH should return list length 1");
    
    //------------------------------------------------------------------------------------//
    //------------------------------------ Test LRANGE -----------------------------------//
    /*-- Since LRANGE is supposed to return the entire 'indexed' list, recv() may not send
         the entire RESP encoding in a single TCP packet, we'll unbuffer multiple times --*/
    //------------------------------------------------------------------------------------//

    char lrange_cmd[] = "*4\r\n$6\r\nLRANGE\r\n$9\r\ntest_list\r\n$1\r\n0\r\n$2\r\n-1\r\n";
    send(client_fd, lrange_cmd, strlen(lrange_cmd), 0);
    
    memset(buffer, 0, sizeof(buffer));

    int total_received = 0;
    int attempts = 0;

    while (attempts++ < 5) {
        int n = recv(client_fd, buffer + total_received, BUFFER_SIZE - total_received - 1, 0);
        if (n <= 0) break;
        total_received += n;
        buffer[total_received] = '\0';

        if (strstr(buffer, "weasel") != NULL) {
            break;
        }

        usleep(100 * 1000);  //-- 100ms hold time before we retry --//
    }

    TEST_ASSERT(strstr(buffer, "weasel") != NULL,
                "LRANGE should return \"weasel\" RESP serialized");

    close(client_fd);
    TEST_SUCCESS("List operations network integration test passed");
}

int main() {
    init_test_framework();
    printf("=== Network Integration Tests ===\n");
    
    //-- Set up signal handlers for cleanup --//
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    //-- Start the server --//
    if (start_server() < 0) {
        TEST_ERROR("Failed to start server - aborting integration tests");
        cleanup_processes();
        save_test_results();
        return 1;
    }
    
    TEST_SUCCESS("Server started successfully");
    
    //-- Run tests --//
    test_set_get_integration();
    test_list_operations_integration();

    //-- Clean up --//
    cleanup_processes();
    
    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}