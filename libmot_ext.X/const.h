/* 
 * File:   const.h
 * Author: Dylan
 *
 * Defines the constants used in libmot_ext.
 *
 */

#ifndef CONST_H
#define	CONST_H

#ifdef	__cplusplus
extern "C" {
#endif

/***********CONVERSIONS***********/
#define STEPS_PER_ROTATION 1000    //steps
#define MM_PER_ROTATION    129     //mm

#define DEG_PER_STEP       0.36f   //deg
#define MM_PER_STEP        0.129f  //mm

#define STEP_PER_MM        7.75f   //steps

#define E_PUCK_DIAM        53      //mm

/***********UTILS***********/
#define ABS(n) (((n) < 0) ? -(n) : (n))
#define EUCL_DIST(x,y) (sqrt(x*x+y*y))

#define PI 3.1415

/***********LEDs***********/
#define LATA_INIT 0x0000
#define LATA_ON   0xf6c0


/***********DISPATCHER***********/
#define ENABLE  1
#define DISABLE 0

#define TMR_INIT 0
#define TMR_ON   1

#define PRESCALER 3


/***********DIRECTIONS***********/


#ifdef	__cplusplus
}
#endif

#endif	/* CONST_H */

