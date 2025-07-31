/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : tests/test_parser.c
 * Module                    : RESP Parser Unit Tests
 * Last Updating Author      : sch0penheimer
 * Last Update               : 2025/31/07
 * Version                   : 1.0.0
 *
 * Description:
 *  Unit tests for RESP protocol parser functionality.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include <assert.h>
#include <string.h>
#include "../src/parser/parser.h"
#include "test_framework.h"

void test_command_parsing() {
    printf("Testing RESP command parsing...\n");
    
    char input[] = "*2\r\n$4\r\nPING\r\n$4\r\ntest\r\n";
    char *tokens[10];
    
    int count = parse_command(input, tokens, 10);
    
    TEST_ASSERT(count == 2, "Expected 2 tokens from RESP parsing");
    TEST_ASSERT(strcmp(tokens[0], "PING") == 0, "First token should be PING");
    TEST_ASSERT(strcmp(tokens[1], "test") == 0, "Second token should be test");
    
    TEST_SUCCESS("RESP command parsing test passed");
}

void test_command_identification() {
    printf("Testing command identification...\n");
    
    TEST_ASSERT(identify_command("PING") == CMD_PING, "PING command identification failed");
    TEST_ASSERT(identify_command("ping") == CMD_PING, "ping lowercase identification failed");
    TEST_ASSERT(identify_command("SET") == CMD_SET, "SET command identification failed");
    TEST_ASSERT(identify_command("GET") == CMD_GET, "GET command identification failed");
    TEST_ASSERT(identify_command("UNKNOWN") == CMD_UNKNOWN, "Unknown command identification failed");
    
    TEST_SUCCESS("Command identification test passed");
}

void test_invalid_resp_format() {
    printf("Testing invalid RESP format...\n");
    
    char invalid_input[] = "invalid_format";
    char *tokens[10];
    
    int count = parse_command(invalid_input, tokens, 10);
    TEST_ASSERT(count == -1, "Invalid RESP format should return -1");
    
    TEST_SUCCESS("Invalid RESP format test passed");
}

int main() {
    init_test_framework();
    printf("=== RESP Parser Tests ===\n");
    
    test_command_parsing();
    test_command_identification();
    test_invalid_resp_format();
    
    save_test_results();
    return total_tests_failed > 0 ? 1 : 0;
}