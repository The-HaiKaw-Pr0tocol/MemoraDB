/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.c
 * Module                    : Hash Table
 * Last Updating Author      : m0hamed541
 * Last Update               : 03/08/2026
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

pthread_mutex_t bucket_mutex[TABLE_SIZE];  // per bucket lock

void hashtable_lock_init(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        pthread_mutex_init(&bucket_mutex[i], NULL);
    }
}

Entry *HASHTABLE[TABLE_SIZE] = {0};

long long current_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}

void set_value(const char *key, const char *value, long long px) {
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];
    pthread_mutex_lock(&bucket_mutex[idx]);
    long long expiry = (px > 0) ? current_millis() + px : 0;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            //-- Free old value based on type --//
            if (entry->type == VALUE_STRING) {
                free(entry->data.string_value);
            } else if (entry->type == VALUE_LIST) {
                list_free(entry->data.list_value);
            }
            
            entry->type = VALUE_STRING;
            entry->data.string_value = strdup(value);
            entry->expiry = expiry;
            pthread_mutex_unlock(&bucket_mutex[idx]);
            return;
        }
        entry = entry->next;
    }

    //-- New entry --//
    entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->type = VALUE_STRING;
    entry->data.string_value = strdup(value);
    entry->expiry = expiry;
    entry->next = HASHTABLE[idx];
    HASHTABLE[idx] = entry;
    pthread_mutex_unlock(&bucket_mutex[idx]);
}

const char *get_value(const char *key) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&bucket_mutex[idx]);
    Entry *prev = NULL;
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                if (prev)
                    prev->next = entry->next;
                else
                    HASHTABLE[idx] = entry->next;

                free(entry->key);
                if (entry->type == VALUE_STRING) {
                    free(entry->data.string_value);
                } else if (entry->type == VALUE_LIST) {
                    list_free(entry->data.list_value);
                }
                free(entry);
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return NULL;
            } else {
                if (entry->type == VALUE_STRING) {
                    const char *result = entry->data.string_value;
                    pthread_mutex_unlock(&bucket_mutex[idx]);
                    return result;
                }
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return NULL;
            }
        }
        prev = entry;
        entry = entry->next;
    }

    pthread_mutex_unlock(&bucket_mutex[idx]);
    return NULL;
}

List *get_or_create_list(const char *key) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&bucket_mutex[idx]);
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return NULL;
            }
            if (entry->type == VALUE_LIST) {
                List *list = entry->data.list_value;
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return list;
            } else {
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return NULL;
            }
        }
        entry = entry->next;
    }

    //-- Not found, create new list entry --//
    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry) {
        pthread_mutex_unlock(&bucket_mutex[idx]);
        return NULL;
    }
    new_entry->key = strdup(key);
    new_entry->type = VALUE_LIST;
    new_entry->data.list_value = list_create();
    new_entry->expiry = 0;
    new_entry->next = HASHTABLE[idx];
    HASHTABLE[idx] = new_entry;

    List *list = new_entry->data.list_value;
    pthread_mutex_unlock(&bucket_mutex[idx]);
    return list;
}

List *get_list_if_exists(const char *key) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&bucket_mutex[idx]);
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return NULL;
            }
            if (entry->type == VALUE_LIST) {
                List *list = entry->data.list_value;
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return list;
            } else {
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return NULL;
            }
        }
        entry = entry->next;
    }
    pthread_mutex_unlock(&bucket_mutex[idx]);
    return NULL;
}

/**
 * Delete a key from the hash table, handling both string and list types.
 * Removes the entry from the linked list and frees all associated memory.
 */
int delete_key(const char *key) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&bucket_mutex[idx]);
    Entry *prev = NULL;
    Entry *entry = HASHTABLE[idx];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev)
                prev->next = entry->next;
            else
                HASHTABLE[idx] = entry->next;

            free(entry->key);
            if (entry->type == VALUE_STRING) {
                free(entry->data.string_value);
            } else if (entry->type == VALUE_LIST) {
                list_free(entry->data.list_value);
            }
            free(entry);
            
            pthread_mutex_unlock(&bucket_mutex[idx]);
            return 1;
        }
        prev = entry;
        entry = entry->next;
    }
    
    pthread_mutex_unlock(&bucket_mutex[idx]);
    return 0;
}

const char *get_type(const char *key) {
    unsigned int idx = hash(key);
    pthread_mutex_lock(&bucket_mutex[idx]);
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                pthread_mutex_unlock(&bucket_mutex[idx]);
                return "none"; 
            }
            const char *typeStr = "none";
            if (entry->type == VALUE_STRING) {
                typeStr = "string";
            } else if (entry->type == VALUE_LIST) {
                typeStr = "list";
            }
            pthread_mutex_unlock(&bucket_mutex[idx]);
            return typeStr;
        }
        entry = entry->next;
    }
    pthread_mutex_unlock(&bucket_mutex[idx]);
    return "none";
}
