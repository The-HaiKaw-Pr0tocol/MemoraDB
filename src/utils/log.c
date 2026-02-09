/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 *
 * File                      : src/utils/log.c
 * Module                    : utilities
 * Last Updating Author      : shady0503
 * Last Update               : 02/10/2026
 * Version                   : 1.0.0
 *
 * Description:
 *  Implementation of the MemoraDB logging system. Provides a POSIX-compliant,
 *  UI-friendly logging API with log-level based colorization and
 *  printf-style formatting.
 *
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "log.h"

#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

/* ANSI color codes used only for TTY output */
#define COLOR_RESET  "\x1b[0m"
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED    "\x1b[31m"
#define COLOR_PURPLE "\x1b[35m"

/* Global logger state */
static struct {
    pthread_mutex_t mutex;
    int is_tty;   /* 1 if stdout is a TTY, 0 otherwise */
} g_log = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .is_tty = 0
};

static pthread_once_t log_init_once = PTHREAD_ONCE_INIT;

static const char *level_str(log_level_t lvl) {
    switch (lvl) {
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default:        return "UNKNW";
    }
}

static const char *level_color(log_level_t lvl) {
    switch (lvl) {
        case LOG_INFO:  return COLOR_GREEN;
        case LOG_WARN:  return COLOR_YELLOW;
        case LOG_ERROR: return COLOR_RED;
        case LOG_DEBUG: return COLOR_PURPLE;
        default:        return COLOR_RESET;
    }
}

/* Get current time and format timestamp as `YYYY-mm-dd HH:MM:SS.mmm` into `buf`. */
static void format_time(char *buf, size_t sz) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm tm;
    localtime_r(&tv.tv_sec, &tm);
    size_t len = strftime(buf, sz, "%Y-%m-%d %H:%M:%S", &tm);
    if (len >= sz) {
        buf[sz - 1] = '\0';
        return;
    }
    snprintf(buf + len, sz - len, ".%03ld", tv.tv_usec / 1000);
}

/* One-time initialization: detect TTY for stdout */
static void log_init(void) {
    g_log.is_tty = isatty(STDOUT_FILENO);
}

void log_shutdown(void) {
    fflush(stdout);
    fflush(stderr);
    /* mutex is statically initialized; we deliberately do not destroy it
     * to avoid use-after-destroy if any late logging occurs. */
}

/* Public logging entrypoint; thread-safe */
void log_message(log_level_t level, const char *format, ...) {
    pthread_once(&log_init_once, log_init);

    if (level < LOG_INFO || level > LOG_DEBUG) {
        return;
    }

    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&g_log.mutex);

    FILE *out = stdout;
    int tty = g_log.is_tty;

    char tbuf[32];
    format_time(tbuf, sizeof(tbuf));

    if (tty) {
        fprintf(out, "[%s] %s[MemoraDB: %s]%s ",
                tbuf, level_color(level), level_str(level), COLOR_RESET);
    } else {
        fprintf(out, "[%s] [MemoraDB: %s] ",
                tbuf, level_str(level));
    }

    vfprintf(out, format, args);
    fputc('\n', out);

    va_end(args);

    log_shutdown();

    pthread_mutex_unlock(&g_log.mutex);
}
