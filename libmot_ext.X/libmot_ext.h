/* 
 * File:   libmot_ext.h
 * Author: Dylan
 *
 * An extension of the default motor library provided.
 *
 */

#ifndef LIBMOT_EXT_H
#define	LIBMOT_EXT_H

#ifdef	__cplusplus
extern "C" {
#endif

/***********SPEED***********/
#define FAST_F ((speed_t) { .l = 700, .r = 700 })
#define MID_F  ((speed_t) { .l = 400, .r = 400 })
#define SLOW_F ((speed_t) { .l = 100, .r = 100 })

#define FAST_B ((speed_t) { .l = -700, .r = -700 })
#define MID_B  ((speed_t) { .l = -400, .r = -400 })
#define SLOW_B ((speed_t) { .l = -100, .r = -100 })

#define ROT_MID_CW  ((speed_t) { .l =  400, .r = -400 })
#define ROT_MID_CCW ((speed_t) { .l = -400, .r =  400 })

#define ZERO ((speed_t) { .l = 0, .r = 0 })

/***********ACCELERATION***********/
#define ACC_ENABLE  1
#define ACC_DISABLE 0

#define STD_CLK 1000

#define INCR 5

/***********TYPE ALIAS***********/
typedef int dist_mm_t;
typedef int step_t;
typedef int deg_t;
typedef float rad_t;

/***********DATA STRUCTURES***********/
typedef struct Speed {
  int l;
  int r;
} speed_t;

typedef struct Position {
    step_t l;
    step_t r;
} position_t;

/***********API***********/

/*==Action sequence==*/
/*Rotate epuck by 'angle' degrees.*/
void e_motor_rotate(deg_t angle);
/*Move 'dist' mm after rotating a certain 'angle'*/
void e_motor_move_dist(dist_mm_t dist, deg_t angle);
/*Set motor speed*/
void e_motor_set_speed(speed_t speed);
/*Go to a given 'pos'ition at given 'speed'*/
void e_motor_go_to_position(position_t pos, speed_t speed);
/*Stop motors*/
void e_motor_stop(void);

/*==Control sequence==*/
/*Return 1 if motor has reached counter destination*/
int e_motor_should_stop(void);
/*Init dispatcher*/
void e_motor_init_dispatcher(int acc);

/*==Acceleration profile==*/
void e_motor_acc_enable(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LIBMOT_EXT_H */

