/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.c
 * Module                    : Hash Table
 * Last Updating Author      : sch0penheimer
 * Last Update               : 07/22/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  Implementation of simple hash table for MemoraDB key-value storage.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
    * Hash Table Implementation
    * 
    * This hash table uses separate chaining for collision resolution.
    * Each entry contains a key-value pair and a pointer to the next entry.
    */
unsigned int hash(const char *key) {
    unsigned int h = 0;
    while (*key) {
        h = (h << 5) + *key++;
    }
    return h % TABLE_SIZE;
}

/*
    * Entry structure for the hash table
    * Contains a key, value, and a pointer to the next entry
    */

pthread_mutex_t hashtable_mutex = PTHREAD_MUTEX_INITIALIZER;

Entry *HASHTABLE[TABLE_SIZE] = {0};

void set_value(const char *key, const char *value) {
    pthread_mutex_lock(&hashtable_mutex);
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = strdup(value);
            pthread_mutex_unlock(&hashtable_mutex);
            return;
        }
        entry = entry->next;
    }
    //-- new entry if not found --//
    entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    entry->next = HASHTABLE[idx];
    HASHTABLE[idx] = entry;
    pthread_mutex_unlock(&hashtable_mutex);
}

/*
    * Get value by key from the hash table
    * Returns NULL if key does not exist
    */
const char *get_value(const char *key) {
    pthread_mutex_lock(&hashtable_mutex);
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            const char *result = entry->value;
            pthread_mutex_unlock(&hashtable_mutex);
            return result;
        }
        entry = entry->next;
    }
    pthread_mutex_unlock(&hashtable_mutex);
    return NULL;
}