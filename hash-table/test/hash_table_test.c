#include <stdio.h>
#include <string.h>

#include "../src/hash_table.h"

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


static char* test_insert_lots_of_items() {
    ht_hash_table* ht = ht_new();
    for (int i = 0; i < 20000; i++) {
        char key[10];
        snprintf(key, 10, "%d", i);
        ht_insert(ht, key, "value");
    }
    return 0;
}


static char* test_insert_with_duplicate_key() {
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "key", "value 1");
    ht_insert(ht, "key", "value 2");

    mu_assert("error, expecting ht->count == 1 ", ht->count == 1);

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


static char* test_resize_up() {
    // Smallest ht size is 53. Table resizes when a load ratio of 0.7 is hit.
    // This should happen on the 38'th insert.
    ht_hash_table* ht = ht_new();
    for (int i = 0; i < 38; i++) {
        char key[10];
        snprintf(key, 10, "%d", i);
        ht_insert(ht, key, "value");
    }
    mu_assert("error, ht should be size 53", ht->size == 53);

    ht_insert(ht, "one extra", "value");
    mu_assert("error, ht should be size 101", ht->size == 101);

    ht_del_hash_table(ht);
    return 0;
}


static char* test_resize_down() {
    // Smallest ht size is 53. Table resizes when a load ratio of 0.7 is hit.
    // This should happen on the 38'th insert.
    ht_hash_table* ht = ht_new();
    for (int i = 0; i < 39; i++) {
        char key[10];
        snprintf(key, 10, "%d", i);
        ht_insert(ht, key, "value");
    }
    mu_assert("error, ht should be size 101", ht->size == 101);

    // Hash table should resize down when we hit 10 objects.
    for (int i = 0; i < 30; i++) {
        char key[10];
        snprintf(key, 10, "%d", i);
        ht_delete(ht, key);
    }

    /* ht_delete(ht, "0"); */
    mu_assert("error, ht should be size 53", ht->size == 53);

    ht_del_hash_table(ht);
    return 0;
}


static char* all_tests() {
    mu_run_test(test_insert);
    mu_run_test(test_insert_lots_of_items);
    mu_run_test(test_insert_with_duplicate_key);
    mu_run_test(test_search_with_invalid_key);
    mu_run_test(test_search_with_valid_key);
    mu_run_test(test_search_with_colliding_keys);
    mu_run_test(test_delete);
    mu_run_test(test_resize_up);
    mu_run_test(test_resize_down);
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

