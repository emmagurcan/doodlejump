#include <stdlib.h>
#include <assert.h>
#include "body.h"
#include "polygon.h"
#include "vector.h"
#include "image.h"
#include "skin.h"

double base_elasticity = 0.8;

typedef struct body {
    list_t *shape;
    vector_t velocity;
    vector_t centroid;
    vector_t acceleration;
    double mass;
    double angle;
    double rot_speed;
    double elasticity;
    vector_t forces;
    vector_t impulses;
    rgb_color_t color;
    void *info;
    body_type_t type;
    free_func_t info_freer;
    bool flag;
    bool monster;
    skin_t *skin;
    bool visible;
} body_t;

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
    body_t *body = malloc(sizeof(body_t));
    body->shape = shape;
    body->velocity = VEC_ZERO;
    body->mass = mass;
    body->angle = 0.0;
    body->rot_speed = 0.0;
    body->elasticity = base_elasticity;
    body->color = color;
    body->acceleration = VEC_ZERO;
    body->centroid = polygon_centroid(shape);
    body->forces = VEC_ZERO;
    body->impulses = VEC_ZERO;
    body->flag = false;
    body->monster = false;
    body->skin = NULL;
    return body;
}

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color, body_type_t type) {
    body_t *body = malloc(sizeof(body_t));
    body->shape = shape;
    body->velocity = VEC_ZERO;
    body->mass = mass;
    body->angle = 0.0;
    body->rot_speed = 0.0;
    body->elasticity = base_elasticity;
    body->color = color;
    body->acceleration = VEC_ZERO;
    body->centroid = polygon_centroid(shape);
    body->forces = VEC_ZERO;
    body->impulses = VEC_ZERO;
    body->info = NULL;
    body->info_freer = NULL;
    body->type = type;
    body->flag = false;
    body->monster = false;
    body->skin = NULL;
    body->visible = false;
    return body;
}


void body_free(body_t *body) {
    list_free(body->shape);
    free(body);
}

list_t *body_get_shape(body_t *body) {
    assert(list_size(body->shape) > 0);
    list_t *list = list_init(list_size(body->shape), free);

    for (size_t i = 0; i < list_size(body->shape); i++) {
        vector_t *vec = malloc(sizeof(vector_t));
        *vec = vec_add(VEC_ZERO, *(vector_t *) list_get(body->shape, i));
        assert(vec != NULL);
        list_add(list, vec);

        assert(list_size(list) == i + 1);
    }

    assert(list_size(body->shape) == list_size(list));
    return list;
}

skin_t *body_get_skin(body_t *body) {
    return body->skin;
}

vector_t body_get_centroid(body_t *body) {
    return body->centroid;
}

vector_t body_get_velocity(body_t *body) {
    return body->velocity;
}

rgb_color_t body_get_color(body_t *body) {
    return body->color;
}

double body_get_elasticity(body_t *body) {
    return body->elasticity;
}

double body_get_mass(body_t *body) {
    return body->mass;
}

vector_t body_get_acceleration(body_t *body) {
    return body->acceleration;
}

vector_t body_get_force(body_t *body) {
    return body->forces;
}

vector_t body_get_impulse(body_t *body) {
    return body->impulses;
}

void *body_get_info(body_t *body) {
    return body->info;
}

body_type_t body_get_type(body_t *body) {
    return body->type;
}

void body_info_freer(body_t *body) {
    list_free(body->info);
    free(body);
}

void body_set_centroid(body_t *body, vector_t x) {
    polygon_translate(body->shape, vec_subtract(x, body->centroid));
    body->centroid = x;
}

void body_set_velocity(body_t *body, vector_t v) {
    body->velocity = v;
}

void body_set_elasticity(body_t *body, double e) {
    body->elasticity = e;
}

void body_set_rot_speed(body_t *body, double v) {
    body->rot_speed = v;
}

void body_set_acceleration(body_t *body, vector_t v) {
    body->acceleration = v;
}

void body_set_skin(body_t *body, skin_t *skin) {
    body->skin = skin;
}

void body_set_active_costume(body_t *body, size_t index) {
    skin_set_active_index(body->skin, index);
}

void body_set_rotation(body_t *body, double angle) {
    polygon_rotate(body->shape, angle - body->angle, body->centroid);
    body->angle = angle;
}

void body_set_info(body_t *body, void *info, free_func_t info_freer) {
    body->info = info;
    body->info_freer = info_freer;
}

void body_set_type(body_t *body, body_type_t type) {
    body->type = type;
}

void body_add_force(body_t *body, vector_t force) {
    body->forces = vec_add(body->forces, force);
    
}

void body_add_impulse(body_t *body, vector_t impulse) {
    body->impulses = vec_add(body->impulses, impulse);
}

void body_tick(body_t *body, double dt) {
    vector_t acc = vec_multiply(1 / body->mass, body->forces);
    body->acceleration = acc;

    vector_t impulse_vel = vec_multiply(1 / body->mass, body->impulses);
    vector_t acc_vel = vec_multiply(dt, body->acceleration);
    vector_t new_vel = vec_add(impulse_vel, acc_vel);
    vector_t updated_vel = vec_add(new_vel, body->velocity);
    vector_t average = vec_multiply(0.5, vec_add(updated_vel, body->velocity));
    body->velocity = updated_vel;

    vector_t translation = vec_multiply(dt, average);

    body->centroid = vec_add(body->centroid, translation);

    polygon_translate(body->shape, translation); 
    polygon_rotate(body->shape, body->rot_speed * dt, body->centroid);

    body->forces = VEC_ZERO;
    body->impulses = VEC_ZERO;
}

double body_get_rotation(body_t *body) {
    return body->angle;
}

void body_hide(body_t *body) {
    body->visible = false;
}

void body_show(body_t *body) {
    body->visible = true;
}

bool body_visible(body_t *body) {
    return body->visible;
}

void body_remove(body_t *body) {
    body->flag = true;
}

bool body_is_removed(body_t *body) {
    return body->flag;
}

void body_make_monster(body_t *body) {
    body->monster = true;
}

bool body_is_monster(body_t *body) {
    return body->monster;
}