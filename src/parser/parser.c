/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/parser/parser.c
 * Module                    : RESP Protocol Parser
 * Last Updating Author      : Haitam Bidiouane
 * Last Update               : 07/24/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  RESP (Redis Serialization Protocol) parser implementation for MemoraDB.
 *  Handles parsing of client commands and command identification.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "parser.h"
#include "../utils/hashTable.h"

int parse_command(char * input, char * tokens[], int max_tokens){
    int counter = 0;
    char * cur = input;

    if(*cur != '*') return -1;

    int num_args = atoi(cur+1);
    cur = strstr(cur, "\r\n");
    if(!cur) return -1;

    cur += 2;

    for(int i = 0; i < num_args && counter < max_tokens; i++){
        if(*cur != '$') return -1;

        int len = atoi (cur+1);
        cur = strstr(cur, "\r\n");
        if (!cur) return -1;
        cur += 2;
        tokens[counter] = cur;
        tokens[counter][len] = '\0';
        cur += len + 2;
        counter++;
    }
    return counter;
}

enum command_t identify_command(const char * cmd){
    if(strcasecmp(cmd, "PING") == 0) return CMD_PING;
    if(strcasecmp(cmd, "ECHO") == 0) return CMD_ECHO;
    if(strcasecmp(cmd, "SET") == 0) return CMD_SET;
    if(strcasecmp(cmd, "GET") == 0) return CMD_GET;
    return CMD_UNKNOWN;
}

void dispatch_command(int client_fd, char * tokens[], int token_count){
    if(token_count == 0){
        dprintf(client_fd, "[MemoraDB: ERROR] Empty Command\n");
        return;
    }

    enum command_t cmd = identify_command(tokens[0]);

    switch (cmd)
    {
    case CMD_PING:
        dprintf(client_fd, "PONG\r\n");
        break;
    case CMD_ECHO:
        if(token_count < 2){
            dprintf(client_fd, "[MemoraDB: WARN] ECHO needs one argument\n");
        } else {
            dprintf(client_fd, "%s\r\n", tokens[1]);
        }
        break;
    case CMD_SET:
        if (token_count < 3) {
            dprintf(client_fd, "[MemoraDB: WARN] SET needs key and value\r\n");
        } else {
            long long px = 0;
            if (token_count >= 5 && strcasecmp(tokens[3], "PX") == 0) {
                px = atoll(tokens[4]);
            }
            set_value(tokens[1], tokens[2], px);
            dprintf(client_fd, "OK\r\n");
        }
        break;
    case CMD_GET:
        if(token_count < 2){
            dprintf(client_fd, "[MemoraDB: WARN] GET needs key\r\n");
        } else {
            const char *value = get_value(tokens[1]);
            if(value)
                dprintf(client_fd, "%s\r\n", value);
            else
                dprintf(client_fd, "nil\r\n");
        }
        break;
    default:
        dprintf(client_fd, "[MemoraDB: WARN] Unknown command '%s'\n", tokens[0]);
        break;
    }
}
