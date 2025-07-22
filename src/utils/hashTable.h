/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.h
 * Module                    : Hash Table
 * Last Updating Author      : sch0penheimer
 * Last Update               : 07/22/2025
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

/* ==================== HASHTABLE SIZE ==================== */
#define TABLE_SIZE 1024

/* ==================== Key-Value Struct ==================== */
typedef struct Entry {
    char *key;
    char *value;
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
 * @brief Initialize the hash table.
 * 
 * This function sets up the hash table by allocating memory for each entry
 * and initializing them to NULL.
 */
void set_value(const char *key, const char *value);

/**
 * @brief Set a key-value pair in the hash table.
 * 
 * This function inserts or updates a key-value pair in the hash table.
 * If the key already exists, it updates the value; otherwise, it creates
 * a new entry.
 * 
 * @param key The key to set.
 * @param value The value to associate with the key.
 */
const char *get_value(const char *key);

#endif // HASHTABLE_H
