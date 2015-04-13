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

void e_motor_draw_shape(shape_t shape);

/********POLYGONS*********/
//n=3
void e_motor_draw_equi_triangle(dist_mm_t size);
void e_motor_draw_iso_triangle(dist_mm_t size_base, dist_mm_t size_leg);
void e_motor_draw_rect_triangle(dist_mm_t a, dist_mm_t b, dist_mm_t c); //TODO: Give 2 lengths, deduce 3rd

//n=4
void e_motor_draw_square(dist_mm_t size);
void e_motor_draw_rectangle(dist_mm_t length, dist_mm_t width);

//n
void e_motor_draw_reg_polygon(int n, dist_mm_t size);

/********CURVES*********/
void e_motor_draw_circle(dist_mm_t radius);

/********CONTROL*********/
void e_motor_init_flags(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LIBGEOM_H */

