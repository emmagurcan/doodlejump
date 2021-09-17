#ifndef __GAME_SCORE_H__
#define __GAME_SCORE_H__

#include "body.h"
#include "scene.h"
#include "vector.h"
#include "shape.h"
#include "list.h"
#include "color.h"
#include <stdlib.h>
#include <math.h>
#include "forces.h"

typedef struct score {
    vector_t actual_centroid;
    vector_t relative_centroid;
    double score;

}score_t;

score_t *score_init(vector_t actual_centroid, vector_t relative_centroid, double score);

void score_set_actual_centroid(score_t *game_score, vector_t centroid);

void score_set_relative_centroid(score_t *game_score, vector_t centroid);

void score_set_score(score_t *game_score, double score);

vector_t score_get_actual_centroid(score_t *game_score);

vector_t score_get_relative_centroid(score_t *game_score);

double score_get_score(score_t *game_score);

#endif // #ifndef __GAME_SCORE_H__