/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/list.h
 * Module                    : Linked List
 * Last Updating Author      : Haitam Bidiouane
 * Last Update               : 07/24/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Header for linked list implementation for MemoraDB list operations.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/* ==================== List Node Structure ==================== */
typedef struct ListNode {
    char *value;
    struct ListNode *next;
} ListNode;

/* ==================== List Structure ==================== */
typedef struct List {
    ListNode *head;
    ListNode *tail;
    size_t length;
} List;

/**
 * @brief Create a new empty list.
 * 
 * @return Pointer to the newly created list.
 */
List *list_create(void);

/**
 * @brief Push a value to the right (tail) of the list.
 * 
 * @param list The list to push to.
 * @param value The value to push.
 * @return The new length of the list.
 */
size_t list_rpush(List *list, const char *value);

/**
 * @brief Get the length of the list.
 * 
 * @param list The list to get the length of.
 * @return The length of the list.
 */
size_t list_length(const List *list);

/**
 * @brief Free all memory associated with the list.
 * 
 * @param list The list to free.
 */
void list_free(List *list);

/**
 * Get a range of elements from the list
 * @param list The list to get elements from
 * @param start Starting index (can be negative)
 * @param end Ending index (can be negative)
 * @param count Pointer to store the number of elements returned
 * @return Array of strings that must be freed by caller, NULL on error
 */
char **list_range(List *list, int start, int end, int *count);

#endif // LIST_H
