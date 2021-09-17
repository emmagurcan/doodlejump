#include "list.h"
#include "test_util.h"
#include <assert.h>
#include <stdlib.h>

void test_list_size0() {
    list_t *l = list_init(10, free);
    assert(list_size(l) == 0);
    list_free(l);
}

void test_list_size1() {
    list_t *l = list_init(10, free);
    void **e = malloc(sizeof(void *));
    assert(list_size(l) == 0);

    list_add(l, e);
    assert(list_size(l) == 1);

    list_remove(l, 0);
    assert(list_size(l) == 0);

    int *elem = malloc(sizeof(int));
    *elem = 1;
    list_add(l, elem);
    int out = *(int *)list_get(l, 0);
    assert(out == *elem);

    list_free(l);
}

void test_list_sizebig() {
    list_t *l = list_init(10, free);
    for (size_t i = 0; i < 100; i++) {
        vector_t *elem = malloc(sizeof(vector_t));
        *elem = (vector_t) {0.0, i};
        list_add(l, elem);
        assert(list_size(l) == (i+1));
    }

    list_free(l);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_list_size0);
    DO_TEST(test_list_size1);
    DO_TEST(test_list_sizebig);

    puts("list_test PASS");
}
