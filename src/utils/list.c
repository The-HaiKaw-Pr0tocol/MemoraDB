/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/list.c
 * Module                    : Linked List
 * Last Updating Author      : Haitam Bidiouane
 * Last Update               : 07/24/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Implementation of linked list for MemoraDB list operations.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "list.h"
#include <string.h>

List *list_create(void) {
    List *list = malloc(sizeof(List));
    if (!list) return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    
    return list;
}

size_t list_rpush(List *list, const char *value) {
    if (!list || !value) return 0;
    
    ListNode *node = malloc(sizeof(ListNode));
    if (!node) return list->length;
    
    node->value = strdup(value);
    if (!node->value) {
        free(node);
        return list->length;
    }
    
    node->next = NULL;
    
    if (list->tail) {
        list->tail->next = node;
        list->tail = node;
    } else {
        //-- Empty list --//
        list->head = list->tail = node;
    }
    
    return ++list->length;
}

size_t list_lpush(List *list, const char *value) {
    if (!list || !value) return 0;
    
    ListNode *node = malloc(sizeof(ListNode));
    if (!node) return list->length;
    
    node->value = strdup(value);
    if (!node->value) {
        free(node);
        return list->length;
    }
    
    node->next = NULL;
    
    if (list->head) {
        node->next = list->head;
        list->head = node;
    } else {
        //-- Empty list --//
        list->head = list->tail = node;
    }
    
    return ++list->length;
}

size_t list_length(const List *list) {
    return list ? list->length : 0;
}

char **list_range(List *list, int start, int end, int *count) {
    if (!list || !count) {
        *count = 0;
        return NULL;
    }

    int len = list->length;
    *count = 0;

    if (start < 0) start += len;
    if (end < 0) end += len;

    if (start < 0) start = 0;
    if (end >= len) end = len - 1;
    if (start > end || start >= len) {
        return NULL;
    }

    *count = end - start + 1;
    char **result = malloc(sizeof(char*) * (*count));
    if (!result) {
        *count = 0;
        return NULL;
    }

    ListNode *current = list->head;
    for (int i = 0; i < start; i++) {
        current = current->next;
    }

    for (int i = 0; i < *count; i++) {
        result[i] = strdup(current->value);
        if (!result[i]) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            *count = 0;
            return NULL;
        }
        current = current->next;
    }

    return result;
}

void list_free(List *list) {
    if (!list) return;
    
    ListNode *current = list->head;
    while (current) {
        ListNode *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    
    free(list);
}
