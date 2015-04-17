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


/***********UTILS***********/
#define PYTHAGORE_COND(a,b,c) (a*a + b*b == c*c)
#define PYTHAGOREAN_TRIPLE 104, 153, 185 //Verfies PYTAHGOREAN_COND
#define RIGHT_ANGLE 90                   //Right angle = 90 deg
    
#define ASSOCIATED_ANGLE(d) (180-d)

#define DEG_2_RAD(d) d*PI/180.0
#define RAD_2_DEG(r) r*180.0/PI
    
#define EUCL_DIST(x,y) (sqrt(x*x+y*y))


/***********SHAPES***********/
typedef enum {SQUARE, RECTANGLE, TRIANGLE_EQUI, TRIANGLE_RECT, TRIANGLE_ISOC, CIRCLE} shape_t;
typedef enum {CW, CCW} rot_direction_t;

typedef struct Coordinates {
    dist_mm_t x;
    dist_mm_t y;
} coord_t;

/*! \brief Draws specified shape with default sizes
 *      \param shape : shape type to draw
 */
void e_motor_draw_shape(shape_t shape);

/********POLYGONS*********/
//n=3
/*! \brief Draws an equilateral triangle
 *      \param size : length of triangle's sides
 */
void e_motor_draw_equi_triangle(dist_mm_t size);

/*! \brief Draws an isocele triangle
 *      \param size_base : base length
 *      \param size_leg  : leg length
 */
void e_motor_draw_iso_triangle(dist_mm_t size_base, dist_mm_t size_leg);

/*! \brief Draws a right triangle
 *
 * Conventions for side denomination is as follows :

    |\
    |  \
  a |    \ c
    |_     \
    |_|______\
        b

 * \note : If 'c' does not conform to Pythagore's condition for right triangles
 *        a new value of 'c' will be computed to make the triangle right.
 *
 */
void e_motor_draw_rect_triangle(dist_mm_t a, dist_mm_t b, dist_mm_t c);

//n=4
/*! \brief Draws a square
 *      \param size : side length
 */
void e_motor_draw_square(dist_mm_t size);

/*! \brief Draws a rectangle
 *      \param length : rectangle length
 *      \param width  : rectangle height
 */
void e_motor_draw_rectangle(dist_mm_t length, dist_mm_t width);

//n > 2
/*! \brief Draws an n-sided regular convex polygon
 *      \param size : side length
 */
void e_motor_draw_reg_polygon(int n, dist_mm_t size);

/********CURVES*********/
/*! \brief Draws a circle
 *      \param radius : Circle radius
 *  \note : default behaviour is a CW rotation
 */
void e_motor_draw_circle(dist_mm_t radius);

/*! \brief Draws a circle with a given rotation direction
 *      \param size : Circle radius
 *      \param dir  : Rotation direction {CW, CCW}
 */
void e_motor_draw_oriented_circle(dist_mm_t radius, rot_direction_t dir);

/*! \brief Draws a curved trajectory
 *      \param dest : Coordinates of destination
 */
void e_motor_draw_curves(coord_t dest);

/*!
 * \brief Draws an oriented arc with params :
 *      \param radius : radius of the arc
 *      \param dir    : rotation direction (clockwise=CW, counterclockwise=CCW)
 *      \param alpha  : arc angle
 */
void e_motor_draw_oriented_arc_circle(dist_mm_t radius, rot_direction_t dir, rad_t alpha);



/********CONTROL*********/
/*! \brief Initializes flags
 *  \note : Default behaviour is
 *          \flag reset_dir 1 : After completing a closed shape, robot rotates to
 *                              recover initial position
 */
void e_motor_init_flags(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LIBGEOM_H */

