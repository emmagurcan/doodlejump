#include "forces.h"
#include "test_util.h"
#include "scene.h" 
#include "body.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>


list_t *make_shape() {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){-1, +1};
    list_add(shape, v);
    return shape;
}

void test_gravity(){
    const double DT = 1e-6; 
    const double M1 = 1.0, M2 = 1e8; 
    const int STEPS = 10000;
    const double G = 1e3;

    vector_t vO = (vector_t) {0.0, 0.0};
    vector_t start = (vector_t){0.0, 10.0};
    vector_t start2 = (vector_t) {0.0, -100000.0};

    scene_t *scene = scene_init();
    body_t *body = body_init(make_shape(), M1, (rgb_color_t){0, 0, 0});
    body_t *big = body_init(make_shape(), M2, (rgb_color_t) {0, 0, 0});
    body_set_centroid (body, start);
    body_set_centroid (big, start2);
    body_set_velocity(body, vO);
    scene_add_body(scene, body);
    scene_add_body(scene, big);
    create_newtonian_gravity(scene, G, body, big);
    
    vector_t pos_prev = start;
    vector_t v_prev = vO;
    vector_t rad = vec_subtract(body_get_centroid(body), body_get_centroid(big));

    scene_tick(scene, DT);
    for (int i = 0; i<STEPS; i++){  
        double radius_length = sqrt(vec_dot(rad, rad));

        double grav = -1 * G * M2 / pow(radius_length, 2);

        vector_t accel = vec_multiply(grav/radius_length, rad);

        vector_t v = vec_add(v_prev, vec_multiply(DT, accel));
        assert(vec_isclose(body_get_velocity(body), v));
        
        vector_t pos = vec_add(pos_prev, vec_add(vec_multiply(DT, v_prev), vec_multiply(1/2 * pow(DT, 2), accel)));
        assert(vec_isclose(body_get_centroid(body), pos));
        
        v_prev = v;
        pos_prev = pos;
        rad = vec_subtract(body_get_centroid(body), body_get_centroid(big));
        
        
        scene_tick(scene, DT);
    }
    scene_free(scene); 
}

void test_projectile_motion(){
    const double DT = 1e-6; 
    const double M1 = 1.0, M2 = 1e8; 
    const int STEPS = 10000;
    const double G = 1e3;

    vector_t vO = (vector_t) {2.0, 0.0};
    vector_t start = (vector_t){10.0, 0.0};
    vector_t start2 = (vector_t) {0.0, -100000.0};

    scene_t *scene = scene_init();
    body_t *body = body_init(make_shape(), M1, (rgb_color_t){0, 0, 0});
    body_t *big = body_init(make_shape(), M2, (rgb_color_t) {0, 0, 0});
    body_set_centroid (body, start);
    body_set_centroid (big, start2);
    body_set_velocity(body, vO);
    scene_add_body(scene, body);
    scene_add_body(scene, big);
    create_newtonian_gravity(scene, G, body, big);

    vector_t pos_prev = start;
    vector_t v_prev = vO;
    vector_t rad = vec_subtract(body_get_centroid(body), body_get_centroid(big));

    scene_tick(scene, DT);
    for (int i = 0; i<STEPS; i++){  
        double radius_length = sqrt(vec_dot(rad, rad));

        double grav = -1 * G * M2 / pow(radius_length, 2);

        vector_t accel = vec_multiply(grav/radius_length, rad);

        vector_t v = vec_add(v_prev, vec_multiply(DT, accel));
        assert(vec_isclose(body_get_velocity(body), v));
        
        vector_t pos = vec_add(pos_prev, vec_add(vec_multiply(DT, v_prev), vec_multiply(1/2 * pow(DT, 2), accel)));
        assert(vec_isclose(body_get_centroid(body), pos));
        
        v_prev = v;
        pos_prev = pos;
        rad = vec_subtract(body_get_centroid(body), body_get_centroid(big));
        
        
        scene_tick(scene, DT);
    }
    scene_free(scene); 
}

// tests two orbiting bodies around eachother 
void test_orbit(){
    const double DT = 1e-6; 
    const int STEPS = 10000; 
    const double G = 1e3; 
    const double M1 = 4.5, M2 = 3.0; 
    const double CLOSE_ENOUGH = 1.0;

    double radius = 50.0; 
    double vel = sqrt(G*M2/radius); 

    vector_t start1 = (vector_t) {0.0, 0.0}; 
    vector_t start2 = vec_add(start1, (vector_t) {0.0, radius});
    
    scene_t *scene = scene_init();
    body_t *body1 = body_init(make_shape(), M1, (rgb_color_t){0, 0, 0});
    body_t *body2 = body_init(make_shape(), M2, (rgb_color_t){0, 0, 0});
    body_set_centroid(body1, start1); 
    body_set_centroid(body2, start2); 
    body_set_velocity(body2, (vector_t){vel, 0.0});
    scene_add_body(scene, body1); 
    scene_add_body(scene, body2); 

    create_newtonian_gravity(scene, G, body1, body2); 

    for (int i = 0; i < STEPS; i++){
        vector_t radius12 = vec_subtract(body_get_centroid(scene_get_body(scene, 0)), body_get_centroid(scene_get_body(scene, 1)));
        double new_radius = sqrt(vec_dot(radius12, radius12)); 
        assert(fabs(radius - new_radius) <= CLOSE_ENOUGH);
        scene_tick(scene, DT);
    }
    scene_free(scene);
}


int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_gravity)
    DO_TEST(test_projectile_motion)
    DO_TEST(test_orbit)

    puts("student_tests PASS");
}
