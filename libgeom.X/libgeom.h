/* 
 * File:   libgeom.h
 * Author: Dylan Bourgeois
 *
 */

#ifndef LIBGEOM_H
#define	LIBGEOM_H

#include "../libmot_ext.X/libmot_ext.h"
#include "../libmot_ext.X/const.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {SQUARE, RECTANGLE, TRIANGLE_EQUI, TRIANGLE_RECT, TRIANGLE_ISOC} shape_t;
typedef enum {CW, CCW} rot_direction_t;

/*Draws specified shape with default sizes*/
void e_motor_draw_shape(shape_t shape);

/********POLYGONS*********/
//n=3
/*Draws an equilateral triangle of side length = 'size'*/
void e_motor_draw_equi_triangle(dist_mm_t size);
/*Draws an isocele triangle of base length = 'size_base' and of leg size = 'size_leg'*/
void e_motor_draw_iso_triangle(dist_mm_t size_base, dist_mm_t size_leg);
/*Draws a right triangle with the following geometry :

    |\
    |  \
  a |    \ c
    |_     \
    |_|______\
        b

 * Note : If 'c' does not conform to Pythagore's condition for right triangles
 *        a new value of 'c' will be computed to make the triangle right.
 *
 */
void e_motor_draw_rect_triangle(dist_mm_t a, dist_mm_t b, dist_mm_t c);

//n=4
/*Draws a square of side length='size'*/
void e_motor_draw_square(dist_mm_t size);
/*Draws a rectangle of sides 'length' and 'width'*/
void e_motor_draw_rectangle(dist_mm_t length, dist_mm_t width);

//n > 2
/*Draws n-sided regular convex polygon with side length='size' */
void e_motor_draw_reg_polygon(int n, dist_mm_t size);

/********CURVES*********/
/*Draws the circle of radius='radius' in a CW manner*/
void e_motor_draw_circle(dist_mm_t radius);
/* Allows to specify if circle should be drawn CW or CCW.*/
void e_motor_draw_oriented_circle(dist_mm_t radius, rot_direction_t dir);
/*TODO
 * Allows to translate of radius before accomplishing circle
 */
void e_motor_draw_circle_translated(dist_mm_t radius);

/********CONTROL*********/
/*Initializes flags*/
void e_motor_init_flags(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LIBGEOM_H */

