#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#define TRIG    PTC7
#define ECHG    PTC8

#define FLAGWAIT    0xFFFFF
extern void Ultrasonic(void);
#endif