/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 *
 * File                      : src/server/client_context.h
 * Module                    : Client Context
 * Last Updating Author      : Youssef Bouraoui
 * Last Update               : 02/01/2026
 * Version                   : 1.0.0
 *
 * Description:
 *  Header file for client context management
 *
 *
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef MEMORADB_CLIENT_CONTEXT_H
#define MEMORADB_CLIENT_CONTEXT_H

typedef struct {
  int client_fd;
  char ip_address[16]; // it is for ipv4 but if we want ipv6 we can use 46
  int port; // to distinguish between different connections coming from the same
            // client
} ClientContext;

#endif