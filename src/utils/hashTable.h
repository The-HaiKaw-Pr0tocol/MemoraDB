/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.h
 * Module                    : Hash Table
 * Last Updating Author      : youssefbouraoui1
 * Last Update               : 02/08/2026
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
    long long expiry; //- 0 = no expiry, != 0 = expiry time in ms -//
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
 * Get an existing list or create a new one
 * @param key The key to lookup or create
 * @return Pointer to the list, NULL on error
 */
List *get_or_create_list(const char *key);

/**
 * Get the list at key if it exists and is a list.
 * @param key The key to lookup
 * @return Pointer to the list, or NULL if not found or not a list
 */
List *get_list_if_exists(const char *key);

/**
 * Delete a key from the hash table, removing both string and list types.
 * Properly frees memory for both string values and list structures.
 * @param key The key to delete
 * @return 1 if the key was deleted, 0 if not found
 */
int delete_key(const char *key);

/**
 * Get current time in milliseconds since epoch
 * @return Current time in milliseconds
 */
long long current_millis(void);

/**
 * @brief Get the type of the value at key.
 * 
 * @param key The key to lookup.
 * @return "string", "list", or "none" if not found.
 */
const char *get_type(const char *key);

#endif // HASHTABLE_H
