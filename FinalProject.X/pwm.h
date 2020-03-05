#ifndef XC_HEADER_PWM_H
#define	XC_HEADER_PWM_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MIN_DC (-10000)    // Min value for each motor -> 0% duty cycle
#define MAX_DC (+10000)    // Max value for each motor -> 100% duty cycle

void setPWM();
int satRPM(int rpm);
int normalizeDC(int* rpm1, int* rpm2);
int updateRange(int min, int max);

#endif	/* XC_HEADER_PWM_H */

