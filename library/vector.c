#include <stdlib.h>
#include <math.h>
#include "vector.h"

const vector_t VEC_ZERO = { .x = 0.0, .y = 0.0};

vector_t vec_add(vector_t v1, vector_t v2) {
    double v3x = v1.x + v2.x;
    double v3y = v1.y + v2.y;
    vector_t v3 = {.x = v3x, .y = v3y};
    return v3; 
}

vector_t vec_subtract(vector_t v1, vector_t v2){
    double v3x = v1.x - v2.x;
    double v3y = v1.y - v2.y;
    vector_t v3 = {.x = v3x, .y = v3y};
    return v3;
}

vector_t vec_negate(vector_t v) {
    double vnegx = -v.x;
    double vnegy = -v.y;
    vector_t vneg = {.x = vnegx, .y = vnegy};
    return vneg;
}

vector_t vec_multiply(double scalar, vector_t v) {
    double vecx = scalar * v.x;
    double vecy = scalar * v.y;
    vector_t vec = {.x = vecx, .y = vecy};
    return vec;
}

double vec_dot(vector_t v1, vector_t v2) {
    double dotprod = (v1.x * v2.x) + (v1.y * v2.y);
    return dotprod;
}

double vec_cross(vector_t v1, vector_t v2) {
    double z = (v1.x * v2.y) - (v1.y * v2.x);
    return z;
}

vector_t vec_rotate(vector_t v, double angle) {
    double vx = (v.x * cos(angle)) - (v.y * sin(angle));
    double vy = (v.x * sin(angle)) + (v.y * cos(angle));
    vector_t vec = {.x = vx, .y = vy};
    return vec;
}

vector_t vec_unit(vector_t v) {
    double magnitude = sqrt((v.x * v.x) + (v.y * v.y));
    vector_t unit = vec_multiply(1 / magnitude, v);
    return unit;
}