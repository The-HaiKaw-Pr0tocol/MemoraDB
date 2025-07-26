/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/client/resp_parser.c
 * Module                    : Client-Side RESP Parser
 * Last Updating Author      : Haitam Bidiouane
 * Last Update               : 07/26/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Client-side RESP (Redis Serialization Protocol) parser implementation.
 *  Handles parsing and formatting of server responses for human-readable display.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "resp_parser.h"

/*
 * RESP Parser Implementation
 * 
 * This parser handles all RESP data types:
 * - Simple Strings (+): +OK\r\n
 * - Errors (-): -ERR message\r\n
 * - Integers (:): :1000\r\n
 * - Bulk Strings ($): $6\r\nfoobar\r\n
 * - Arrays (*): *2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n
 * - Null Bulk Strings: $-1\r\n
 */

static const char* find_crlf(const char* str) {
    return strstr(str, "\r\n");
}

static int parse_integer_from_resp(const char* str) {
    return atoi(str + 1); // Skip the type indicator
}

static char* extract_string_until_crlf(const char* start, const char* end) {
    int len = end - start;
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}

int parse_resp_response(const char *input, resp_object_t *response) {
    if (!input || !response) return -1;
    
    const char *current = input;
    const char *crlf;
    
    // Initialize response
    memset(response, 0, sizeof(resp_object_t));
    
    switch (current[0]) {
        case '+': {
            // Simple String
            response->type = RESP_SIMPLE_STRING;
            crlf = find_crlf(current);
            if (!crlf) return -1;
            
            response->data.string_value = extract_string_until_crlf(current + 1, crlf);
            return (crlf + 2) - input;
        }
        
        case '-': {
            // Error
            response->type = RESP_ERROR;
            crlf = find_crlf(current);
            if (!crlf) return -1;
            
            response->data.string_value = extract_string_until_crlf(current + 1, crlf);
            return (crlf + 2) - input;
        }
        
        case ':': {
            // Integer
            response->type = RESP_INTEGER;
            crlf = find_crlf(current);
            if (!crlf) return -1;
            
            response->data.integer_value = parse_integer_from_resp(current);
            return (crlf + 2) - input;
        }
        
        case '$': {
            // Bulk String
            crlf = find_crlf(current);
            if (!crlf) return -1;
            
            int length = parse_integer_from_resp(current);
            current = crlf + 2;
            
            if (length == -1) {
                // Null bulk string
                response->type = RESP_NULL;
                return current - input;
            }
            
            response->type = RESP_BULK_STRING;
            response->data.string_value = malloc(length + 1);
            if (!response->data.string_value) return -1;
            
            strncpy(response->data.string_value, current, length);
            response->data.string_value[length] = '\0';
            
            return (current + length + 2) - input; // +2 for final \r\n
        }
        
        case '*': {
            // Array
            response->type = RESP_ARRAY;
            crlf = find_crlf(current);
            if (!crlf) return -1;
            
            int count = parse_integer_from_resp(current);
            current = crlf + 2;
            
            response->data.array_value.count = count;
            if (count == 0) {
                response->data.array_value.elements = NULL;
                return current - input;
            }
            
            response->data.array_value.elements = malloc(count * sizeof(char*));
            if (!response->data.array_value.elements) return -1;
            
            for (int i = 0; i < count; i++) {
                // For simplicity, we'll store array elements as strings
                // Parse each element as bulk string
                if (current[0] != '$') return -1;
                
                crlf = find_crlf(current);
                if (!crlf) return -1;
                
                int elem_length = parse_integer_from_resp(current);
                current = crlf + 2;
                
                if (elem_length == -1) {
                    response->data.array_value.elements[i] = strdup("(nil)");
                } else {
                    response->data.array_value.elements[i] = malloc(elem_length + 1);
                    if (!response->data.array_value.elements[i]) return -1;
                    
                    strncpy(response->data.array_value.elements[i], current, elem_length);
                    response->data.array_value.elements[i][elem_length] = '\0';
                    current += elem_length + 2; // +2 for \r\n
                }
            }
            
            return current - input;
        }
        
        default:
            response->type = RESP_UNKNOWN;
            return -1;
    }
}

void display_resp_response(const resp_object_t *response, int indent) {
    if (!response) return;
    
    char indent_str[64] = {0};
    for (int i = 0; i < indent && i < 63; i++) {
        indent_str[i] = ' ';
    }
    
    switch (response->type) {
        case RESP_SIMPLE_STRING:
            printf("%s%s\n", indent_str, response->data.string_value);
            break;
            
        case RESP_ERROR:
            printf("%s(error) %s\n", indent_str, response->data.string_value);
            break;
            
        case RESP_INTEGER:
            printf("%s(integer) %lld\n", indent_str, response->data.integer_value);
            break;
            
        case RESP_BULK_STRING:
            printf("%s\"%s\"\n", indent_str, response->data.string_value);
            break;
            
        case RESP_NULL:
            printf("%s(nil)\n", indent_str);
            break;
            
        case RESP_ARRAY:
            if (response->data.array_value.count == 0) {
                printf("%s(empty array)\n", indent_str);
            } else {
                for (int i = 0; i < response->data.array_value.count; i++) {
                    printf("%s%d) \"%s\"\n", indent_str, i + 1, 
                           response->data.array_value.elements[i]);
                }
            }
            break;
            
        case RESP_UNKNOWN:
        default:
            printf("%s(unknown response type)\n", indent_str);
            break;
    }
}

void free_resp_response(resp_object_t *response) {
    if (!response) return;
    
    switch (response->type) {
        case RESP_SIMPLE_STRING:
        case RESP_ERROR:
        case RESP_BULK_STRING:
            free(response->data.string_value);
            break;
            
        case RESP_ARRAY:
            if (response->data.array_value.elements) {
                for (int i = 0; i < response->data.array_value.count; i++) {
                    free(response->data.array_value.elements[i]);
                }
                free(response->data.array_value.elements);
            }
            break;
            
        case RESP_INTEGER:
        case RESP_NULL:
        case RESP_UNKNOWN:
        default:
            // No dynamic memory to free
            break;
    }
    
    memset(response, 0, sizeof(resp_object_t));
}

int parse_and_display_resp(const char *input) {
    resp_object_t response;
    int bytes_consumed = parse_resp_response(input, &response);
    
    if (bytes_consumed > 0) {
        display_resp_response(&response, 0);
        free_resp_response(&response);
    }
    
    return bytes_consumed;
}