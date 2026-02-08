/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/parser/parser.c
 * Module                    : RESP Protocol Parser
 * Last Updating Author      : youssefbouraoui1
 * Last Update               : 02/08/2026
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

#define _GNU_SOURCE
#include "parser.h"
#include "../utils/hashTable.h"
#include <stdio.h>
#include <stdbool.h>

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
    if(strcasecmp(cmd, "DEL") == 0) return CMD_DEL;
    if(strcasecmp(cmd, "RPUSH") == 0) return CMD_RPUSH;
    if(strcasecmp(cmd, "LRANGE") == 0) return CMD_LRANGE;
    if(strcasecmp(cmd, "LPUSH") == 0) return CMD_LPUSH;
    if(strcasecmp(cmd,"LLEN") == 0) return CMD_LLEN;
    if(strcasecmp(cmd, "LPOP") == 0) return CMD_LPOP;
    if(strcasecmp(cmd, "BLPOP") == 0) return CMD_BLPOP;
    if(strcasecmp(cmd,"TYPE")==0) return CMD_TYPE;
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
        dprintf(client_fd, "+PONG\r\n");
        break;
    case CMD_ECHO:
        if(token_count < 2){
            dprintf(client_fd, "[MemoraDB: WARN] ECHO needs one argument\n");
        } else {
            dprintf(client_fd, "$%lu\r\n%s\r\n", strlen(tokens[1]), tokens[1]);
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
            dprintf(client_fd, "+OK\r\n");
        }
        break;
    case CMD_GET:
        if(token_count < 2){
            dprintf(client_fd, "[MemoraDB: WARN] GET needs key\r\n");
        } else {
            const char *value = get_value(tokens[1]);
            if(value)
                dprintf(client_fd, "$%lu\r\n%s\r\n", strlen(value), value);
            else
                dprintf(client_fd, "$-1\r\n");
        }
        break;
    case CMD_RPUSH:
        if (token_count < 3) {
            dprintf(client_fd, "[MemoraDB: WARN] RPUSH needs key and at least one value\r\n");
        } else {
            List *list = get_or_create_list(tokens[1]);
            if (!list) {
                dprintf(client_fd, "[MemoraDB: ERROR] could not create list\r\n");
                break;
            }

            size_t total_elements = 0;
            for (int i = 2; i < token_count; i++) {
                size_t new_len = list_rpush(list, tokens[i]);
                if (new_len > total_elements) {
                    total_elements = new_len;
                }
            }

            dprintf(client_fd, ":%zu\r\n", total_elements);
        }
        break;
    case CMD_LPUSH:
        if (token_count < 3) {
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'LPUSH'\r\n");
        } else {
            List *list = get_or_create_list(tokens[1]);
            if (!list) {
                dprintf(client_fd, "[MemoraDB: ERROR] could not create list\r\n");
                break;
            }

            size_t total_elements = 0;
            for (int i = 2 ; i < token_count ; i++) {
                size_t new_len = list_lpush(list, tokens[i]);
                if (new_len > total_elements) {
                    total_elements = new_len;
                }
            }

            dprintf(client_fd, ":%zu\r\n", total_elements);
        }
        break;
    case CMD_LRANGE:
        if (token_count < 4) {
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'LRANGE'\r\n");
        } else {
            int start = atoi(tokens[2]);
            int end = atoi(tokens[3]);
            
            List *list = get_list_if_exists(tokens[1]);
            int result_count = 0;
            char **elements = NULL;
            if (list) {
                elements = list_range(list, start, end, &result_count);
            }
            
            if (elements) {
                dprintf(client_fd, "*%d\r\n", result_count);
                for (int i = 0; i < result_count; i++) {
                    dprintf(client_fd, "$%lu\r\n%s\r\n", strlen(elements[i]), elements[i]);
                    free(elements[i]);
                }
                free(elements);
            } else {
                dprintf(client_fd, "*0\r\n");
            }
        }
        break;
    case CMD_LLEN:
        if (token_count < 2) {
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'LLEN'\r\n");
        } else {
            List *list = get_list_if_exists(tokens[1]);
            int length = 0;
            if (list) {
                length = list_length(list);
            }
            dprintf(client_fd, ":%d\r\n", length);
        }
        break;
    case CMD_LPOP:
        if (token_count == 2) {
            List *list = get_list_if_exists(tokens[1]);
            char *popped = lpop_element(list);
            if (popped) {
                dprintf(client_fd, "$%lu\r\n%s\r\n", strlen(popped), popped);
                free(popped);
            } else {
                dprintf(client_fd, "$-1\r\n");
            }
        } else if (token_count == 3) {
            List *list = get_list_if_exists(tokens[1]);
            int count = atoi(tokens[2]);
            if (count <= 0) {
                dprintf(client_fd, "*0\r\n");
            } else {
                int actual_count = 0;
                char **popped_elements = lpop_multiple(list, count, &actual_count);

                dprintf(client_fd, "*%d\r\n", actual_count);
                for (int i = 0; i < actual_count; i++) {
                    dprintf(client_fd, "$%lu\r\n%s\r\n", strlen(popped_elements[i]), popped_elements[i]);
                    free(popped_elements[i]);
                }
                free(popped_elements);
            }
        } else {
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'LPOP'\r\n");
        }
        break;
    case CMD_BLPOP: {
        if (token_count != 3) {
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'BLPOP'\r\n");
            break;
        }

        const char *list_name = tokens[1];
        double timeout_sec = atof(tokens[2]);
        long long start_time = current_millis();
        long long timeout_ms = (long long)(timeout_sec * 1000);

        List *list = get_list_if_exists(list_name);
        char *element = NULL;

        while (1) {
            element = lpop_element(list);
            if (element != NULL) {
                dprintf(client_fd, "*2\r\n$%lu\r\n%s\r\n$%lu\r\n%s\r\n",
                        strlen(list_name), list_name,
                        strlen(element), element);
                free(element);
                break;
            }

            long long elapsed = current_millis() - start_time;

            if (timeout_sec == 0.0 || elapsed < timeout_ms) {
                usleep(100 * 1000);
                continue;
            }

            dprintf(client_fd, "$-1\r\n");
            break;
        }
        break;
    }
    case CMD_DEL:
        if (token_count < 2) {
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'DEL'\r\n");
        } else {
            int deleted_count = 0;
            /* delete each key provided */
            for (int i = 1; i < token_count; i++) {
                if (delete_key(tokens[i])) {
                    deleted_count++;
                }
            }
            dprintf(client_fd, ":%d\r\n", deleted_count);
        }
        break;
    case CMD_TYPE:
        if (token_count<2){
            dprintf(client_fd, "[MemoraDB: ERROR] wrong number of arguments for 'TYPE', the 'TYPE' command expects a key\r\n");
        }else{
            const char *type = get_type(tokens[1]); 
            dprintf(client_fd, "+%s\r\n", type); 
        }
        break;
    default:
        dprintf(client_fd, "[MemoraDB: WARN] Unknown command '%s'\n", tokens[0]);
        break;
    }
}
