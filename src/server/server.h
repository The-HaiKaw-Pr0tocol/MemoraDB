/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/server/server.h
 * Module                    : MemoraDB Server Header
 * Last Updating Author      : sch0penheimer
 * Last Update               : 07/22/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Main header file for the MemoraDB server.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef MEMORADB_MAIN_H
#define MEMORADB_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>

//-- Config Constants --//
#define BUFFER_SIZE 1024
#define MAX_TOKENS 16
#define DEFAULT_PORT 6379
#define CONNECTION_BACKLOG 5
#define RESP_TERMINATOR_LEN 2

//-- Implemented Command Types for the RESP Protocol --//
enum command_t {
    CMD_PING,
    CMD_ECHO,
    CMD_SET,
    CMD_GET,
    CMD_UNKNOWN
};

extern volatile int server_running;
extern int server_fd_global;


/**
 * Parse RESP (Redis Serialization Protocol) command
 * @param input: Input buffer containing RESP formatted command
 * @param tokens: Array to store parsed tokens
 * @param max_tokens: Maximum number of tokens to parse
 * @return: Number of tokens parsed, -1 on error
 */
int parse_command(char *input, char *tokens[], int max_tokens);

/**
 * Identify command type from command string
 * @param cmd: Command string to identify
 * @return: Command type enum value
 */
enum command_t identify_command(const char *cmd);

/**
 * Dispatch and execute command based on tokens
 * @param client_fd: Client socket file descriptor
 * @param tokens: Array of parsed command tokens
 * @param token_count: Number of tokens in array
 */
void dispatch_command(int client_fd, char *tokens[], int token_count);

/**
 * Handle client connection in separate thread
 * @param arg: Pointer to client file descriptor
 * @return: NULL on completion
 */
void *handle_client(void *arg);

#endif