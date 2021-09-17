#include <stdlib.h>
#include <assert.h>
#include "test_util.h"
#include "shape.h"
#include "list.h"
#include "vector.h"
#include <stdio.h>
#include "polygon.h"

#define WINDOW_TITLE "CS 3"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
const vector_t MIN = {0, 0};
const vector_t MAX = {WINDOW_WIDTH, WINDOW_HEIGHT};


void test_star() {
    list_t *star = make_shape_star((vector_t) {0.0, 0.0}, 5, 5.0, 3.0);

    assert(list_size(star) == 10);

    assert(vec_isclose(*(vector_t *)list_get(star, 0), (vector_t) {0.0, 5.0}));

    list_free(star);
}

void test_star_big() {
    list_t *star = make_shape_star((vector_t) {0.0, 0.0}, 10, 5.0, 3.0);

    assert(list_size(star) == 20);

    assert(vec_isclose(*(vector_t *)list_get(star, 0), (vector_t) {0.0, 5.0}));

    list_free(star);
}

void test_rectangle() {
    list_t *rect = make_shape_rectangle(1.0, 1.0, (vector_t) {0.5, 0.5});

    vector_t test_point = *(vector_t *) list_get(rect, 0);
    printf("%f %f\n", test_point.x, test_point.y);
    assert(vec_equal(polygon_centroid(rect), (vector_t) {0.5, 0.5}));
    assert(vec_equal(test_point, (vector_t) {0.0, 0.0}));
    
    list_free(rect);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_star);
    DO_TEST(test_star_big);
    DO_TEST(test_rectangle);

    puts("shape_test PASS");
}