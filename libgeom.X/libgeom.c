/*
 * File:   libmot_ext.c
 * Author: Dylan Bourgeois
 *
 */

#include "libgeom.h"
#include "../motor_led/e_epuck_ports.h"
#include <math.h>



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
    float theta = acos((size_base / (size_leg * 2.0)));
    float phi = 2 * asin((size_base / (size_leg * 2.0)));

    e_motor_move_dist(size_base, 0);
    e_motor_move_dist(size_leg, ASSOCIATED_ANGLE(RAD_2_DEG(theta)));
    e_motor_move_dist(size_leg, ASSOCIATED_ANGLE(RAD_2_DEG(phi)));

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

void e_motor_rect_triangle(dist_mm_t a, dist_mm_t b, dist_mm_t c)
{
    float theta = acos((float)b / (float)c);

    e_motor_move_dist(a,  0);
    e_motor_move_dist(b, RIGHT_ANGLE);
    e_motor_move_dist(c, ASSOCIATED_ANGLE(RAD_2_DEG(theta)));

    if (flags.reset_dir) {
       e_motor_rotate(RIGHT_ANGLE+RAD_2_DEG(theta));
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

void e_motor_draw_reg_polygon(int n, dist_mm_t size)
{
    int i;
    for (i=0;i<n;i++)
    {
        deg_t theta_n = (deg_t) (n-2) * 180.0 / n ;
        e_motor_move_dist(size, ASSOCIATED_ANGLE(theta_n));
    }
}

void e_motor_draw_curves(coord_t dest)
{
    float radius = ( (dest.y*dest.y) / (2.0 * dest.x) ) + 0.5 * dest.x;
    float eucl_dist = EUCL_DIST(dest.x, dest.y);

    float alpha = 2.0 * asin(eucl_dist / (2.0 * radius));
    
    rot_direction_t dir;

    if (radius > 0) dir = CW;
    else dir = CCW;
    
    //quadrant 2 or 3
    if (dest.y < 0) alpha = 2*PI - alpha;
    //quadrant 1 or 4 => alpha
    
    e_motor_draw_oriented_arc_circle(ABS((dist_mm_t)radius),dir,alpha);
}

void e_motor_draw_oriented_arc_circle(dist_mm_t radius, rot_direction_t dir, float alpha)
{
    float epsilon = (radius + E_PUCK_DIAM) / (float) radius; // (v_l / v_r)
        
    speed_t speed_cw  = {.l = epsilon * 200 , .r = 200};
    speed_t speed_ccw = {.l = 200 , .r = epsilon * 200};

    int steps = alpha * radius * STEP_PER_MM;

    position_t pos = {.l=steps, .r=steps};

    if (dir == CW) {
      e_motor_go_to_position(pos, speed_cw);
    } else {
      e_motor_go_to_position(pos, speed_ccw);
    }
}

void e_motor_draw_oriented_circle(dist_mm_t radius, rot_direction_t dir)
{
    e_motor_draw_oriented_arc_circle(radius,dir,2*PI);
}

void e_motor_draw_circle(dist_mm_t radius)
{
    e_motor_draw_oriented_circle(radius, CW);
}

void e_motor_init_flags(void)
{
    flags.reset_dir = 1;
}

int main ()
{
    e_motor_init_dispatcher(0);
    e_motor_init_flags();

    //coord_t coord1 = (coord_t){.x = 100,.y = 50};
    //e_motor_draw_curves(coord1);
    coord_t coord2 = (coord_t){.x = 30, .y = -40};
    e_motor_draw_curves(coord2);
   
    e_motor_stop();

    while (1) {}
}