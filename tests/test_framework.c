/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_framework.c
 * Module                    : Test Framework Implementation
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 * 
 * Description:
 *  Implementation of Maven-style test framework for MemoraDB.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "test_framework.h"
#include <time.h>

// Global test counters
int total_tests_run = 0;
int total_tests_passed = 0;
int total_tests_failed = 0;
static clock_t start_time;

void init_test_framework(void) {
    total_tests_run = 0;
    total_tests_passed = 0;
    total_tests_failed = 0;
    start_time = clock();
}

void save_test_results(void) {
    FILE *f = fopen("/tmp/memoradb_test_results.txt", "a");
    if (f) {
        fprintf(f, "%d %d %d\n", total_tests_run, total_tests_passed, total_tests_failed);
        fclose(f);
    }
}

void load_all_test_results(void) {
    total_tests_run = 0;
    total_tests_passed = 0;
    total_tests_failed = 0;
    
    FILE *f = fopen("/tmp/memoradb_test_results.txt", "r");
    if (f) {
        int run, passed, failed;
        while (fscanf(f, "%d %d %d", &run, &passed, &failed) == 3) {
            total_tests_run += run;
            total_tests_passed += passed;
            total_tests_failed += failed;
        }
        fclose(f);
    }
}

void print_test_summary(void) {
    //-- Called on failure - just exit without summary --//
    printf(COLOR_RED "\nTest suite stopped due to failure\n" COLOR_RESET);
}

void print_final_summary(void) {
    load_all_test_results();
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n");
    
    //-- Use green background if all pass, red if any fail --//
    if (total_tests_failed == 0) {
        printf(COLOR_GREEN COLOR_BOLD);
    } else {
        printf(COLOR_RED COLOR_BOLD);
    }
    
    printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           TEST EXECUTION SUMMARY                         ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf(COLOR_RESET);
    
    if (total_tests_failed == 0) {
        printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
        printf("║ " COLOR_GREEN COLOR_BOLD "Status:         SUCCESS" COLOR_RESET "                                                ║\n");
    } else {
        printf("║ " COLOR_RED COLOR_BOLD "Status:           FAILURE" COLOR_RESET "                                                ║\n");
    }
    printf("║ Total Tests:      %-3d                                                    ║\n", total_tests_run);
    printf("║ " COLOR_GREEN "Passed:           %-3d" COLOR_RESET "                                                    ║\n", total_tests_passed);
    printf("║ " COLOR_RED   "Failed:           %-3d" COLOR_RESET "                                                    ║\n", total_tests_failed);
    printf("║ Execution Time:   %.3f seconds                                          ║\n", execution_time);
    
    if (total_tests_failed == 0) {
        printf(COLOR_GREEN COLOR_BOLD);
        printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
        printf("║                            ALL TESTS PASSED                              ║\n");
    } else {
        printf(COLOR_RED COLOR_BOLD);
        printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
        printf("║                             TESTS FAILED                                 ║\n");
        printf("║                  Please fix issues before deployment                     ║\n");
    }
    
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET "\n");
}
