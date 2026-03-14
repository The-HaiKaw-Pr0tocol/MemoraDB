/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/hashTable.c
 * Module                    : Hash Table
 * Last Updating Author      : shady0503
 * Last Update               : 03/14/2026
 * Version                   : 1.0.0
 * 
 * Description:
 *  Implementation of simple hash table for MemoraDB key-value storage.
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "hashTable.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

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
static pthread_cond_t list_cond = PTHREAD_COND_INITIALIZER;

Entry *HASHTABLE[TABLE_SIZE] = {0};

static Entry *find_entry_locked(const char *key) {
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static List *get_list_if_exists_locked(const char *key) {
    Entry *entry = find_entry_locked(key);
    long long now = current_millis();

    if (!entry) {
        return NULL;
    }

    if (entry->expiry > 0 && entry->expiry <= now) {
        return NULL;
    }

    if (entry->type != VALUE_LIST) {
        return NULL;
    }

    return entry->data.list_value;
}

static List *get_or_create_list_locked(const char *key) {
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                return NULL;
            }
            if (entry->type == VALUE_LIST) {
                return entry->data.list_value;
            }
            return NULL;
        }
        entry = entry->next;
    }

    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry) {
        return NULL;
    }

    new_entry->key = strdup(key);
    if (!new_entry->key) {
        free(new_entry);
        return NULL;
    }

    new_entry->type = VALUE_LIST;
    new_entry->data.list_value = list_create();
    if (!new_entry->data.list_value) {
        free(new_entry->key);
        free(new_entry);
        return NULL;
    }

    new_entry->expiry = 0;
    new_entry->next = HASHTABLE[idx];
    HASHTABLE[idx] = new_entry;
    return new_entry->data.list_value;
}

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
            //-- Free old value based on type --//
            if (entry->type == VALUE_STRING) {
                free(entry->data.string_value);
            } else if (entry->type == VALUE_LIST) {
                list_free(entry->data.list_value);
            }
            
            entry->type = VALUE_STRING;
            entry->data.string_value = strdup(value);
            entry->expiry = expiry;
            pthread_mutex_unlock(&hashtable_mutex);
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
                pthread_mutex_unlock(&hashtable_mutex);
                return NULL;
            } else {
                if (entry->type == VALUE_STRING) {
                    const char *result = entry->data.string_value;
                    pthread_mutex_unlock(&hashtable_mutex);
                    return result;
                }
                pthread_mutex_unlock(&hashtable_mutex);
                return NULL;
            }
        }
        prev = entry;
        entry = entry->next;
    }

    pthread_mutex_unlock(&hashtable_mutex);
    return NULL;
}

List *get_list_if_exists(const char *key) {
    pthread_mutex_lock(&hashtable_mutex);
    List *list = get_list_if_exists_locked(key);
    pthread_mutex_unlock(&hashtable_mutex);
    return list;
}

/**
 * Delete a key from the hash table, handling both string and list types.
 * Removes the entry from the linked list and frees all associated memory.
 */
int delete_key(const char *key) {
    pthread_mutex_lock(&hashtable_mutex);
    unsigned int idx = hash(key);
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
            
            pthread_mutex_unlock(&hashtable_mutex);
            return 1;
        }
        prev = entry;
        entry = entry->next;
    }
    
    pthread_mutex_unlock(&hashtable_mutex);
    return 0;
}

const char *get_type(const char *key) {
    pthread_mutex_lock(&hashtable_mutex);
    unsigned int idx = hash(key);
    Entry *entry = HASHTABLE[idx];
    long long now = current_millis();
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->expiry > 0 && entry->expiry <= now) {
                pthread_mutex_unlock(&hashtable_mutex);
                return "none"; 
            }
            const char *typeStr = "none";
            if (entry->type == VALUE_STRING) {
                typeStr = "string";
            } else if (entry->type == VALUE_LIST) {
                typeStr = "list";
            }
            pthread_mutex_unlock(&hashtable_mutex);
            return typeStr;
        }
        entry = entry->next;
    }
    pthread_mutex_unlock(&hashtable_mutex);
    return "none";
}

size_t db_lpush_atomic(const char *key, char *values[], int value_count) {
    if (!key || !values || value_count <= 0) {
        return 0;
    }

    pthread_mutex_lock(&hashtable_mutex);

    List *list = get_or_create_list_locked(key);
    if (!list) {
        pthread_mutex_unlock(&hashtable_mutex);
        return 0;
    }

    size_t new_length = list_length(list);
    for (int i = 0; i < value_count; i++) {
        new_length = list_lpush(list, values[i]);
    }

    if (value_count > 1) {
        pthread_cond_broadcast(&list_cond);
    } else {
        pthread_cond_signal(&list_cond);
    }

    pthread_mutex_unlock(&hashtable_mutex);
    return new_length;
}

size_t db_rpush_atomic(const char *key, char *values[], int value_count) {
    if (!key || !values || value_count <= 0) {
        return 0;
    }

    pthread_mutex_lock(&hashtable_mutex);

    List *list = get_or_create_list_locked(key);
    if (!list) {
        pthread_mutex_unlock(&hashtable_mutex);
        return 0;
    }

    size_t new_length = list_length(list);
    for (int i = 0; i < value_count; i++) {
        new_length = list_rpush(list, values[i]);
    }

    if (value_count > 1) {
        pthread_cond_broadcast(&list_cond);
    } else {
        pthread_cond_signal(&list_cond);
    }

    pthread_mutex_unlock(&hashtable_mutex);
    return new_length;
}

char *db_blpop_wait_atomic(const char *key, long long timeout_ms) {
    if (!key) {
        return NULL;
    }

    pthread_mutex_lock(&hashtable_mutex);

    struct timespec deadline;
    if (timeout_ms > 0) {
        if (clock_gettime(CLOCK_REALTIME, &deadline) != 0) {
            pthread_mutex_unlock(&hashtable_mutex);
            return NULL;
        }

        deadline.tv_sec += timeout_ms / 1000;
        deadline.tv_nsec += (timeout_ms % 1000) * 1000000L;
        if (deadline.tv_nsec >= 1000000000L) {
            deadline.tv_sec += 1;
            deadline.tv_nsec -= 1000000000L;
        }
    }

    while (1) {
        List *list = get_list_if_exists_locked(key);
        if (list && list_length(list) > 0) {
            char *element = lpop_element(list);
            pthread_mutex_unlock(&hashtable_mutex);
            return element;
        }

        if (timeout_ms <= 0) {
            pthread_mutex_unlock(&hashtable_mutex);
            return NULL;
        }

        int wait_result = pthread_cond_timedwait(&list_cond, &hashtable_mutex, &deadline);
        if (wait_result == ETIMEDOUT) {
            pthread_mutex_unlock(&hashtable_mutex);
            return NULL;
        }
        if (wait_result != 0) {
            pthread_mutex_unlock(&hashtable_mutex);
            return NULL;
        }
    }
}
