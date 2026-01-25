/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/client/client.h
 * Module                    : Client Utilities Header
 * Last Updating Author      : shady0503
 * Last Update               : 01/23/2026
 * Version                   : 1.0.0
 * 
 * Description:
 *   Header file for MemoraDB TCP client utilities.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

/* ==================== CONSTANTS ==================== */
#define SERVER_PORT         6379
#define BUFFER_SIZE         1024
#define DEFAULT_SERVER_IP   "127.0.0.1"
#define MAX_ARGS           64

/* ==================== STRUCTURES ==================== */
typedef struct {
    int socket_fd;
    struct sockaddr_in server_addr;
    const char *server_ip;
    int port;
    int connected;
} client_t;

#endif