#include "game_make_objects.h"
#include <stdio.h>
#include <stdlib.h>
#include "sdl_wrapper.h"

#include "image.h"
#include "skin.h"

list_t *make_costumes_from_images(list_t *images) {
    list_t *costumes = list_init(list_size(images), free);

    for (size_t i = 0; i < list_size(images); i++) {
        list_add(costumes, sdl_create_image_costume(*(image_t *) list_get(images, i)));
    }

    list_free(images);
    return costumes;
}

// for now, the sprite is a circle with radius 10, and center in the middle
body_t *make_sprite(scene_t *scene, vector_t start, double size, double mass, rgb_color_t color, size_t resolution, double acc_val, image_t default_image, image_t sprite_jet_image) {
    list_t *sprite_points = make_shape_circle(size, start, resolution);
    body_t *sprite = body_init_with_info(sprite_points, mass, color, SPRITE);
    double *score = malloc(sizeof(double));
    *score = 0;
    body_set_info(sprite, score, free);
    // create textures function here.
    list_t *costumes = list_init(2, free);
    list_add(costumes, sdl_create_image_costume(default_image));
    list_add(costumes, sdl_create_image_costume(sprite_jet_image));
    skin_t *skin = skin_init(costumes, true);
    body_set_skin(sprite, skin);
    body_set_centroid(sprite, start);
    scene_add_body(scene, sprite);
    list_t *bodies = list_init(1, NULL);
    list_add(bodies, sprite);
    double *acc = malloc(sizeof(double));
    *acc = acc_val;
    param_t *param = param_init(bodies, acc, free);
    scene_add_bodies_force_creator(scene, accelerate_down, param, bodies, param_free);

    return sprite;
}

void monster_collision(body_t *sprite, body_t *monster, vector_t axis, void *aux) {
    if (body_get_type(sprite) == SPRITE) {
        vector_t sprite_velocity = body_get_velocity(sprite);
        if (sprite_velocity.y < 0) {
            body_set_velocity(sprite, (vector_t) {sprite_velocity.x, sqrt(2 * ((double *) aux)[0] * ((double *) aux)[1])});
            body_remove(monster);
            double *score = (double *) body_get_info(sprite);
            *score += ((double *)aux)[2];
        }
        else {
            body_set_type(sprite, SPRITE_DEAD);
        }
    }
}

// Makes a monster
body_t *make_monster(scene_t *scene, vector_t start, double size, double mass, vector_t velo, rgb_color_t color, double monster_score, double acc, double bounce_height, size_t resolution, image_t image) {
    body_t *sprite = scene_get_body(scene, 1);
    list_t *monster_points = make_shape_circle(size, start, resolution);
    body_t *monster = body_init_with_info(monster_points, mass, color, MONSTER);
    list_t *costumes = list_init(1, NULL);
    list_add(costumes, sdl_create_image_costume(image));
    skin_t *skin = skin_init(costumes, true);
    body_set_skin(monster, skin);
    body_set_velocity(monster, velo);
    scene_add_body(scene, monster);
    double *vals = malloc(sizeof(double) * 3);
    vals[0] = acc;
    vals[1] = bounce_height;
    vals[2] = monster_score;
    create_collision(scene, sprite, monster, monster_collision, vals, free);
    return monster;
}

// makes a bullet
body_t *make_shot(scene_t *scene, vector_t start, double size, double mass, rgb_color_t color, vector_t velocity, size_t resolution, image_t image) {
    list_t *shot_points = make_shape_ellipse(2*M_PI, size * 0.5, size, start, resolution);
    body_t *shot = body_init_with_info(shot_points, mass, color, SHOT);
    
    list_t *costumes = list_init(1, NULL);
    list_add(costumes, sdl_create_image_costume(image));
    skin_t *skin = skin_init(costumes, true);
    body_set_skin(shot, skin);
    
    body_set_centroid(shot, start);
    body_set_velocity(shot, velocity);
    scene_add_body(scene, shot);
    return shot;
}

// makes a platform at given location with given color (this will be important when we add different types of platforms);
// also makes a collision between the sprite and the block made 
body_t *make_platform(scene_t *scene, vector_t centroid, double width, double height, double mass, vector_t velocity, rgb_color_t color, image_t image, double acc, double bounce_height, collision_handler_t coll) {
    body_t *sprite = scene_get_body(scene, 1);
    list_t *plat_points = make_shape_rectangle(width, height, centroid);
    body_t *platform = body_init_with_info(plat_points, mass, color, PLATFORM);

    list_t *costumes = list_init(1, NULL);
    list_add(costumes, sdl_create_image_costume(image));
    skin_t *skin = skin_init(costumes, true);
    body_set_skin(platform, skin);

    body_set_velocity(platform, velocity);
    scene_add_body(scene, platform);
    double *vals = malloc(sizeof(double) * 2);
    vals[0] = acc;
    vals[1] = bounce_height;

    create_collision(scene, platform, sprite, coll, vals, NULL);
    return platform;
}

// Makes a spring
body_t *make_spring(scene_t *scene, vector_t base, double size, double mass, double acc, double bounce_height, rgb_color_t color, image_t image) {
    body_t *sprite = scene_get_body(scene, 1);
    list_t *spring_points = make_shape_rectangle(size, size, vec_add(base, (vector_t) {0.0, size/2}));
    body_t *spring = body_init_with_info(spring_points, mass, color, SPRING);

    list_t *costumes = list_init(1, NULL);
    list_add(costumes, sdl_create_image_costume(image));
    skin_t *skin = skin_init(costumes, true);
    body_set_skin(spring, skin);

    scene_add_body(scene, spring);
    double *vals = malloc(sizeof(double) * 2);
    vals[0] = acc;
    vals[1] = bounce_height;
    create_collision(scene, spring, sprite, platform_collision, vals, free);
    return spring;
}

body_t *make_indicator(scene_t *scene, vector_t centroid, double mass, double width, double height, rgb_color_t color) {
    list_t *points = make_shape_rectangle(width, height, centroid);
    body_t *indicator = body_init_with_info(points, mass, color, INDICATOR);
    scene_add_body(scene, indicator);
    return indicator;
}

// makes a relative point at the base. Used for elevation calculations.
body_t *make_base(scene_t *scene, vector_t location, double mass, double size, rgb_color_t color) {
    list_t *base_points = make_shape_rectangle(size, size, location);
    body_t *base = body_init_with_info(base_points, 0.1, color, BASE);
    scene_add_body(scene, base);
    return base;
}

typedef struct {
    scene_t *scene;
    double initial_elevation;
    double acc;
    double low_acc;
    double high_acc;
    double stop_acc;
    double start_dec;
} jet_data_t;

// Force creator
void jet_force(param_t *param) {
    body_t *sprite = param_get_body(param, 0);
    body_t *jet = param_get_body(param, 1);
    body_set_centroid(jet, body_get_centroid(sprite));

    jet_data_t *jet_data = (jet_data_t *) param_get_constant(param);
    scene_t *scene = jet_data->scene;
    double delta_elevation = body_get_centroid(sprite).y - body_get_centroid(scene_get_body(scene, 0)).y - jet_data->initial_elevation; 
    if (delta_elevation < jet_data->stop_acc) {
        body_add_force(sprite, (vector_t) {0.0, body_get_mass(sprite) * jet_data->high_acc});
    }
    else if (delta_elevation <= jet_data->start_dec) {
        body_add_force(sprite, (vector_t) {0.0, body_get_mass(sprite) * jet_data->acc});
    }
    else if (delta_elevation > jet_data->start_dec) {
        body_add_force(sprite, (vector_t) {0.0, body_get_mass(sprite) * jet_data->low_acc});
        if (body_get_velocity(sprite).y <= 0.0) {
            body_remove(jet);
            body_set_type(sprite, SPRITE);
            body_set_active_costume(sprite, 0);
        }
    }
    
}

// Collision handler
void jet_collision(body_t *sprite, body_t *jet, vector_t axis, void *aux) {
    if(body_get_type(sprite) == SPRITE) {
        // Need a structure for current image
        body_set_type(sprite, SPRITE_INVUL);
        body_set_active_costume(sprite, 1);
        skin_hide(body_get_skin(jet));
        body_hide(jet);
        body_set_velocity(sprite, (vector_t) {body_get_velocity(sprite).x, 100});
        body_set_centroid(jet, body_get_centroid(sprite));

        list_t *bodies = list_init(2, NULL);
        list_add(bodies, sprite);
        list_add(bodies, jet);

        param_t *param = param_init(bodies, aux, NULL);

        scene_add_bodies_force_creator(((jet_data_t *) aux)->scene, (force_creator_t) jet_force, param, bodies, param_free);
    }
}

body_t *make_jet(scene_t *scene, vector_t base, double size, double mass, rgb_color_t color, double acc, double low_acc, double high_acc, double stop_acc, double start_dec, image_t image) {
    body_t *sprite = scene_get_body(scene, 1);
    list_t *jet_points = make_shape_rectangle(size, size, vec_add(base, (vector_t) {0.0, size/2}));

    double initial_elevation = base.y - body_get_centroid(scene_get_body(scene, 0)).y;
    jet_data_t *jet_data = malloc(sizeof(jet_data_t));
    *jet_data = (jet_data_t) {scene, initial_elevation, acc, low_acc, high_acc, stop_acc, start_dec};

    body_t *jet = body_init_with_info(jet_points, mass, color, JET);
    list_t *costumes = list_init(1, free);
    list_add(costumes, sdl_create_image_costume(image));
    skin_t *skin = skin_init(costumes, true);
    body_set_skin(jet, skin);
    scene_add_body(scene, jet);
    create_collision(scene, sprite, jet, jet_collision, jet_data, free);

    return jet;
}