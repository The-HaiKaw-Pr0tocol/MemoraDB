/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/client/history.c
 * Module                    : History Management
 * Last Updating Author      : shady0503
 * Last Update               : 01/23/2026
 * Version                   : 1.0.0
 * 
 * Description:
 *  Command history management with filtering for MemoraDB client.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

 #include "history.h"
#include "linenoise.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/**
 * Check if a string is empty or whitespace-only
 */
static int is_empty_or_whitespace(const char *str) {
    if (str == NULL || *str == '\0') {
        return 1;
    }
    
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/**
 * Check if two strings are equal (case-sensitive)
 */
static int strings_equal(const char *a, const char *b) {
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;
    return strcmp(a, b) == 0;
}

void history_init(void) {
    if (initialized) {
        return;
    }
    
    linenoiseHistorySetMaxLen(HISTORY_MAX_LEN);
    
    initialized = 1;
}

int history_add(const char *cmd) {
    if (!initialized) {
        history_init();
    }
    
    if (cmd == NULL) {
        return -1;
    }
    
    if (is_empty_or_whitespace(cmd)) {
        return 0;
    }
    
    if (last_command != NULL && strings_equal(last_command, cmd)) {
        return 0;
    }
    
    int result = linenoiseHistoryAdd(cmd);
    
    if (result != 0) {
        char *new_cmd = strdup(cmd);
        if (new_cmd == NULL) {
            return -1;
        }
        
        free(last_command);
        last_command = new_cmd;
        return 0;
    }
    
    return 0;
}

void history_cleanup(void) {
    if (last_command != NULL) {
        free(last_command);
        last_command = NULL;
    }
    
    initialized = 0;
}