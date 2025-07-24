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
        // Empty list
        list->head = list->tail = node;
    }
    
    return ++list->length;
}

size_t list_length(const List *list) {
    return list ? list->length : 0;
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
