#include "skin.h"

// Costume structure that holds texture information (and maybe animations).
typedef struct costume {
    SDL_Texture *texture;
    double width;
    double height;
} costume_t;

costume_t *costume_init(SDL_Texture *texture, double width, double height) {
    costume_t *costume = malloc(sizeof(costume_t));
    costume->width = width;
    costume->height = height;
    costume->texture = texture;

    return costume;
}

void costume_free(costume_t *costume) {
    SDL_DestroyTexture(costume->texture);
    free(costume);
}

double costume_get_width(costume_t *costume) {
    return costume->width;
}

double costume_get_height(costume_t *costume) {
    return costume->height;
}

SDL_Texture *costume_get_texture(costume_t *costume) {
    return costume->texture;
}

// This gets included in body.
typedef struct skin {
    list_t *costumes; // List of textures and size details
    size_t active_index;
    bool show;
} skin_t;

skin_t *skin_init(list_t *costumes, bool show) {
    skin_t *skin = malloc(sizeof(skin_t));
    skin->active_index = 0;
    skin->costumes = costumes;
    skin->show = show;

    return skin;
}


void skin_show(skin_t *skin) {
    skin->show = true;
}

void skin_hide(skin_t *skin) {
    skin->show = false;
}

costume_t *skin_get_active_costume(skin_t *skin) {
    return (costume_t *) list_get(skin->costumes, skin->active_index);
}

size_t skin_get_active_index(skin_t *skin) {
    return skin->active_index;
}

bool skin_visible(skin_t *skin) {
    return skin->show;
}

void skin_set_active_index(skin_t *skin, size_t index) {
    skin->active_index = index;
}

void skin_free(skin_t *skin) {
    list_free(skin->costumes);
    free(skin);
}