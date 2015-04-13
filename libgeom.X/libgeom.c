/*
 * File:   libmot_ext.c
 * Author: Dylan Bourgeois
 *
 */

#include "libgeom.h"
#include <math.h>

#define PYTHAGORE_COND(a,b,c) (a*a + b*b == c*c)
#define PYTHAGOREAN_TRIPLE 104, 153, 185 //Verfies PYTAHGOREAN_COND
#define ASSOCIATED_ANGLE(d) (180-d)

#define DEFAULT_LEN 50                   //Default debug value
#define RIGHT_ANGLE 90                   //Right angle = 90 deg

struct Control {
    int reset_dir:1; //Should e_puck rotate back to original orientation
} flags;

void e_motor_draw_shape(shape_t shape)
{
    switch (shape)
    {
        case SQUARE:
            e_motor_draw_square(DEFAULT_LEN * 2);
            break;
        case RECTANGLE:
            e_motor_draw_rectangle(DEFAULT_LEN, DEFAULT_LEN * 3);
            break;
        case TRIANGLE_EQUI:
            e_motor_draw_equi_triangle(DEFAULT_LEN * 2);
            break;
        case TRIANGLE_RECT:
            e_motor_draw_rect_triangle(PYTHAGOREAN_TRIPLE);
            break;
        case TRIANGLE_ISOC:
            e_motor_draw_iso_triangle(DEFAULT_LEN * 2, DEFAULT_LEN * 3);
            break;
        default:
            break;
    }
}

void e_motor_draw_square(dist_mm_t size)
{
    //Square = rectangle with length=width
    e_motor_draw_rectangle(size, size);
}

void e_motor_draw_rectangle(dist_mm_t length, dist_mm_t width)
{
    e_motor_move_dist(length,  0);
    e_motor_move_dist(width , RIGHT_ANGLE);
    e_motor_move_dist(length, RIGHT_ANGLE);
    e_motor_move_dist(width , RIGHT_ANGLE);
    
    if(flags.reset_dir) {
        e_motor_rotate(RIGHT_ANGLE);
    }
}

void e_motor_draw_iso_triangle(dist_mm_t size_base, dist_mm_t size_leg)
{
    deg_t theta =(deg_t) acos((size_base / (size_leg * 2.0)));
    deg_t phi = 2 * (deg_t) asin((size_base / (size_leg * 2.0)));

    e_motor_move_dist(size_base, 0);
    e_motor_move_dist(size_leg, ASSOCIATED_ANGLE(theta));
    e_motor_move_dist(size_leg, ASSOCIATED_ANGLE(phi));

    if (flags.reset_dir) {
        e_motor_rotate(ASSOCIATED_ANGLE(theta-phi));
    }
}

void e_motor_draw_equi_triangle(dist_mm_t size)
{
    e_motor_move_dist(size,   0);
    e_motor_move_dist(size, 120);
    e_motor_move_dist(size, 120);

    if (flags.reset_dir) {
        e_motor_rotate(120);
    }
}

void e_motor_draw_rect_triangle(dist_mm_t a, dist_mm_t b, dist_mm_t c)
{
    if (PYTHAGORE_COND(a,b,c))
    {
        e_motor_rect_triangle(a,b,c);
    } else {
        dist_mm_t computed_c = (dist_mm_t) sqrt(a*a + b*b);
        e_motor_rect_triangle(a,b,computed_c);
    }
}

void e_motor_rect_triangle(dist_mm_t a, dist_mm_t b, dist_mm_t c)
{
    deg_t theta = (deg_t) acos((float)b / (float)c);

    e_motor_move_dist(a,  0);
    e_motor_move_dist(b, RIGHT_ANGLE);
    e_motor_move_dist(c, ASSOCIATED_ANGLE(theta));

    if (flags.reset_dir) {
       e_motor_rotate(RIGHT_ANGLE-theta);
    }
}

void e_motor_draw_reg_polygon(int n, dist_mm_t size)
{
    int i;
    for (i=0;i<=n;i++)
    {
        deg_t theta_n = (deg_t) (n-2) * 180.0 / n ;
        e_motor_move_dist(size, ASSOCIATED_ANGLE(theta_n));
    }
}

void e_motor_draw_circle(dist_mm_t radius)
{
    e_motor_draw_oriented_circle(radius, CW);
}

void e_motor_draw_oriented_circle(dist_mm_t radius, rot_direction_t dir)
{
    float epsilon = (radius + E_PUCK_DIAM) / (float) radius; // (v_l / v_r)
    speed_t speed_cw  = {.l = epsilon * 200 , .r = 200};
    speed_t speed_ccw = {.l = 200 , .r = epsilon * 200};
    int steps = 2 * PI * radius * STEP_PER_MM;

    if (dir == CW) {
        e_motor_go_to_position(steps, speed_cw);
    } else {
        e_motor_go_to_position(steps, speed_ccw);
    }
}

void e_motor_init_flags(void)
{
    flags.reset_dir = 1;
}

int main ()
{
    e_motor_init_dispatcher();
    e_motor_init_flags();

    position_t pos = {.l = 1000, .r = 1000};
    speed_t speed = {.l = 100, .r = 400};
    
    e_motor_go_to_position(pos, speed);
   
    e_motor_stop();

    while (1) {}
}