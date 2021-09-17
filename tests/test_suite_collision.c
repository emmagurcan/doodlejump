#include <stdlib.h>
#include <stdbool.h>
#include "test_util.h"
#include <assert.h>
#include "shape.h"
#include "collision.h"


void test_find_collisions() {
    double elasticity = 0.8;
    // Make two test bodies
    list_t *circle1 = make_shape_circle(1.0, (vector_t) {0.0, 0.0}, 1000);
    list_t *circle2 = make_shape_circle(5.0, (vector_t) {100.0, 100.0}, 40);

    // Check no collision
    collision_info_t col_info = find_collision(circle2, circle1);

    assert(!col_info.collided);
    free(circle2);
    
    // Check collision
    list_t *circle3 = make_shape_circle(1.0, (vector_t) {0.5, 0.0}, 1000);

    collision_info_t col_info_2 = find_collision(circle3, circle1);

    assert(col_info_2.collided);
    assert(0.999 < col_info_2.axis.x && col_info_2.axis.x < 1.005);
    assert(-0.01 < col_info_2.axis.y && col_info_2.axis.y < 0.01);

    free(circle3);

    // Check near collision
    list_t *circle4 = make_shape_circle(1.0, (vector_t) {1.0, 0.0}, 100);

    collision_info_t col_info_3 = find_collision(circle1, circle4);

    assert(col_info_3.collided);
    assert(0.999 < col_info_2.axis.x && col_info_2.axis.x < 1.005);
    assert(-0.01 < col_info_2.axis.y && col_info_2.axis.y < 0.01);
    free(circle4);
    free(circle1);
}


int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }
    
    DO_TEST(test_find_collisions)

    puts("collision_test PASS");
}