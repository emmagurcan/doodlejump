#include "game_score.h"


score_t *score_init(vector_t actual_centroid, vector_t relative_centroid, double score) {
    score_t *game_score = malloc(sizeof(score));
    assert(game_score != NULL);
    game_score->actual_centroid = actual_centroid;
    game_score->relative_centroid = relative_centroid;
    game_score->score = score;
    return game_score;
}

void score_set_actual_centroid(score_t *game_score, vector_t centroid) {
    game_score->actual_centroid = centroid;
}

void score_set_relative_centroid(score_t *game_score, vector_t centroid) {
    game_score->relative_centroid = centroid;
}

void score_set_score(score_t *game_score, double score) {
    game_score->score = score;
}

vector_t score_get_actual_centroid(score_t *game_score) {
    return game_score->actual_centroid;
}

vector_t score_get_relative_centroid(score_t *game_score) {
    return game_score->relative_centroid;
}

double score_get_score(score_t *game_score) {
    return game_score->score;
}
