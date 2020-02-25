#ifndef XC_HEADER_PWM_H
#define	XC_HEADER_PWM_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MIN_ABS (-10000)    // Min value for each motor -> 0% duty cycle
#define MAX_ABS (+10000)    // Max value for each motor -> 100% duty cycle

void setupPWM ();
int satRPM(int rpm);
int refreshPWMvalue(int* n1, int* n2);
short int refreshPWMRange(int min, int max);
int sign(int x);

#endif	/* XC_HEADER_PWM_H */

