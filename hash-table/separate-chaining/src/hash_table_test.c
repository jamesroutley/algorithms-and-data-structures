#include <stdio.h>
#include <string.h>

#include "hash_table.h"

// MinUnit testing framework. http://www.jera.com/techinfo/jtns/jtn002.html
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)


int tests_run = 0;


static char* test_insert() {
    ht_item* item = ht_new_item("k", "v");
    ht_hash_table* ht = ht_new();

    ht_insert(ht, item);

    // Check only one item in hash table
    int count = 0;
    for (int i = 0; i < ht->size; i++) {
        if (ht->items[i] != NULL) {
            count++;
        }
    }
    mu_assert("error, num items in ht != 1", count == 1);

    // Check item has correct key, value, next, previous
    for (int i = 0; i < ht->size; i++) {
        if (ht->items[i] != NULL) {
            ht_item* cur_item = ht->items[i];
            mu_assert("error, key != k", strcmp(cur_item->key, "k") == 0);
            mu_assert("error, key != v", strcmp(cur_item->value, "v") == 0);
            mu_assert("error, next != NULL", cur_item->next == NULL);
            mu_assert("error, previous != NULL", cur_item->previous == NULL);
        }
    }

    // Tests passed
    ht_del_hash_table(ht);
    return 0;
}


static char* test_search_with_invalid_key() {
    // New empty hash table
    ht_hash_table* ht = ht_new();
    ht_item* item = ht_search(ht, "invalid_key");
    mu_assert("error, invalid key should return NULL", item == NULL);
    ht_del_hash_table(ht);
    return 0;
}


static char* test_search_with_valid_key() {
    ht_hash_table* ht = ht_new();
    ht_item* insert_item = ht_new_item("k", "v");
    ht_insert(ht, insert_item);
    ht_item* item = ht_search(ht, "k");
    mu_assert("error, insert_item != item", insert_item == item); 
    ht_del_hash_table(ht);
    return 0;
}


static char* test_search_with_colliding_keys() {
    // I know from manual testing that the strings "bz" and "4" collide using
    // ht_hash, with an m value of 53.
    ht_hash_table* ht = ht_new();

    ht_item* insert_item_a = ht_new_item("bz", "bz val");
    ht_item* insert_item_b = ht_new_item("4", "4 val");

    ht_insert(ht, insert_item_a);
    ht_insert(ht, insert_item_b);

    ht_item* item_a = ht_search(ht, "bz");
    ht_item* item_b = ht_search(ht, "4");

    mu_assert("error, insert_item_a != item_a", insert_item_a == item_a); 
    mu_assert("error, insert_item_b != item_b", insert_item_b == item_b); 

    ht_del_hash_table(ht);
    return 0;
}


static char* test_delete() {
    ht_hash_table* ht = ht_new();
    ht_item* insert_item = ht_new_item("k", "v");
    ht_insert(ht, insert_item);
    ht_delete(ht, insert_item);
    ht_item* item = ht_search(ht, "k");
    mu_assert("error, item != NULL", item == NULL); 
    ht_del_hash_table(ht);
    return 0;
}


static char* all_tests() {
    mu_run_test(test_insert);
    mu_run_test(test_search_with_invalid_key);
    mu_run_test(test_search_with_valid_key);
    mu_run_test(test_search_with_colliding_keys);
    mu_run_test(test_delete);
    return 0;
}


int main() {
    char* result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("all tests passed\n");
    }
    printf("%d tests run\n", tests_run);
    return result != 0;
}

