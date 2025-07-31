/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_hashtable.c
 * Module                    : Hash Table Unit Tests
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 * 
 * Description:
 *  Unit tests for hash table operations including SET, GET, TTL functionality.
 *  Tests direct hashtable operations without server communication.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "../src/utils/hashTable.h"
#include "test_framework.h"

void test_basic_set_get() {
    printf("Testing basic SET/GET operations...\n");
    
    set_value("test_key", "test_value", 0);
    const char *result = get_value("test_key");
    
    TEST_ASSERT(result != NULL, "GET should return non-NULL value");
    TEST_ASSERT(strcmp(result, "test_value") == 0, "GET should return correct value");
    TEST_SUCCESS("Basic SET/GET test passed");
}

void test_key_expiry() {
    printf("Testing key expiry with TTL...\n");
    
    // Set key with 100ms expiry
    set_value("expiry_key", "expiry_value", 100);
    
    // Should exist immediately
    const char *result = get_value("expiry_key");
    TEST_ASSERT(result != NULL, "Key should exist immediately after SET");
    TEST_ASSERT(strcmp(result, "expiry_value") == 0, "Key should have correct value");
    
    // Wait for expiry
    usleep(110000);
    
    // Should be expired
    result = get_value("expiry_key");
    TEST_ASSERT(result == NULL, "Key should be expired after TTL");
    
    TEST_SUCCESS("Key expiry test passed");
}

void test_key_overwrite() {
    printf("Testing key overwrite...\n");
    
    set_value("overwrite_key", "original_value", 0);
    set_value("overwrite_key", "new_value", 0);
    
    const char *result = get_value("overwrite_key");
    TEST_ASSERT(result != NULL, "Overwritten key should exist");
    TEST_ASSERT(strcmp(result, "new_value") == 0, "Key should have new value");
    
    TEST_SUCCESS("Key overwrite test passed");
}

void test_nonexistent_key() {
    printf("Testing nonexistent key...\n");
    
    const char *result = get_value("nonexistent_key");
    TEST_ASSERT(result == NULL, "Nonexistent key should return NULL");
    
    TEST_SUCCESS("Nonexistent key test passed");
}

int main() {
    init_test_framework();
    printf("=== Hash Table Tests ===\n");
    
    test_basic_set_get();
    test_key_expiry();
    test_key_overwrite();
    test_nonexistent_key();
    
    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}