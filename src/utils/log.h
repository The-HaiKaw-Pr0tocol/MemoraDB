/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 *
 * File                      : src/utils/log.h
 * Module                    : utilities
 * Last Updating Author      : shady0503
 * Last Update               : 02/10/2026
 * Version                   : 1.0.0
 *
 * Description:
 *  Header for the MemoraDB logging system. Provides a POSIX-compliant,
 *  UI-friendly logging API with log-level based colorization and
 *  printf-style formatting.
 *
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef MEMORADB_LOG_H
#define MEMORADB_LOG_H

#include <stdio.h>

/* ==================== Log Levels ==================== */
typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
} log_level_t;

/**
 * Log a formatted message with the given log level.
 *
 * All log output in MemoraDB MUST pass through this interface.
 *
 * @param level   Log level.
 * @param format  printf-style format string.
 * @param ...     Arguments for formatting.
 */
void log_message(log_level_t level, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

#endif /* MEMORADB_LOG_H */
