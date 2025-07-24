/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/server/server.h
 * Module                    : MemoraDB Server Header
 * Last Updating Author      : sch0penheimer
 * Last Update               : 07/24/2025
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

extern volatile int server_running;
extern int server_fd_global;

/**
 * Handle client connection in separate thread
 * @param arg: Pointer to client file descriptor
 * @return: NULL on completion
 */
void *handle_client(void *arg);

#endif