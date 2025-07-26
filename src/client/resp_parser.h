/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/client/resp_parser.h
 * Module                    : Client-Side RESP Parser
 * Last Updating Author      : Haitam Bidiouane
 * Last Update               : 07/26/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Client-side RESP (Redis Serialization Protocol) parser for MemoraDB.
 *  Handles parsing and formatting of server responses for human-readable display.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef RESP_PARSER_H
#define RESP_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ==================== CONSTANTS ==================== */
#define RESP_MAX_DEPTH      10
#define RESP_BUFFER_SIZE    4096

/* ==================== ENUMERATIONS ==================== */
typedef enum {
    RESP_SIMPLE_STRING,     // +
    RESP_ERROR,             // -
    RESP_INTEGER,           // :
    RESP_BULK_STRING,       // $
    RESP_ARRAY,             // *
    RESP_NULL,              // $-1
    RESP_UNKNOWN
} resp_type_t;

/* ==================== STRUCTURES ==================== */
typedef struct {
    resp_type_t type;
    union {
        char *string_value;
        long long integer_value;
        struct {
            char **elements;
            int count;
        } array_value;
    } data;
} resp_object_t;

/* ==================== FUNCTION DECLARATIONS ==================== */

/**
 * Parse RESP protocol response from server
 * 
 * @param input Input buffer containing RESP formatted response
 * @param response Pointer to store parsed response object
 * @return Number of bytes consumed, or -1 on error
 */
int parse_resp_response(const char *input, resp_object_t *response);

/**
 * Format and display RESP response object in human-readable format
 * 
 * @param response Parsed RESP response object
 * @param indent Indentation level for nested structures
 */
void display_resp_response(const resp_object_t *response, int indent);

/**
 * Free memory allocated for RESP response object
 * 
 * @param response RESP response object to free
 */
void free_resp_response(resp_object_t *response);

/**
 * Parse and display RESP response in one call
 * 
 * @param input Input buffer containing RESP formatted response
 * @return Number of bytes consumed, or -1 on error
 */
int parse_and_display_resp(const char *input);

#endif // RESP_PARSER_H