#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "list.h"
#include "vector.h"

// A file containing a bunch of helper functions for making vector lists of certain shapes.

/**
 * Creates a list of vectors in a regular star shape at the origin oriented up.
 *
 * @param center Vector representing the symmetric center of the shape.
 * @param point_num Number of outer points.
 * @param outer_r Radius of outer points (distance from center to outer).
 * @param inner_r Radius of inner points (distance from center to inner).
 *
 * @return list of vectors of points in counter clockwise direction.
 */
list_t *make_shape_star(vector_t center, size_t point_num, double outer_r, double inner_r);

/**
 * Creates a list of vectors in a pacman shape at the provided center
 * 
 * @param angle 1/2 angle of pacman's "pie" cutout
 * @param radius pacman's radius
 * @param centroid pacman's center
 * @param n number of points on the "circle" from one edge of the cutout to the other 
*/
list_t *make_shape_pacman(double angle, double radius, vector_t centroid, size_t n);

/**
 * Creates a list of vectors in a circle shape with a provided center
 * 
 * @param radius circle's radius
 * @param centroid circle's center
 * @param n number of points on the circle
*/
list_t *make_shape_circle(double radius, vector_t centroid, size_t n);

/**
 * Creates a list of vectors in a slice of a circle shape with a provided center
 * 
 * @param angle the total angle of the "slice" of the circle to be made
 * @param radius circle's radius
 * @param centroid circle's center
 * @param n number of points on the circle
*/
list_t *make_shape_part_circle(double angle, double radius, vector_t centroid, size_t n);

/**
 * Creates a list of vectors in an ellipse shape with a provided center
 * 
 * @param angle total angle of the shape being made from the center (2*M_PI for a full ellipse)
 * @param x_radius ellipse's radius in the x direction 
 * @param y_radius ellipse's radius in the y direction
 * @param centroid ellipse's center
 * @param n number of points on the ellipse
*/
list_t *make_shape_ellipse(double angle, double x_radius, double y_radius, vector_t centroid, size_t n);

list_t *make_shape_rectangle(double width, double height, vector_t centroid);
#endif