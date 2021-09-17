#include "scene.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

const size_t INITIAL_SIZE = 10;

typedef struct param {
    list_t *bodies;
    void *constant;
    free_func_t freer;
} param_t;

param_t *param_init(list_t *bodies, void *constant, free_func_t freer) {
    param_t *param = malloc(sizeof(param_t));
    param->bodies = bodies;
    param->constant = constant;
    param->freer = freer;
    return param;
}

body_t *param_get_body(param_t *param, size_t index) {
    return list_get(param->bodies, index);
}

void *param_get_constant(param_t *param) {
    return param->constant;
}

void param_free(param_t *param) {
    if (param->freer != NULL) {
        (param->freer)(param->constant);
    }
    free(param);
}

typedef struct scene {
    list_t *bodies;
    list_t *forces;
    list_t *params;
} scene_t;

scene_t *scene_init(void) {
    scene_t *scene = malloc(sizeof(scene_t));

    scene->bodies = list_init(INITIAL_SIZE, (free_func_t) body_free);
    scene->forces = list_init(INITIAL_SIZE, (free_func_t) free);
    scene->params = list_init(INITIAL_SIZE, (free_func_t) free);

    assert(scene != NULL);
    assert(scene->bodies != NULL);

    return scene;
}

void scene_free(scene_t *scene) {
    free(scene->forces);
    list_free(scene->params);
    free(scene);
}

size_t scene_bodies(scene_t *scene) {
    assert(scene != NULL);
    return list_size(scene->bodies);
}

size_t scene_forces(scene_t *scene) {
    return list_size(scene->forces);
}

body_t *scene_get_body(scene_t *scene, size_t index) {
    return (body_t *) list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
    list_add(scene->bodies, body);
}

void scene_remove_body(scene_t *scene, size_t index) {
    assert(index < scene_bodies(scene));
    body_t *body = scene_get_body(scene, index);
    if (!body_is_removed(body)) {
        body_remove(body);
    }
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux, free_func_t freer) {
    scene_add_bodies_force_creator(scene, forcer, aux, NULL, freer);
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer, void *aux, list_t *bodies, free_func_t freer) {
    param_t *param = param_init(bodies, aux, freer);
    list_add(scene->forces, (force_creator_t) forcer);
    list_add(scene->params, (param_t *) param);
}

void scene_tick(scene_t *scene, double dt) {
    for (size_t j = 0; j < scene_forces(scene); j++) {
        force_creator_t func = list_get(scene->forces, j);
        param_t *param = list_get(scene->params, j);
        func(param_get_constant(param));
    }

    // Dettach the forces from removed bodies
    for (size_t j = scene_forces(scene); j > 0; --j) {
        param_t *param = list_get(scene->params, j-1);
        if (param->bodies != NULL) {
            for (size_t i = 0; i < list_size(param->bodies); i++) {
                if (body_is_removed(param_get_body(param, i))) {
                    list_remove(scene->forces, j-1);
                    param_t *del_param = list_remove(scene->params, j-1);
                    param_free(del_param);
                    break;
                }  
            }
        }
    }

    for (size_t i = scene_bodies(scene); i > 0; --i) {
        body_t *body = scene_get_body(scene, i-1);
        if (body_is_removed(body)) {
            list_remove(scene->bodies, i-1);
            body_free(body);
        }
    }

    for (size_t i = 0; i < scene_bodies(scene); i++) {
        body_tick(scene_get_body(scene, i), dt);
    }
}