/*
 * File:   motorlib.c
 * Author: Dylan Bourgeois
 *
 * Created on March 21, 2015, 2:30 PM
 */

#include "motorlib.h"
#include "e_init_port.h"
#include "e_epuck_ports.h"
#include "stdarg.h"


//Motor R
//Init interrupt timer
void Init_T5(int conv_r) {
    T5CON = TMR_INIT;
    
    if(!conv_r) {
        motor_r.step_max=0;
        motor_r.counter++;
        return;                                 // avoid divide by 0
    }

    T5CONbits.TCKPS = PRESCALER;
    TMR5 = TMR_INIT;

    PR5 = (1000*MILLISEC)/(256.0*conv_r);
    
    IFS1bits.T5IF = DISABLE; //Interrupt flag
    IEC1bits.T5IE = ENABLE;  //Interrupt enable
    T5CONbits.TON = TMR_ON;  //Start timer
}

void _ISR _T5Interrupt (void)
{
    IFS1bits.T5IF = DISABLE;

    static int step_r = 0;

    if (epuck.direction)
    {
        if (!step_r)
        {
            MOTOR1_PHA = MOTOR1_PHA^1;
            MOTOR1_PHB = MOTOR1_PHB^1;
        } else {
            MOTOR1_PHC = MOTOR1_PHC^1;
            MOTOR1_PHD = MOTOR1_PHD^1;

        }
    } else {

        if (!step_r)
        {
            MOTOR1_PHA = MOTOR1_PHA^1;
            MOTOR1_PHB = MOTOR1_PHB^1;
        } else {
            MOTOR1_PHC = MOTOR1_PHC^1;
            MOTOR1_PHD = MOTOR1_PHD^1;
        }
    }

    step_r = step_r^1;
    motor_r.counter++;
    
    epuck.execute = !e_motor_should_stop();

}


//Motor L
//Init interrupt timer
void Init_T4(int conv_l) {
    T4CON = TMR_INIT;

    if(!conv_l) {
        motor_l.step_max=0;
        motor_l.counter++;
        return;                                  // avoid divide by 0
    }

    T4CONbits.TCKPS=PRESCALER;
    
    TMR4 = TMR_INIT;
    PR4 = (1000*MILLISEC)/(256.0*conv_l);

    IFS1bits.T4IF = DISABLE; //Interrupt flag
    IEC1bits.T4IE = ENABLE;  //Interrupt enable
    T4CONbits.TON = TMR_ON;  //Start timer
}

void _ISR _T4Interrupt (void)
{
    IFS1bits.T4IF = DISABLE;

    static int step_l = 0;

    if (epuck.direction)
    {
        if (!step_l)
        {
            MOTOR2_PHA = MOTOR2_PHA^1;
            MOTOR2_PHB = MOTOR2_PHB^1;
        } else {
            MOTOR2_PHC = MOTOR2_PHC^1;
            MOTOR2_PHD = MOTOR2_PHD^1;
        }
    } else {

        if (step_l)
        {
            MOTOR2_PHA = MOTOR2_PHA^1;
            MOTOR2_PHB = MOTOR2_PHB^1;
        } else {
            MOTOR2_PHC = MOTOR2_PHC^1;
            MOTOR2_PHD = MOTOR2_PHD^1;
        }
    }

    step_l = step_l^1;
    motor_l.counter ++;

    epuck.execute = !e_motor_should_stop();

}

//Motor Control
void e_motor_init (void)
{
    LATD = LATD_INIT;
}

int e_motor_should_stop(void)
{
    return  ((motor_r.counter >= motor_r.step_max)
          && (motor_l.counter >= motor_l.step_max));
}

int e_motor_is_turning(speed_t speed)
{
    return (speed.r * speed.l < 0);
}

void e_motor_speed_limit(speed_t *speed)
{
    if(speed->r < -MAX_VAL)
    {
        speed->r = -MAX_VAL;
    }
    else if(speed->r > MAX_VAL)
    {
        speed->r = MAX_VAL;
    }

    if(speed->l < -MAX_VAL)
    {
        speed->l = -MAX_VAL;
    }
    else if(speed->l > MAX_VAL)
    {
        speed->l = MAX_VAL;
    }
}

void e_motor_set_speed (speed_t speed)
{
    int convert_r, convert_l;

    e_motor_speed_limit(&speed);

    if(e_motor_is_turning(speed))
    {
        epuck.direction = LR;
    } else {
        epuck.direction = FB;
    }

    convert_r = ABS(speed.r)*STEPS_PER_ROTATION/(MAX_VAL+1);
    convert_l = ABS(speed.l)*STEPS_PER_ROTATION/(MAX_VAL+1);

    Init_T5(convert_r);
    Init_T4(convert_l);
}

void e_motor_set_position_counters(position_t pos)
{
    motor_r.step_max = pos.r;
    motor_l.step_max = pos.l;
}

void e_motor_go_to_position(position_t pos, speed_t speed)
{
    epuck.execute = GO;
    
    motor_r.counter = 0;
    motor_l.counter = 0;

    e_motor_set_position_counters(pos);
    e_motor_set_speed(speed);
    
    if ((!speed.r)&&(!speed.l))
    {
        epuck.execute = STOP;
    }

    while (epuck.execute);
}

void e_motor_read_position_counter(int *position_r, int *position_l)
{
    *position_r = motor_r.counter;
    *position_l = motor_l.counter;
}

void stop(void)
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

void e_motor_move_dist(dist_mm_t dist, direction_t dir)
{
    //TODO: Process LR directions
    int sgn = (dir == B) ? -1 : 1;
    int steps = (int) (sgn * dist * STEP_PER_MM);
    position_t pos = { .l = steps, .r = steps };

    e_motor_go_to_position(pos, FAST_F);
}

void e_motor_rotate(deg_t deg)
{
    //TODO: Calc angles
}

void e_motor_run_sequence(int len, ...)
{
    va_list valist;
    int i;

    va_start(valist, len);

    for (i=0; i < len; i++)
    {
        movement_t mov = va_arg(valist, movement_t);
        e_motor_move_dist(mov.dist, mov.dir);
        while(epuck.execute);
    }

    va_end(valist);
}

void e_init_struct (void)
{
    motor_r.counter  = 0;
    motor_r.step_max = 0;

    motor_l.counter  = 0;
    motor_l.step_max = 0;

    epuck.direction = FB;
    epuck.execute   = GO;

}

int main(void)
{
    long i;
    
    for (i=0;i<600000;i++) asm("nop");

    LATA = LATA_INIT;

    e_init_port();
    e_init_struct();
    e_motor_init();

    //movement_t p1 = (movement_t){ .dir = F, .dist = 50, .speed = FAST_F };
    //movement_t p2 = (movement_t){ .dir = L, .dist = 50, .speed = FAST_F };

    //e_motor_run_sequence(2, p1,p2);


    position_t origin = { .l=500 , .r=500 };
    position_t origin1 = { .l=1000 , .r=1000 };

    speed_t speed1 = {.l = -10, .r = 10};
    speed_t speed2 = {.l = 10, .r = -10};

    e_motor_go_to_position(origin, speed1);
    e_motor_go_to_position(origin1, speed2);
    stop();

    while(1)
    {

    }

}
