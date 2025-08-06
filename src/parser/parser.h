/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/parser/parser.h
 * Module                    : RESP Protocol Parser
 * Last Updating Author      : kei077
 * Last Update               : 08/06/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  RESP (Redis Serialization Protocol) parser for MemoraDB.
 *  Handles parsing of client commands and command identification.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

/**
 * Command types supported by MemoraDB
 */
enum command_t {
    CMD_PING,
    CMD_ECHO,
    CMD_SET,
    CMD_GET,
    CMD_RPUSH,
    CMD_LPUSH,
    CMD_LRANGE,
    CMD_LLEN,
    CMD_LPOP,
    CMD_BLPOP,
    CMD_UNKNOWN
};

/**
 * Parse RESP protocol command from input buffer
 * 
 * @param input Input buffer containing RESP formatted command
 * @param tokens Array to store parsed tokens
 * @param max_tokens Maximum number of tokens to parse
 * @return Number of tokens parsed, or -1 on error
 */
int parse_command(char *input, char *tokens[], int max_tokens);

/**
 * Identify command type from command string
 * 
 * @param cmd Command string to identify
 * @return Command type enumeration
 */
enum command_t identify_command(const char *cmd);

/**
 * Dispatch and execute command based on tokens
 * @param client_fd Client socket file descriptor
 * @param tokens Array of parsed command tokens
 * @param token_count Number of tokens in array
 */
void dispatch_command(int client_fd, char *tokens[], int token_count);

#endif // PARSER_H
