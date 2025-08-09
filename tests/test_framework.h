/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_framework.h
 * Module                    : Test Framework Header
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 * 
 * Description:
 *  Common test framework utilities and macros for all unit / integration tests.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

//-- ANSI Color Codes --//
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BOLD    "\x1b[1m"
#define COLOR_RESET   "\x1b[0m"

//-- Global test counters --//
extern int total_tests_run;
extern int total_tests_passed;
extern int total_tests_failed;

#define TEST_ASSERT(condition, message) \
    do { \
        total_tests_run++; \
        if (!(condition)) { \
            total_tests_failed++; \
            fprintf(stderr, COLOR_RED "[MemoraDB-TEST: FAIL] %s at %s:%d" COLOR_RESET "\n", message, __FILE__, __LINE__); \
        } else { \
            total_tests_passed++; \
        } \
    } while(0)

#define TEST_SUCCESS(message) \
    printf(COLOR_GREEN "[MemoraDB-TEST: SUCCESS] %s" COLOR_RESET "\n", message)

#define TEST_ERROR(message) \
    do { \
        printf(COLOR_RED "[MemoraDB-TEST: FAIL] %s" COLOR_RESET "\n", message); \
        total_tests_failed++; \
    } while(0)

#define TEST_WARNING(message) \
    printf(COLOR_YELLOW "[MemoraDB-TEST: WARNING] %s" COLOR_RESET "\n", message);

//-- Test suite management --//
void init_test_framework(void);
void print_test_summary(void);
void print_final_summary(void);
void save_test_results(void);
void load_all_test_results(void);

#endif // TEST_FRAMEWORK_H