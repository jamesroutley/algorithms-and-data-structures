#include <stdio.h>

#include "../src/prime.h"

// MinUnit testing framework. http://www.jera.com/techinfo/jtns/jtn002.html
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)

int tests_run = 0;


static char* test_is_prime() {
    typedef struct {
        int in;
        int expected;
    } test_case;
    
    test_case* test_cases[] = {
        &(test_case) {-1, -1},
        &(test_case) {0, -1},
        &(test_case) {1, -1},
        &(test_case) {2, 1},
        &(test_case) {3, 1},
        &(test_case) {4, 0},
        &(test_case) {5, 1},
        &(test_case) {13, 1},
        &(test_case) {51, 0},
    };

    int num_test_cases = sizeof(test_cases)/sizeof(test_case*);

    for (int i = 0; i < num_test_cases; i++) {
        test_case* tc = test_cases[i];
        /* printf("%d: ", tc->in); */
        /* printf("%d: ", is_prime(tc->in)); */
        /* printf("%d\n", tc->expected); */
        mu_assert("is_prime returned incorrect result", 
            is_prime(tc->in) == tc->expected);
    }
    return 0;
}


static char* test_next_prime() {
    typedef struct {
        int in;
        int expected;
    } test_case;
    
    test_case* test_cases[] = {
        &(test_case) {-1, 2},
        &(test_case) {0, 2},
        &(test_case) {1, 2},
        &(test_case) {2, 2},
        &(test_case) {3, 3},
        &(test_case) {4, 5},
        &(test_case) {5, 5},
        &(test_case) {50, 53},
    };

    int num_test_cases = sizeof(test_cases)/sizeof(test_case*);

    for (int i = 0; i < num_test_cases; i++) {
        test_case* tc = test_cases[i];
        /* printf("%d: ", tc->in); */
        /* printf("%d: ", next_prime(tc->in)); */
        /* printf("%d\n", tc->expected); */
        mu_assert("next_prime returned incorrect result", 
            next_prime(tc->in) == tc->expected);
    }
    return 0;
}



static char* all_tests() {
    mu_run_test(test_is_prime);
    mu_run_test(test_next_prime);
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
