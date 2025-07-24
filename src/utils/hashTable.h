/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.h
 * Module                    : Hash Table
 * Last Updating Author      : Haitam Bidiouane
 * Last Update               : 07/24/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Header for simple hash table for MemoraDB key-value storage.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "list.h"

/* ==================== HASHTABLE SIZE ==================== */
#define TABLE_SIZE 1024

/* ==================== Value Types ==================== */
typedef enum {
    VALUE_STRING,
    VALUE_LIST
} value_type_t;

/* ==================== Key-Value Struct ==================== */
typedef struct Entry {
    char *key;
    value_type_t type;
    union {
        char *string_value;
        List *list_value;
    } data;
    long long expiry; // 0 = no expiry, != 0 = expiry time in ms 
    struct Entry *next;
} Entry;

/* ============================================================ */
/* ==================== The Main HashTable ==================== */
/* ============================================================ */

extern Entry *HASHTABLE[TABLE_SIZE];

/**
 * @brief Hash function to compute the index for a given key.
 * 
 * This function uses a simple hash algorithm to convert a string key
 * into an unsigned integer index suitable for use in the hash table.
 * 
 * @param key The key to hash.
 * @return The computed hash index.
 */
unsigned int hash(const char *key);

/**
 * @brief Set a string value in the hash table.
 * 
 * @param key The key to set.
 * @param value The string value to associate with the key.
 * @param px Expiry time in milliseconds (0 for no expiry).
 */
void set_value(const char *key, const char *value, long long px);

/**
 * @brief Get a string value from the hash table.
 * 
 * @param key The key to retrieve.
 * @return The string value, or NULL if not found or expired.
 */
const char *get_value(const char *key);

/**
 * @brief Push values to the right of a list stored at key.
 * 
 * @param key The key of the list.
 * @param values Array of values to push.
 * @param count Number of values to push.
 * @return The new length of the list, or -1 if key holds non-list value.
 */
int rpush_list(const char *key, const char *values[], int count);

#endif // HASHTABLE_H
