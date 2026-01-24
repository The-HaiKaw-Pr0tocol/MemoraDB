/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/client/history.h
 * Module                    : History Management Header
 * Last Updating Author      : shady0503
 * Last Update               : 01/23/2026
 * Version                   : 1.0.0
 * 
 * Description:
 *  Header file for command history management with filtering.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>


#define HISTORY_MAX_LEN 100


static char *last_command = NULL;
static int initialized = 0;
/**
 * Initialize the history system
 * Sets up linenoise history with max length of 100
 * 
 * @return void
 */
void history_init(void);

/**
 * Add a command to history with filtering
 * Skips empty commands and consecutive duplicates
 * 
 * @param cmd The command string to add
 * @return int 0 on success, -1 on failure
 */
int history_add(const char *cmd);

/**
 * Cleanup history resources
 * 
 * @return void
 */
void history_cleanup(void);

#endif // HISTORY_H

