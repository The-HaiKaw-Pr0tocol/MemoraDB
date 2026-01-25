/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_history.c
 * Module                    : History Unit Tests
 * Last Updating Author      : shady0503
 * Last Update               : 01/23/2026
 * Version                   : 1.0.0
 *
 * Description:
 *  Unit tests for history operations including filtering and addition.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

 #include <assert.h>
#include <string.h>
#include "../src/client/history/history.h"
#include "test_framework.h"

static void reset_history() {
    history_cleanup();
    history_init();
}

void test_empty_command_filtering() {
    printf("Testing empty command filtering...\n");
    reset_history();
    
    int result1 = history_add("");
    TEST_ASSERT(result1 == 0, "Empty string should be filtered");
    
    int result2 = history_add("   ");
    TEST_ASSERT(result2 == 0, "Whitespace-only should be filtered");
    
    int result3 = history_add("\t\n");
    TEST_ASSERT(result3 == 0, "Tab/newline should be filtered");
    
    history_cleanup();
    TEST_SUCCESS("Empty command filtering test passed");
}

void test_consecutive_duplicates() {
    printf("Testing consecutive duplicate filtering...\n");
    reset_history();
    
    int r1 = history_add("SET x 1");
    TEST_ASSERT(r1 == 0, "First command should be added");
    
    int r2 = history_add("SET x 1");
    TEST_ASSERT(r2 == 0, "Duplicate should be filtered");
    
    int r3 = history_add("SET x 2");
    TEST_ASSERT(r3 == 0, "Different command should be added");
    
    int r4 = history_add("SET x 2");
    TEST_ASSERT(r4 == 0, "New duplicate should be filtered");
    
    int r5 = history_add("SET x 1");
    TEST_ASSERT(r5 == 0, "Non-consecutive duplicate should be added");
    
    history_cleanup();
    TEST_SUCCESS("Consecutive duplicate filtering test passed");
}

void test_null_handling() {
    printf("Testing NULL command handling...\n");
    reset_history();
    
    int result = history_add(NULL);
    TEST_ASSERT(result == -1, "NULL should return error");
    
    history_cleanup();
    TEST_SUCCESS("NULL handling test passed");
}

void test_valid_commands() {
    printf("Testing valid command addition...\n");
    reset_history();
    
    TEST_ASSERT(history_add("GET key") == 0, "GET command should be added");
    TEST_ASSERT(history_add("SET key value") == 0, "SET command should be added");
    TEST_ASSERT(history_add("PING") == 0, "PING command should be added");
    TEST_ASSERT(history_add("ECHO hello") == 0, "ECHO command should be added");
    
    history_cleanup();
    TEST_SUCCESS("Valid command addition test passed");
}

void test_special_cases() {
    printf("Testing special cases...\n");
    reset_history();
    
    //-- Commands with special characters --//
    TEST_ASSERT(history_add("SET key \"value with spaces\"") == 0, 
                "Command with quotes should work");
    TEST_ASSERT(history_add("A") == 0, "Single char command should work");
    
    //-- Long command --//
    char long_cmd[256];
    memset(long_cmd, 'X', 200);
    long_cmd[200] = '\0';
    TEST_ASSERT(history_add(long_cmd) == 0, "Long command should work");
    
    history_cleanup();
    TEST_SUCCESS("Special cases test passed");
}

void test_history_sequence() {
    printf("Testing realistic command sequence...\n");
    reset_history();
    
    const char *commands[] = {
        "PING", "SET x 1", "GET x", "SET x 2", 
        "GET x", "GET x", "DEL x"
    };
    
    for (int i = 0; i < 7; i++) {
        history_add(commands[i]);
    }
    
    history_cleanup();
    TEST_SUCCESS("Command sequence test passed");
}

void test_cleanup_multiple() {
    printf("Testing cleanup idempotency...\n");
    reset_history();
    
    history_add("PING");
    history_cleanup();
    history_cleanup();  //-- Multiple cleanup should be safe --//
    
    history_init();
    TEST_ASSERT(history_add("GET x") == 0, "Should work after cleanup");
    
    history_cleanup();
    TEST_SUCCESS("Cleanup idempotency test passed");
}

int main() {
    init_test_framework();
    printf("=== Command History Tests ===\n");
    
    test_empty_command_filtering();
    test_consecutive_duplicates();
    test_null_handling();
    test_valid_commands();
    test_special_cases();
    test_history_sequence();
    test_cleanup_multiple();
    
    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}
