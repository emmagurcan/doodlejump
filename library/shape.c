#include "shape.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <stdio.h>

list_t *make_shape_star(vector_t center, size_t point_num, double outer_r, double inner_r) {
    assert(point_num != 0);
    list_t *star = list_init(point_num * 2, free);
    double rad = 2 * M_PI / point_num;

    vector_t far_point = {0.0, 0.0 + outer_r};
    double initial_angle_inner = (M_PI / point_num) + (M_PI / 2);

    vector_t close_point = {inner_r * cos(initial_angle_inner), inner_r * sin(initial_angle_inner)};

    for (size_t i = 0; i < point_num; i++){
        double rotate = rad * i;
        vector_t *far = malloc(sizeof(vector_t));
        vector_t *close = malloc(sizeof(vector_t));
        *far = far_point;
        *close = close_point;

        *far = vec_rotate(*far, rotate);
        *far = vec_add(*far, center);
       
        *close = vec_rotate(*close, rotate);
        *close = vec_add(*close, center);


        list_add(star, far);
        list_add(star, close);
    }

    return star;
}

list_t *make_shape_pacman(double angle, double radius, vector_t centroid, size_t n) {
    list_t *pac_points = list_init(3, (free_func_t) free);
    for (size_t i = 0; i <= n; i++) {
        double temp_angle = angle + (i * ((2 * M_PI) - (M_PI/3))) / n;
        double x = centroid.x + (radius * cos(temp_angle));
        double y = centroid.y + (radius * sin(temp_angle));
        vector_t *v = malloc(sizeof(vector_t));
        v->x = x;
        v->y = y;
        list_add(pac_points, v);
    }

    vector_t *v = malloc(sizeof(vector_t));
    v->x = centroid.x;
    v->y = centroid.y;
    list_add(pac_points, v);
    return pac_points;
}

list_t *make_shape_circle(double radius, vector_t centroid, size_t n){
    list_t *circle_points = list_init(n, (free_func_t) free);
    for (size_t i = 0; i <= n; i++) {
        double temp_angle = (i * 2 * M_PI) / n;
        double x = centroid.x + (radius * cos(temp_angle));
        double y = centroid.y + (radius * sin(temp_angle));
        vector_t *v = malloc(sizeof(vector_t));
        v->x = x;
        v->y = y;
        list_add(circle_points, v);
    }
    return circle_points;
}

list_t *make_shape_part_circle(double angle, double radius, vector_t centroid, size_t n){
    list_t *shape = list_init(3, (free_func_t) free);
    for (size_t i = 0; i <= n; i++) {
        double temp_angle = (i * ((2 * M_PI) - (angle))) / n;
        double x = centroid.x + (radius * cos(temp_angle));
        double y = centroid.y + (radius * sin(temp_angle));
        vector_t *v = malloc(sizeof(vector_t));
        v->x = x;
        v->y = y;
        list_add(shape, v);
    }

    vector_t *v = malloc(sizeof(vector_t));
    v->x = centroid.x;
    v->y = centroid.y;
    list_add(shape, v);
    return shape;
}

list_t *make_shape_ellipse(double angle, double x_radius, double y_radius, vector_t centroid, size_t n){
    list_t *ellipse_points = list_init(n, (free_func_t) free);
    for (size_t i = 0; i <= n; i++) {
        double temp_angle = (i * angle) / n;
        double x = centroid.x + (x_radius * cos(temp_angle));
        double y = centroid.y + (y_radius * sin(temp_angle));
        vector_t *v = malloc(sizeof(vector_t));
        v->x = x;
        v->y = y;
        list_add(ellipse_points, v);
    }
    return ellipse_points;
}

list_t *make_shape_rectangle(double width, double height, vector_t centroid) {
    double w = 0.5 * width; 
    double h = 0.5 * height;
    list_t *rect_points = list_init(4, (free_func_t) free);
    for (size_t i = 1; i <= 4; i ++) {
        double x = centroid.x;
        double y = centroid.y;
        if (i > 2) {
            y = y + h;
        }
        else {
            y = y - h;
        }
        if (i > 1 && i < 4) {
            x = x + w;
        }
        else {
            x = x - w;
        }
        vector_t *v = malloc(sizeof(vector_t));
        v->x = x;
        v->y = y;
        list_add(rect_points, v);
    }
    return rect_points;
}