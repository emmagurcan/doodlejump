#ifndef __GAME_MAKE_OBJECTS_H__
#define __GAME_MAKE_OBJECTS_H__

#include "body.h"
#include "scene.h"
#include "vector.h"
#include "shape.h"
#include "list.h"
#include "color.h"
#include <stdlib.h>
#include <math.h>
#include "forces.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

// typedef enum {
//     SPRITE,
//     SPRITE_DEAD,
//     SPRITE_INVUL,
//     SPRITE_RESET,
//     SPRITE_START,
//     BASE,
//     PLATFORM,
//     MONSTER,
//     SHOT,
//     SPRING,
//     INDICATOR,
//     JET, 
//     START_SCREEN, 
//     DEATH_SCREEN
// } body_type_t;

SDL_Surface *image_load(char *file);

list_t *make_costumes_from_images(list_t *images);

body_t *make_base(scene_t *scene, vector_t location, double mass, double size, rgb_color_t color);

// for now, the sprite is a circle with radius 10, and center in the middle
body_t *make_sprite(scene_t *scene, vector_t start, double size, double mass, rgb_color_t color, size_t resolution, double acc, image_t default_image, image_t sprite_jet_image);

// Makes a monster
body_t *make_monster(scene_t *scene, vector_t start, double size, double mass, vector_t velo, rgb_color_t color, double monster_score, double acc, double bounce_height, size_t resolution, image_t image);

// makes a bullet
body_t *make_shot(scene_t *scene, vector_t start, double size, double mass, rgb_color_t color, vector_t velocity, size_t resolution, image_t image);

// makes a platform at given location with given color (this will be important when we add different types of platforms);
// also makes a collision between the sprite and the block made 
body_t *make_platform(scene_t *scene, vector_t centroid, double width, double height, double mass, vector_t velocity, rgb_color_t color, image_t image, double acc, double bounce_height, collision_handler_t coll);

// makes a spring
body_t *make_spring(scene_t *scene, vector_t base, double size, double mass, double acc, double bounce_height, rgb_color_t color, image_t image);

// makes a base
body_t *make_jet(scene_t *scene, vector_t base, double size, double mass, rgb_color_t color, double acc, double low_acc, double high_acc, double stop_acc, double start_dec, image_t image);

// makes indicator
body_t *make_indicator(scene_t *scene, vector_t centroid, double mass, double width, double height, rgb_color_t color);
#endif // #ifndef __GAME_MAKE_OBJECTS_H__