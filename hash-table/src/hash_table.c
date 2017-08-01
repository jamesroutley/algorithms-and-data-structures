#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"

#include "hash_table.h"
#include "prime.h"


// HT_DELETED_ITEM is used to mark a bucket containing a deleted item
static ht_item* HT_DELETED_ITEM = &(ht_item) {NULL, NULL};


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
 * Initialises a new empty hash table using a particular size index
 */
static ht_hash_table* ht_new_sized(int size_index) {
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->size_index = size_index;

    int base_size = 50 * pow(2, ht->size_index);
    ht->size = next_prime(base_size);

    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}


/*
 * Initialises a new empty hash table
 */
ht_hash_table* ht_new() {
    return ht_new_sized(0);
}


/*
 * Deletes the hash table
 */
void ht_del_hash_table(ht_hash_table* ht) {
    // Iterate through items and delete any that are found
    for (int i = 0; i < ht->size; i++) {
        if (ht->items[i] != NULL && ht->items[i] != HT_DELETED_ITEM) {
            ht_del_item(ht->items[i]);
        }
    }
    free(ht->items);
    free(ht);
}


/*
 * Resize ht
 */
static void ht_resize(ht_hash_table* ht, int direction) {
    int new_size_index = ht->size_index + direction;
    if (new_size_index < 0) {
        // Don't resize down the smallest hash table
        return;
    }
    // Create a temporary new hash table to insert items into
    ht_hash_table* new_ht = ht_new_sized(new_size_index);
    // Iterate through existing hash table, add all items to new
    for (int i = 0; i < ht->size; i++) {
        if (ht->items[i] != NULL && ht->items[i] != HT_DELETED_ITEM) {
            ht_insert(new_ht, ht->items[i]->key, ht->items[i]->value);
        }
    }

    // Swap ht and new_ht's properties. Delete new_ht
    int tmp_size_index = ht->size_index;
    ht->size_index = new_ht->size_index;
    new_ht->size_index = tmp_size_index;

    int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    int tmp_count = ht->count;
    ht->count = new_ht->count;
    new_ht->count = tmp_count;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;
    ht_del_hash_table(new_ht);
}


/*
 * Returns the hash of 's', an int between 0 and 'm'.
 */
static int ht_generic_hash(char* s, int a, int m) {
    long hash = 0;
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


static int ht_hash(char* s, int num_buckets, int attempt) {
    int hash_a = ht_generic_hash(s, 151, num_buckets);
    int hash_b = ht_generic_hash(s, 163, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}


/*
 * Inserts the 'key': 'value' pair into the hash table
 */
void ht_insert(ht_hash_table* ht, char* key, char* value) {
    float load = (float)ht->count / ht->size;
    if (load > 0.7) {
        ht_resize(ht, 1);
    }
    ht_item* item = ht_new_item(key, value);

    // Cycle though filled buckets until we hit an empty or deleted one
    int index = ht_hash(item->key, ht->size, 0);
    int i = 1;
    while (ht->items[index] != NULL && ht->items[index] != HT_DELETED_ITEM) {
        index = ht_hash(item->key, ht->size, i);
        i++;
    } 

    // index points to a free bucket
    ht->items[index] = item;
    ht->count++;
}


/*
 * Returns the value associated with 'key', or NULL if the key doesn't exist
 */
char* ht_search(ht_hash_table* ht, char* key) {
    int index = ht_hash(key, ht->size, 0);
    int i = 1;
    while (ht->items[index] != NULL && ht->items[index] != HT_DELETED_ITEM) {
        if (strcmp(ht->items[index]->key, key) == 0) {
            return ht->items[index]->value;
        }
        index = ht_hash(key, ht->size, i);
        i++;
    } 
    return NULL;
}


/*
 * Deletes key's item from the hash table. Does nothing if 'key' doesn't exist
 */
void ht_delete(ht_hash_table* ht, char* key) {
    float load = (float)ht->count / ht->size;
    if (load < 0.1) {
        // TODO: rename to resize_down
        ht_resize(ht, -1);
    }
    ht->count--;

    int index = ht_hash(key, ht->size, 0);
    int i = 1;
    while (ht->items[index] != NULL && ht->items[index] != HT_DELETED_ITEM) {
        if (strcmp(ht->items[index]->key, key) == 0) {
            ht_del_item(ht->items[index]);
            ht->items[index] = HT_DELETED_ITEM;
        }
        index = ht_hash(key, ht->size, i);
        i++;
    } 
}
