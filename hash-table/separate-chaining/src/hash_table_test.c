#include <stdio.h>
#include <string.h>

#include "hash_table.h"

// MinUnit testing framework. http://www.jera.com/techinfo/jtns/jtn002.html
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)
#define strings_equal(a, b) strcmp(a, b) == 0


int tests_run = 0;


static char* test_insert() {
    ht_hash_table* ht = ht_new();

    ht_insert(ht, "k", "v");

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
        }
    }

    // Tests passed
    ht_del_hash_table(ht);
    return 0;
}


static char* test_search_with_invalid_key() {
    // New empty hash table
    ht_hash_table* ht = ht_new();
    char* value = ht_search(ht, "invalid_key");
    mu_assert("error, invalid key should return NULL", value == NULL);
    ht_del_hash_table(ht);
    return 0;
}


static char* test_search_with_valid_key() {
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "k", "v");
    char* value = ht_search(ht, "k");
    mu_assert("error, unexpected value", strings_equal(value, "v")); 
    ht_del_hash_table(ht);
    return 0;
}


static char* test_search_with_colliding_keys() {
    // I know from manual testing that the strings "bz" and "4" collide using
    // ht_hash, with an m value of 53.
    ht_hash_table* ht = ht_new();

    ht_insert(ht, "bz", "bz value");
    ht_insert(ht, "4", "4 value");

    char* value_a = ht_search(ht, "bz");
    char* value_b = ht_search(ht, "4");

    mu_assert("error, insert_item_a != item_a", 
        strings_equal(value_a, "bz value")); 
    mu_assert("error, insert_item_b != item_b", 
        strings_equal(value_b, "4 value")); 

    ht_del_hash_table(ht);
    return 0;
}


static char* test_delete() {
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "k", "v");
    ht_delete(ht, "k");
    char* value = ht_search(ht, "k");
    mu_assert("error, value != NULL", value == NULL); 
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

