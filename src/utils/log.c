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
#include <sys/time.h>

void log_message(log_level_t level, const char *format, ...) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm tm_storage;
    struct tm *tm_info = localtime_r(&tv.tv_sec, &tm_storage);
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    const char *prefix;
    switch (level) {
        case LOG_INFO:  prefix = "[MemoraDB: INFO] "; break;
        case LOG_WARN:  prefix = "[MemoraDB: WARN] "; break;
        case LOG_ERROR: prefix = "[MemoraDB: ERROR]"; break;
        case LOG_DEBUG: prefix = "[MemoraDB: DEBUG]"; break;
        default:        prefix = "[MemoraDB] "; break;
    }

    fprintf(stdout, "[%s.%03ld] %s", time_buf, tv.tv_usec / 1000, prefix);

    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);

    fprintf(stdout, "\n");
}
