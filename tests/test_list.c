/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_list.c
 * Module                    : List Operations Unit Tests
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 *
 * Description:
 *  Unit tests for list operations including RPUSH, LPUSH, LRANGE, LLEN.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include <assert.h>
#include <string.h>
#include "../src/utils/list.h"
#include "test_framework.h"

void test_list_creation() {
    printf("Testing list creation...\n");
    
    List *list = list_create();
    TEST_ASSERT(list != NULL, "List creation should succeed");
    TEST_ASSERT(list->head == NULL, "New list head should be NULL");
    TEST_ASSERT(list->tail == NULL, "New list tail should be NULL");
    TEST_ASSERT(list->length == 0, "New list length should be 0");
    
    list_free(list);
    TEST_SUCCESS("List creation test passed");
}

void test_rpush_operations() {
    printf("Testing RPUSH operations...\n");
    
    List *list = list_create();
    
    size_t len1 = list_rpush(list, "first");
    TEST_ASSERT(len1 == 1, "First RPUSH should return length 1");
    TEST_ASSERT(list->length == 1, "List length should be 1");
    
    size_t len2 = list_rpush(list, "second");
    TEST_ASSERT(len2 == 2, "Second RPUSH should return length 2");
    TEST_ASSERT(list->length == 2, "List length should be 2");
    
    list_free(list);
    TEST_SUCCESS("RPUSH operations test passed");
}

void test_lpush_operations() {
    printf("Testing LPUSH operations...\n");
    
    List *list = list_create();
    
    size_t len1 = list_lpush(list, "first");
    TEST_ASSERT(len1 == 1, "First LPUSH should return length 1");
    
    size_t len2 = list_lpush(list, "second");
    TEST_ASSERT(len2 == 2, "Second LPUSH should return length 2");
    
    list_free(list);
    TEST_SUCCESS("LPUSH operations test passed");
}

void test_lrange_operations() {
    printf("Testing LRANGE operations...\n");
    
    List *list = list_create();
    list_rpush(list, "item1");
    list_rpush(list, "item2");
    list_rpush(list, "item3");
    
    int count;
    char **result = list_range(list, 0, -1, &count);
    
    TEST_ASSERT(result != NULL, "LRANGE should return non-NULL result");
    TEST_ASSERT(count == 3, "LRANGE should return 3 items");
    TEST_ASSERT(strcmp(result[0], "item1") == 0, "First item should be item1");
    TEST_ASSERT(strcmp(result[1], "item2") == 0, "Second item should be item2");
    TEST_ASSERT(strcmp(result[2], "item3") == 0, "Third item should be item3");
    
    //-- Free result --//
    for (int i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
    list_free(list);
    
    TEST_SUCCESS("LRANGE operations test passed");
}

int main() {
    init_test_framework();
    printf("=== List Operations Tests ===\n");
    
    test_list_creation();
    test_rpush_operations();
    test_lpush_operations();
    test_lrange_operations();
    
    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}