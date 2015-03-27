/* 
 * File:   motorlib.h
 * Author: Dylan Bourgeois
 *
 * Created on March 21, 2015, 2:30 PM
 */

#ifndef MOTORLIB_H
#define	MOTORLIB_H

#ifdef	__cplusplus
extern "C" {
#endif

/***********DEFINITIONS***********/
#ifndef CONVERSIONS
#define CONVERSIONS
    
#define STEPS_PER_ROTATION 1000    //steps
#define MM_PER_ROTATION    129     //mm
    
#define DEG_PER_STEP       0.36f   //deg
#define MM_PER_STEP        0.129f  //mm

#define STEP_PER_MM        7.75f   //steps
#endif


#ifndef LAT_VAL
#define LAT_VAL

#define LATD_INIT 0x0055

#define LATA_INIT 0x0000
#define LATA_ON   0xf6c0
#endif


#ifndef TIMERS
#define TIMERS

#define ENABLE  1
#define DISABLE 0

#define TMR_INIT 0
#define TMR_ON   1

#define PRESCALER 3
#endif
    
#ifndef UTILS
#define UTILS
    
#define ABS(n) (((n) < 0) ? -(n) : (n))

#define MAX_VAL 12
#endif


#ifndef SPEED
#define SPEED
#define FAST_F ((speed_t) { .l = 250, .r = 250 })
#define MID_F  ((speed_t) { .l = 150, .r = 150 })
#define SLOW_F ((speed_t) { .l =  10, .r =  10 })

#define FAST_B ((speed_t) { .l = -250, .r = -250 })
#define MID_B  ((speed_t) { .l = -150, .r = -150 })
#define SLOW_B ((speed_t) { .l = - 10, .r = - 10 })
#endif

/***********DATA STRUCTURES***********/
/*Directional enums*/
typedef enum {FB, LR} state_t;          // Going straight (FB) or turning (LR)
typedef enum {F, B, L, R} direction_t;  // Define up down left right
typedef enum {CW=1, CCW=-1} rot_dir_t;  // Define CW and CCW rotation signs
typedef enum {STOP, GO} execute_t;      // Define execution states for movement
typedef int dist_mm_t;                  // Type alias for distances
typedef int deg_t;                      // Type alias for degrees

/*Object Structs*/
struct Robot {
    state_t direction:1;
    unsigned execute:1;
} epuck;

struct Motor {
    long step_max;
    long counter;
} motor_r, motor_l;

/*Value Structs*/
typedef struct Speed {
    int l;
    int r;
} speed_t;

typedef struct Position {
    int l; //in steps
    int r; //in steps
} position_t;

/*Action structs*/
typedef struct Movement {
    dist_mm_t dist;
    speed_t speed;
    direction_t dir;
} movement_t;



/***********API***********/
/*Initialize motors*/
void e_motors_init (void);
/*Set motor speed*/
void e_motors_set_speed (speed_t speed);
/*Go to a given position*/
void e_motors_go_to_position(position_t pos, speed_t speed);
/*Read counters to position pointer*/
void e_motors_read_position(int *pos_l, int *pos_r);
/*Set position counters*/
void e_motors_set_position_counters(position_t pos);
/*Move 'dist' mm in a given direction*/
void e_motor_move_dist(dist_mm_t dist, direction_t dir);
/*Rotate epuck by 'deg' degrees. deg>0 => CW, deg<0 => CCW*/
void e_motor_rotate(deg_t deg);
/*Run a sequence of movements*/
void e_motor_run_sequence(int len, ...);

/*Control sequence*/
/*Return 1 if motor is turning*/
int e_motor_is_turning(speed_t speed);
/*Return 1 if motor has reached counter destination*/
int e_motor_should_stop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTORLIB_H */

