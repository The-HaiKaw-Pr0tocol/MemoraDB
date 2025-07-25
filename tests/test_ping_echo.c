#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define MAX_TOKENS 16

extern void* handle_client(void*);
extern int parse_command(char *input, char *tokens[], int max_tokens);
extern void dispatch_command(int client_fd, char *tokens[], int token_count);

void test_ping_echo() {
    int sv[2]; 
    char buffer[BUFFER_SIZE];
    char *tokens[MAX_TOKENS];

    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0);

    pthread_t tid;
    int *server_fd = malloc(sizeof(int));
    *server_fd = sv[1];
    pthread_create(&tid, NULL, handle_client, server_fd);
    pthread_detach(tid);

    // --- PING test ---
    const char *ping_cmd = "*1\r\n$4\r\nPING\r\n";
    write(sv[0], ping_cmd, strlen(ping_cmd));
    usleep(100000); // response time 
    int bytes = read(sv[0], buffer, sizeof(buffer) - 1);
    buffer[bytes] = '\0';
    assert(strstr(buffer, "+PONG") != NULL);

    // --- ECHO test ---
    const char *echo_cmd = "*2\r\n$4\r\nECHO\r\n$5\r\nhello\r\n";
    write(sv[0], echo_cmd, strlen(echo_cmd));
    usleep(100000); // response time
    bytes = read(sv[0], buffer, sizeof(buffer) - 1);
    buffer[bytes] = '\0';
    assert(strstr(buffer, "hello") != NULL);

    close(sv[0]);
}

int main() {
    test_ping_echo();
    printf("PING and ECHO tests passed.\n");
    return 0;
}
