#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "collision.h"

//the projection of a shape onto a line
vector_t polygon_proj (list_t *shape, vector_t line) { 
    double min = vec_dot(*((vector_t *) list_get(shape, 0)), line);
    double max = vec_dot( *((vector_t *) list_get(shape, 0)), line);
    size_t shape_size = list_size(shape);

    for (size_t i = 1; i < shape_size; i++) {

        vector_t curr = * ((vector_t *) list_get(shape, i));

        double dot_prod = vec_dot(curr, line);
        if (dot_prod < min) {
            min = dot_prod;
        }
        if (dot_prod > max) {
            max = dot_prod;
        }
    }
    vector_t projection = {.x = min, .y = max};
    return projection;
}

void perpendicular_axis_helper(list_t * shape, list_t * axis) {
    size_t s = list_size(shape);

    size_t e = s - 1;
    for (size_t i = 0; i < s; i++) {
        vector_t v1 = *((vector_t *) list_get(shape, e));
        vector_t v2 = *((vector_t *) list_get(shape, i));
        vector_t edge = vec_add(v2, vec_negate(v1));
        vector_t perp_axis = {edge.y, -edge.x};
        vector_t unit_perp_axis = vec_unit(perp_axis);
        vector_t *unit_point = malloc(sizeof(vector_t));
        *unit_point = unit_perp_axis;
        list_add(axis, (vector_t *) unit_point);
        e = i;
    }

}

// find all perpendicular axis
list_t *perpendicular_axis(list_t *shape1, list_t *shape2) {
    size_t s1 = list_size(shape1);
    size_t s2 = list_size(shape2);
    size_t s3 = s1 + s2;
    list_t *axis = list_init(s3, free);

    perpendicular_axis_helper(shape1, axis);
    perpendicular_axis_helper(shape2, axis);
   
    return axis;
}

collision_info_t find_collision(list_t *shape1, list_t *shape2){
    list_t *axis = perpendicular_axis(shape1, shape2);
    size_t s = list_size(axis);
    vector_t line;

    double min_overlap = 1000000;
    double overlap;

    collision_info_t col;
    col.collided = true;
    col.axis = VEC_ZERO;

    for (size_t i = 0; i < s; i++) {
        line = *((vector_t *) list_get(axis, i));
        // get projections of edges onto perpendicular axis
        vector_t proj1 = polygon_proj(shape1, line);
        vector_t proj2 = polygon_proj(shape2, line);

        // check for overlap
        if (proj1.x > proj2.y || proj2.x > proj1.y) { 
            col.collided = false;
            return col;
        }

        // calculate overlap
        if (proj2.x < proj1.y) {
            overlap = proj1.y - proj2.x;
        }
        if (proj1.x < proj2.y) {
            overlap = proj2.y - proj1.x;
        }

        // find smallest overlap
        if (overlap < min_overlap) {
            // update collision axis
            col.axis = line;
            min_overlap = overlap;
        }
    }
    return col;
}