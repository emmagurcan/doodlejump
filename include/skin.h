#ifndef __SKIN_H__
#define __SKIN_H__

#include "list.h"
#include "image.h"
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>


typedef struct costume costume_t;

costume_t *costume_init(SDL_Texture *Texture, double width, double height);

void costume_free(costume_t *costume);

double costume_get_width(costume_t *costume);

double costume_get_height(costume_t *costume);

SDL_Texture *costume_get_texture(costume_t *costume);

// This gets included in body.
typedef struct skin skin_t;

skin_t *skin_init(list_t *images, bool show);


// ***** FOR SCORE IMPLEMENTATION AND ANYTHING ELSE ******
// skin_t *skin_text_init(list_t *images, bool show, SDL_Renderer renderer) {
//     skin_t *skin = malloc(sizeof(skin_t));
//     skin->active_costume = 0;
//     skin->costumes = list_init(list_size(images), costume_free);
//     for (size_t i = 0; i < list_size(images); i++) {
//         costume_t *costume = costume_init(*(image_t *)list_get(images, i), renderer);
//         list_add(costumes, costume);
//     }
//     skin->show = show;

//     return skin;
// }


void skin_show(skin_t *skin);

void skin_hide(skin_t *skin);

costume_t *skin_get_active_costume(skin_t *skin);

size_t skin_get_active_index(skin_t *skin);

bool skin_visible(skin_t *skin);

void skin_set_active_index(skin_t *skin, size_t index);

void skin_free(skin_t *skin);


#endif