#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"

#include "hash_table.h"

int HT_TABLE_SIZES[] = {53, 101, 211, 401, 809, 1601, 3203, 6421}; 


/*
 * Initialises a new item containing k: v
 */
static ht_item* ht_new_item(char* k, char* v) {
    ht_item* i = xmalloc(sizeof(ht_item));
    i->key = xmalloc(strlen(k) + 1);
    i->value = xmalloc(strlen(v) + 1);
    
    strcpy(i->key, k);
    strcpy(i->value, v);

    return i;
}


/*
 * Deletes the ht_item i
 */
static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}


/*
 * Initialises a new empty hash table
 */
ht_hash_table* ht_new() {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->size_index = 0;
    ht->size = HT_TABLE_SIZES[ht->size_index];
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}


/*
 * Deletes the hash table
 */
void ht_del_hash_table(ht_hash_table* ht) {
    // Iterate through items and delete any that are found
    for (int i = 0; i < ht->size; i++) {
        if (ht->items[i] != NULL) {
            ht_del_item(ht->items[i]);
        }
    }
    free(ht->items);
    free(ht);
}


/*
 * Returns the hash of 's', an int between 0 and 'm'.
 */
static int ht_hash(char* s, int m) {
    long hash = 0;
    int a = 128;  // Length of alphabet. ASCII codes are numbered 0 to 127.
    int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        /* Map char to a large integer */ 
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    // hash has just been moduloed, so should be below the max int size, as
    // long as the number of buckets are low enough.
    return (int)hash;
}


/*
 * Inserts the 'key': 'value' pair into the hash table
 */
void ht_insert(ht_hash_table* ht, char* key, char* value) {
    // TODO: increment load number
    ht_item* i = ht_new_item(key, value);
    int index = ht_hash(i->key, ht->size);
    
    // Collision
    int jump = 3;
    while (ht->items[index] != NULL) {
        index += jump;
        index = index % ht->size;
        jump += 2;
    }

    // index points to an empty bucket
    ht->items[index] = i;
}


/*
 * Returns the value associated with 'key', or NULL if the key doesn't exist
 */
char* ht_search(ht_hash_table* ht, char* key) {
    int index = ht_hash(key, ht->size);

    int jump = 3;
    while (ht->items[index] != NULL) {
        if (strcmp(ht->items[index]->key, key) == 0) {
            return ht->items[index]->value;
        }
        index += jump;
        index = index % ht->size;
        jump += 2;
    }
    return NULL;
}


/*
 * Deletes key's item from the hash table. Does nothing if 'key' doesn't exist
 */
void ht_delete(ht_hash_table* ht, char* key) {
    int index = ht_hash(key, ht->size);
    int jump = 3;
    while (ht->items[index] != NULL) {
        if (strcmp(ht->items[index]->key, key) == 0) {
            ht_del_item(ht->items[index]);
            ht->items[index] = NULL;
        }
        index += jump;
        index = index % ht->size;
        jump += 2;
    }
}
