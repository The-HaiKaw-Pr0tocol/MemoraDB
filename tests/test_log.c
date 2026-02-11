/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 *
 * File                      : tests/test_log.c
 * Module                    : Logging Unit Tests
 * Last Updating Author      : shady0503
 * Last Update               : 02/10/2026
 * Version                   : 1.0.0
 *
 * Description:
 *  Unit tests for the MemoraDB logging system. These tests validate
 *  log-level filtering semantics and basic formatting expectations.
 *  
 *
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../src/utils/log.h"
#include "test_framework.h"

static void with_redirected_stdout(int *saved_fd, FILE **tmp_file) {
    *saved_fd = dup(fileno(stdout));          /* save terminal fd */
    *tmp_file = tmpfile();
    if (*tmp_file != NULL) {
        fflush(stdout);
        dup2(fileno(*tmp_file), fileno(stdout)); /* stdout -> tmpfile */
    }
}

static void restore_stdout(int saved_fd, FILE *tmp_file) {
    fflush(stdout);
    if (saved_fd != -1) {
        dup2(saved_fd, fileno(stdout));       /* restore terminal fd */
        close(saved_fd);
    }
    if (tmp_file != NULL) {
        fclose(tmp_file);
    }
}

static void read_captured_output(FILE *tmp_file, char *buffer, size_t size) {
    fflush(tmp_file);
    fseek(tmp_file, 0, SEEK_SET);
    size_t n = fread(buffer, 1, size - 1, tmp_file);
    buffer[n] = '\0';
}

void test_log_level_presence(void) {
    printf("Testing log level presence...\n");

    int saved_fd = -1;
    FILE *tmp = NULL;
    char buffer[512];

    with_redirected_stdout(&saved_fd, &tmp);

    /* With level filtering removed, all messages should appear */
    log_message(LOG_DEBUG, "debug should appear");
    log_message(LOG_INFO, "info should appear");

    memset(buffer, 0, sizeof(buffer));
    read_captured_output(tmp, buffer, sizeof(buffer));

    TEST_ASSERT(strstr(buffer, "debug should appear") != NULL,
                "DEBUG message should appear when logging is unconditional");
    TEST_ASSERT(strstr(buffer, "info should appear") != NULL,
                "INFO message should appear when logging is unconditional");

    restore_stdout(saved_fd, tmp);
    TEST_SUCCESS("Log level presence test passed");
}

void test_log_basic_formatting(void) {
    printf("Testing basic log formatting...\n");

    int saved_fd = -1;
    FILE *tmp = NULL;
    char buffer[512];

    with_redirected_stdout(&saved_fd, &tmp);

    log_message(LOG_INFO, "format check");

    memset(buffer, 0, sizeof(buffer));
    read_captured_output(tmp, buffer, sizeof(buffer));

    TEST_ASSERT(strstr(buffer, "MemoraDB: INFO") != NULL,
                "Log output should include level tag 'MemoraDB: INFO'");
    TEST_ASSERT(strstr(buffer, "format check") != NULL,
                "Log output should include the original message");

    restore_stdout(saved_fd, tmp);
    TEST_SUCCESS("Basic log formatting test passed");
}

/* ==================== Concurrency Test ==================== */

#define LOG_THREAD_COUNT 4
#define LOG_MESSAGES_PER_THREAD 200

typedef struct {
    int id;
} LogThreadArgs;

static void *log_worker(void *arg) {
    LogThreadArgs *t = (LogThreadArgs *)arg;
    for (int i = 0; i < LOG_MESSAGES_PER_THREAD; i++) {
        log_message(LOG_INFO, "thread-%d message-%d", t->id, i);
    }
    return NULL;
}

void test_log_concurrency(void) {
    printf("Testing concurrent logging...\n");

    int saved_fd = -1;
    FILE *tmp = NULL;

    with_redirected_stdout(&saved_fd, &tmp);

    pthread_t threads[LOG_THREAD_COUNT];
    LogThreadArgs args[LOG_THREAD_COUNT];

    for (int i = 0; i < LOG_THREAD_COUNT; i++) {
        args[i].id = i;
        if (pthread_create(&threads[i], NULL, log_worker, &args[i]) != 0) {
            restore_stdout(saved_fd, tmp);
            TEST_ERROR("Failed to create thread in concurrency test");
            return;
        }
    }
    for (int i = 0; i < LOG_THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    fflush(tmp);
    fseek(tmp, 0, SEEK_END);
    long len = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);

    if (len < 0) {
        restore_stdout(saved_fd, tmp);
        TEST_ERROR("Failed to get temp file length for concurrency test");
        return;
    }

    char *buffer = (char *)malloc((size_t)len + 1);
    if (buffer == NULL) {
        restore_stdout(saved_fd, tmp);
        TEST_ERROR("Failed to allocate buffer for concurrency log capture");
        return;
    }

    size_t read_bytes = fread(buffer, 1, (size_t)len, tmp);
    buffer[read_bytes] = '\0';

    restore_stdout(saved_fd, tmp);

    int occurrences = 0;
    const char *p = buffer;
    while ((p = strstr(p, "thread-")) != NULL) {
        occurrences++;
        p += strlen("thread-");
    }

    free(buffer);

    TEST_ASSERT(occurrences == LOG_THREAD_COUNT * LOG_MESSAGES_PER_THREAD,
                "All concurrent log messages should be present without loss or corruption");

    TEST_SUCCESS("Concurrent logging test passed");
}

int main(void) {
    init_test_framework();
    printf("=== Logging Tests ===\n");

    test_log_level_presence();
    test_log_basic_formatting();
    test_log_concurrency();

    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}

