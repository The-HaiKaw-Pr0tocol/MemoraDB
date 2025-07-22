/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.c
 * Module                    : Hash Table
 * Last Updating Author      : Kei077
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
#include <sys/time.h>

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

pthread_mutex_t hashtable_mutex = PTHREAD_MUTEX_INITIALIZER;

Entry *HASHTABLE[TABLE_SIZE] = {0};

long long current_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}

void set_value(const char *key, const char *value, long long px) {
    pthread_mutex_lock(&hashtable_mutex);
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];
    long long expiry = (px > 0) ? current_millis() + px : 0;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = strdup(value);
            entry->expiry = expiry;
            pthread_mutex_unlock(&hashtable_mutex);
            return;
        }
        entry = entry->next;
    }

    // New entry
    entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    entry->expiry = expiry;
    entry->next = HASHTABLE[idx];
    HASHTABLE[idx] = entry;
    pthread_mutex_unlock(&hashtable_mutex);
}

const char *get_value(const char *key) {
    pthread_mutex_lock(&hashtable_mutex);
    unsigned int idx = hash(key);
    Entry *prev = NULL;
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                // Key expired, remove from list
                if (prev)
                    prev->next = entry->next;
                else
                    HASHTABLE[idx] = entry->next;

                free(entry->key);
                free(entry->value);
                free(entry);
                pthread_mutex_unlock(&hashtable_mutex);
                return NULL;
            } else {
                const char *result = entry->value;
                pthread_mutex_unlock(&hashtable_mutex);
                return result;
            }
        }
        prev = entry;
        entry = entry->next;
    }

    pthread_mutex_unlock(&hashtable_mutex);
    return NULL;
}
