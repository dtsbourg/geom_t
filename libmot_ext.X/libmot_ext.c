/*
 * File:   libmot_ext.c
 * Author: Dylan Bourgeois
 *
 */

#include "libmot_ext.h"
#include "const.h"
/****** /!\ Relative path /!\ ******/
#include "../motor_led/e_motors.h"
#include "../motor_led/e_init_port.h"
#include "../motor_led/e_epuck_ports.h"
/****** /!\ Relative path /!\ ******/

struct Motor {
    long step_max;
} motor_r, motor_l;

static int exec = 0;

void Init_T3()
{
    
    T3CON = TMR_INIT;
    T3CONbits.TCKPS = PRESCALER;

    TMR3 = TMR_INIT;
    
    PR3 = (1000);

    IFS0bits.T3IF = DISABLE; //Interrupt flag
    IEC0bits.T3IE = ENABLE;  //Interrupt enable
    T3CONbits.TON = TMR_ON;  //Start timer
}

void _ISR __attribute__((auto_psv)) _T3Interrupt (void)
{
    IFS0bits.T3IF = DISABLE;
    exec = !e_motor_should_stop();
}

void e_motor_go_to_position(position_t pos, speed_t speed)
{
    exec = 1;

    //Set initial step values
    e_set_steps_left(0);
    e_set_steps_right(0);

    //Set goals
    motor_l.step_max = pos.l;
    motor_r.step_max = pos.r;

    e_motor_set_speed(speed);

    //Wait until action has completed
    while (exec);
}

void e_motor_rotate(deg_t angle)
{
    int steps = (int) ((STEP_PER_MM*PI*E_PUCK_DIAM*ABS(angle))/360.0);
    
    position_t pos = ((position_t){ .l = steps, .r = steps });
    //Rotate clock-wise or counter clock-wise depending on angle's sign
    speed_t speed = (angle < 0) ? ROT_MID_CCW : ROT_MID_CW;
    
    e_motor_go_to_position(pos, speed);
}

void e_motor_move_dist(dist_mm_t dist, deg_t angle)
{
    unsigned int steps = (unsigned int) (dist*STEP_PER_MM);
    position_t pos = ((position_t){ .l = steps, .r = steps });
    speed_t speed = MID_F;
    
    if (angle) {
        e_motor_rotate(angle);
    }
    
    e_motor_go_to_position(pos,speed);
}

void e_motor_set_speed(speed_t speed)
{
    e_set_speed_left(speed.l);
    e_set_speed_right(speed.r);
}

void e_motor_stop(void)
{
    long i;

    T4CONbits.TON = 0;
    T5CONbits.TON = 0;

    MOTOR2_PHA = 0;
    MOTOR2_PHB = 0;
    MOTOR2_PHC = 0;
    MOTOR2_PHD = 0;

    LATA = LATA_ON;

    for(i=0;i<600000;i++)
        asm("nop");

    LATA = LATA_INIT;
}

int e_motor_should_stop(void)
{
    //Did epuck reach its step objective ?
    return ((ABS( e_get_steps_left()) >= ABS(motor_l.step_max))
         && (ABS(e_get_steps_right()) >= ABS(motor_r.step_max)));
}

void e_motor_init_dispatcher()
{
    Init_T3();
    e_init_motors();
}

//int main ()
//{
//    e_init_motors();
//    Init_T3();
//
//    e_motor_move_dist(100, 0);
//    e_motor_move_dist(100, 120);
//    e_motor_move_dist(100, 120);
//
//    e_motor_stop();
//
//    while (1) {}
//}