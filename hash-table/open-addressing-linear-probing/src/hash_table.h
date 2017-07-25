#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_


// ht_item is an item in the hash table
typedef struct ht_item ht_item;
struct ht_item {
    char* key;
    char* value;
};


typedef struct {
    int size_index;
    int size;
    // TODO: we're storing an array of pointers to items - this is maybe
    // wasted space?
    ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new();
void ht_del_hash_table(ht_hash_table* ht);

void ht_insert(ht_hash_table* ht, char* key, char* value);
char* ht_search(ht_hash_table* ht, char* key);
void ht_delete(ht_hash_table* h, char* key);

#endif  // HASH_TABLE_H_
