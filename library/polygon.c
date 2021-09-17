#include <math.h>
#include <stdlib.h>
#include "polygon.h"
#include <stdbool.h>

double polygon_signed_area(list_t *polygon) {
    double area = 0.0;
    size_t size = list_size(polygon);
    size_t e = size - 1;
    for (size_t i = 0; i < size; i++) {
        vector_t *ve = list_get(polygon, e);
        vector_t *vi = list_get(polygon, i);
        area += (ve->x * vi->y);
        area -= (vi->x * ve->y);
        e = i;
    }
    return area / 2;
}

double polygon_area(list_t *polygon) {
    double signedarea = polygon_signed_area(polygon);
    return fabs(signedarea);
}

vector_t polygon_centroid(list_t *polygon) {
    double sarea = polygon_signed_area(polygon);
    size_t size = list_size(polygon);
    double x = 0.0;
    double y = 0.0;
    size_t e = size;

    for (size_t i = 0; i < size; i++) {
        e = (i + 1) % size;
        vector_t *vi = list_get(polygon, i);
        vector_t *ve = list_get(polygon, e);
        x += (vi->x + ve->x) * ((vi->x * ve->y) - (ve->x * vi->y));
        y += (vi->y + ve->y) * ((vi->x * ve->y) - (ve->x * vi->y));
    
    }
    x = x / (6 * sarea);
    y = y / (6 * sarea);
    vector_t v = {.x = x, .y = y};
    return v;
}

void polygon_translate(list_t *polygon, vector_t translation) {
    size_t size = list_size(polygon);
    for (size_t i = 0; i < size; i++) {
        vector_t *vi = list_get(polygon, i);
        vi[0] = vec_add(vi[0], translation);
    }
}

void polygon_rotate(list_t *polygon, double angle, vector_t point) {
    size_t size = list_size(polygon);
    polygon_translate(polygon, vec_negate(point));
    for (size_t i = 0; i < size; i++) {
        vector_t *v = list_get(polygon, i);
        v[0] = vec_rotate(v[0], angle);
    }
    polygon_translate(polygon, point);
}

bool circle_intersect(double poly_centx, double poly_centy, double ball_x, double ball_y, double poly_rad, double ball_rad){
    double distance = sqrt((poly_centx - ball_x) * (poly_centx - ball_x) + (poly_centy - ball_y) * (poly_centy - ball_y));
    if (distance <= poly_rad){
        return true;
    }
    return false;
}

bool polygon_overlap(list_t* ball1, list_t* ball2){
    for (size_t i = 0; i < list_size(ball1); i++){
        for(size_t j = 0; j < list_size(ball2); j++){
            vector_t* pacman = list_get(ball1, i);
            vector_t* ball = list_get(ball2, j);
            if (circle_intersect(pacman -> x, pacman -> y, ball -> x, ball -> y, 20.0, 2.0)){
                return true;
                }
            }
    }
    return false;
}