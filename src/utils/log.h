/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/log.h
 * Module                    : Utilities
 * Last Updating Author      : kei077
 * Last Update               : 07/19/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Helper methods for logging.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */
#ifndef MEMORADB_LOG_H
#define MEMORADB_LOG_H

#include <stdio.h>
#include <time.h>

/* ==================== Log Levels ==================== */
typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
} log_level_t;

/**
 * Logs a message with a specified log level.
 * 
 * @param level   The log level (INFO, WARN, ERROR, DEBUG).
 * @param format  The printf-style format string.
 * @param ...     Additional arguments for formatting.
 */
void log_message(log_level_t level, const char *format, ...);

#endif // MEMORADB_LOG_H
