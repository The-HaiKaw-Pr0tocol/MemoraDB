/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/log.c
 * Module                    : Utilities
 * Last Updating Author      : kei077
 * Last Update               : 07/19/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Header file for logging helper methods.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */
#include "log.h"
#include <stdarg.h>

void log_message(log_level_t level, const char *format, ...) {
    const char *prefix;
    switch (level) {
        case LOG_INFO:  prefix = "[MemoraDB: INFO] "; break;
        case LOG_WARN:  prefix = "[MemoraDB: WARN] "; break;
        case LOG_ERROR: prefix = "[MemoraDB: ERROR]"; break;
        case LOG_DEBUG: prefix = "[MemoraDB: DEBUG]"; break;
        default:        prefix = "[MemoraDB] "; break;
    }

    // Print prefix
    fprintf(stdout, "%s ", prefix);

    // Print formatted message
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);

    // Add newline
    fprintf(stdout, "\n");
}
