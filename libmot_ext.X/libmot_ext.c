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

struct Control {
    int exec:1;
    int acc_enabled:1;
} mot_flags;

static speed_t speed_goal;
static speed_t current_speed;
static step_t acc_len = 0;
static int acc_state;


int e_motor_should_acc(speed_t current_speed)
{
    return ((ABS(current_speed.l) <= ABS(speed_goal.l))
         && (ABS(current_speed.r) <= ABS(speed_goal.r)));
}

int e_motor_should_dec(void)
{
    return ((ABS( e_get_steps_left()) >= ABS(motor_l.step_max) - acc_len)
         && (ABS(e_get_steps_right()) >= ABS(motor_r.step_max) - acc_len)
         && current_speed.l > 0
         && current_speed.r > 0);
}

//Prevents PR3 overflow
int e_motor_select_prescaler(float delta_time)
{
    if (delta_time >= 0.01)         return 3; //prescaler = 256; 57600 < PR3 < 576
    else if (delta_time >=   0.001) return 2; //prescaler = 64 ;  2304 < PR3 < 230
    else if (delta_time >=  0.0001) return 1; //prescaler = 8  ; 18432 < PR3 < 1843
    else if (delta_time >= 0.00001) return 0; //prescaler = 1  ;  1474 < PR3 < 147
    else return -1; //delta_time < 10 �s => error
}

void Init_T3(speed_t final_speed, float time)
{
    T3CON = TMR_INIT;
    TMR3 = TMR_INIT;
    
    if (!time) //Acceleration disabled
    {
        mot_flags.acc_enabled = ACC_DISABLE;
        T3CONbits.TCKPS = PRESCALER;

        PR3 = STD_CLK;
    } else { //Acceleration enabled
        
        mot_flags.acc_enabled=ACC_ENABLE;

        speed_goal = final_speed;   //Set speed goal
        current_speed = ZERO;       //Initialize current speed

        //Calculate timer frequencies
        float avg_speed = ( final_speed.l + final_speed.r ) / 2.0;
        float delta_time = time / avg_speed;

        int prescaler = e_motor_select_prescaler(delta_time);

        T3CONbits.TCKPS = prescaler;

        PR3 = (FCY/prescaler)*(delta_time/INCR);

        acc_len = avg_speed * time; //steps
    }

    IFS0bits.T3IF = DISABLE; //Interrupt flag
    IEC0bits.T3IE = ENABLE;  //Interrupt enable
    T3CONbits.TON = TMR_ON;  //Start timer
}


void _ISR __attribute__((auto_psv)) _T3Interrupt (void)
{
    IFS0bits.T3IF = DISABLE;
    
    if (mot_flags.acc_enabled) {
        if ((acc_state == 0) && e_motor_should_acc(current_speed)) {

            current_speed.l = current_speed.l + INCR;
            current_speed.r = current_speed.r + INCR;

            e_motor_set_speed(current_speed);
            
        } else if (acc_state == 0) {
            acc_state = 1;
        }

        if (e_motor_should_dec()) {
            acc_state = 2;
            
            current_speed.l = current_speed.l - INCR;
            current_speed.r = current_speed.r - INCR;

            e_motor_set_speed(current_speed);

        } else if (acc_state == 2) {
            mot_flags.exec = 0;
        }
    } else {
        mot_flags.exec = !e_motor_should_stop();
    }
}

void e_motor_go_to_position(position_t pos, speed_t speed)
{
    mot_flags.exec = 1;

    //Set initial step values
    e_set_steps_left(0);
    e_set_steps_right(0);

    //Set goals
    motor_l.step_max = pos.l;
    motor_r.step_max = pos.r;

    e_motor_set_speed(speed);

    //Wait until action has completed
    while (mot_flags.exec);
}

void e_motor_rotate(deg_t angle)
{
    step_t steps = (step_t) ((STEP_PER_MM*PI*E_PUCK_DIAM*ABS(angle))/360.0);
    
    position_t pos = ((position_t){ .l = steps, .r = steps });
    //Rotate clock-wise or counter clock-wise depending on angle's sign
    speed_t speed = (angle < 0) ? ROT_MID_CCW : ROT_MID_CW;
    
    e_motor_go_to_position(pos, speed);
}


void e_motor_move_dist(dist_mm_t dist, deg_t angle)
{
    step_t steps = (step_t) (dist*STEP_PER_MM);
    position_t pos = ((position_t){ .l = steps, .r = steps });
    speed_t speed = mot_flags.acc_enabled ? current_speed : MID_F;
    
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

    MOTOR1_PHA = 0;
    MOTOR1_PHB = 0;
    MOTOR1_PHC = 0;
    MOTOR1_PHD = 0;

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

void e_motor_init_dispatcher(int acc)
{
    if (acc) Init_T3(FAST_F,1);
    else Init_T3(ZERO, 0);
    
    e_init_motors();
}


int main ()
{
    e_motor_init_dispatcher(ACC_ENABLE);

    e_motor_move_dist(400, 0);

    e_motor_stop();

    while (1) {}
}