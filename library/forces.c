#include <stdlib.h>
#include <math.h>
#include "collision.h"
#include "forces.h"
#include "scene.h"
#include <stdio.h>


const double TOO_CLOSE = 30.0;


vector_t compute_connect(body_t *body1, body_t *body2){
    vector_t center1 = body_get_centroid(body1);
    vector_t center2 = body_get_centroid(body2);
    vector_t connect21 = vec_subtract(center1, center2);
    return connect21;
}

void gravity(param_t *param) {
    body_t *body1 = param_get_body(param, 0);
    body_t *body2 = param_get_body(param, 1);
    double constant = *((double *) param_get_constant(param));

    vector_t connect21 = compute_connect(body1, body2);

    double distance = sqrt((connect21.x * connect21.x) + (connect21.y * connect21.y));

    if (distance <= TOO_CLOSE) {
        return;
    }

    double m1 = body_get_mass(body1);
    double m2 = body_get_mass(body2);

    double force_const = (1 * constant * m1 * m2) / pow(distance, 2);
    vector_t unit = vec_multiply(1 / distance, connect21);

    vector_t force = vec_multiply(force_const, unit);

    body_add_force(body1, vec_negate(force));
    body_add_force(body2, force);
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1, body_t *body2) {
    double *G_point = malloc(sizeof(double));
    *G_point = G;
    list_t *bodies = list_init(2, (free_func_t) free);
    list_add(bodies, body1);
    list_add(bodies, body2);

    param_t *aux = param_init(bodies, G_point, free);
    scene_add_bodies_force_creator(scene, (force_creator_t) gravity, aux, bodies, (free_func_t) free);
}

void spring(param_t *param) {
    body_t *body1 = param_get_body(param, 0);
    body_t *body2 = param_get_body(param, 1);
    double constant = *((double *) param_get_constant(param));

    vector_t connect21 = compute_connect(body1, body2);
    double distance = sqrt((connect21.x * connect21.x) + (connect21.y * connect21.y));

    vector_t unit = vec_multiply(1 / distance, connect21);
    vector_t force = vec_multiply((constant * distance), unit);

    body_add_force(body1, vec_negate(force));
    body_add_force(body2, force);
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
    double *k_point = malloc(sizeof(double));
    *k_point = k;
    list_t *bodies = list_init(2, (free_func_t) free);
    list_add(bodies, body1);
    list_add(bodies, body2);

    param_t *aux = param_init(bodies, k_point, free);
    scene_add_bodies_force_creator(scene, (force_creator_t) spring, aux, bodies, (free_func_t) free);
}

void drag(param_t *param) {
    body_t *body = param_get_body(param, 0);
    double gamma = *((double *) param_get_constant(param));

    vector_t velocity = body_get_velocity(body);

    if (velocity.x == 0.0 && velocity.y == 0.0) {
        return;
    }
    
    double magnitude = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
    double consts = -1 * magnitude * gamma;

    vector_t unit = vec_multiply(1 / magnitude, velocity);
    vector_t drag_force = vec_multiply(consts, unit);
    body_add_force(body, drag_force);
}

void create_drag(scene_t *scene, double gamma, body_t *body){
    double *gamma_point = malloc(sizeof(double));
    *gamma_point = gamma;
    list_t *bodies = list_init(2, (free_func_t) free);
    list_add(bodies, body);

    param_t *aux = param_init(bodies, gamma_point, free);
    scene_add_bodies_force_creator(scene, (force_creator_t) drag, aux, bodies, (free_func_t) free);
}

typedef struct col_aux {
    collision_handler_t handler;
    body_t *body1;
    body_t *body2;
    void *aux;
    free_func_t freer;
    bool colliding;
} col_aux_t;

col_aux_t *col_aux_init(collision_handler_t handler, body_t *body1, body_t *body2, void *aux, free_func_t freer) {
    col_aux_t *col_aux = malloc(sizeof(col_aux_t));
    col_aux->handler = handler;
    col_aux->body1 = body1;
    col_aux->body2 = body2;
    col_aux->aux = aux;
    col_aux->freer = freer;
    col_aux->colliding = false;

    return col_aux;
}

void col_aux_free(col_aux_t* col_aux) {
    if (col_aux->freer != NULL) {
        (col_aux->freer)(col_aux->aux);
    }
    free(col_aux);
}

void collision_force_creator(col_aux_t *col_aux) {
    body_t *body1 = col_aux->body1;
    body_t *body2 = col_aux->body2;

    collision_info_t col_info = find_collision(body_get_shape(body1), body_get_shape(body2));
    if (col_info.collided  && !col_aux->colliding) {
        vector_t axis = col_info.axis;
        void *aux = col_aux->aux;
        collision_handler_t handler = col_aux->handler;
        col_aux->colliding = true;
        handler(body1, body2, axis, aux);
    }
    if (!col_info.collided) {
        col_aux->colliding = false;
    }
    
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2, collision_handler_t handler, void *aux, free_func_t freer) {
    col_aux_t *col_aux = col_aux_init(handler, body1, body2, aux, freer);
    
    list_t *bodies = list_init(2, (free_func_t) free);
    list_add(bodies, body1);
    list_add(bodies, body2);

    scene_add_bodies_force_creator(scene, (force_creator_t) collision_force_creator, col_aux, bodies, col_aux_free);
}

void physics_collision(body_t *body1, body_t *body2, vector_t axis, double *elasticity) {
    double mass1 = body_get_mass(body1);
    double mass2 = body_get_mass(body2);
    vector_t vel1 = body_get_velocity(body1);
    vector_t vel2 = body_get_velocity(body2);


    double u1 = vec_dot(axis, vel1);
    double u2 = vec_dot(axis, vel2);

    double e = *elasticity;

    double impulse;
    if (mass1 == INFINITY) {
        impulse = mass2;
    }
    else if (mass2 == INFINITY) {
        impulse = mass1;
    }
    else {
        impulse = mass1 * mass2 / (mass1 + mass2);
    }

    impulse *=  (1 + e) * (u2 - u1);

    body_add_impulse(body1, vec_multiply(impulse, axis));
    body_add_impulse(body2, vec_multiply(-impulse, axis));
}


void create_physics_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    double *e = malloc(sizeof(double));
    *e = elasticity;

    col_aux_t *col_aux = col_aux_init(physics_collision, body1, body2, e, free);
    
    list_t *bodies = list_init(2, (free_func_t) free);
    list_add(bodies, body1);
    list_add(bodies, body2);

    scene_add_bodies_force_creator(scene, (force_creator_t) collision_force_creator, col_aux, bodies, col_aux_free);
}


void destructive_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    body_remove(body1);
    body_remove(body2);
}

void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    col_aux_t *col_aux = col_aux_init(destructive_collision, body1, body2, NULL, free);
    
    list_t *bodies = list_init(2, (free_func_t) free);
    list_add(bodies, body1);
    list_add(bodies, body2);

    scene_add_bodies_force_creator(scene, (force_creator_t) collision_force_creator, col_aux, bodies, col_aux_free);
}

// Applys a gravitational force to bodies.
void accelerate_down(param_t *param) {
    body_t *sprite = param_get_body(param, 0);
    body_add_force(sprite, (vector_t) {0.0, - body_get_mass(sprite) * (* (double *) param_get_constant(param))});
}

// Collision handler between the sprite and the platform.
void platform_collision(body_t *platform, body_t *sprite, vector_t axis, void *aux) {
    vector_t sprite_velocity = body_get_velocity(sprite);
    if (sprite_velocity.y < 0) {
        body_set_velocity(sprite, (vector_t) {sprite_velocity.x, sqrt(2 * ((double *)aux)[0] * ((double *)aux)[1])});
    }
}

void breaking_platform_collision(body_t *platform, body_t *sprite, vector_t axis, void *aux) {
    if (body_get_velocity(sprite).y < 0) {
        body_remove(platform);
    }
}