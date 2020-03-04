#ifndef XC_HEADER_GLOBAL_H
#define	XC_HEADER_GLOBAL_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MAX_PROP_VEL +8000      // Maximum allowed propeller velocity
#define MIN_PROP_VEL -8000      // Minimum allowed propeller velocity

// Timer variables
long int Fosc = 7372800; // 7.3728 MHz
long int Fcy  = 1843200; // Fosc / 4; // number of clocks in one second = 1,843,200 clocks for each second

// Board state variable: Controlled, Timeout, Halt(safe mode))
enum state{C, T, H};
enum state board_state;

// Board state definitions
#define STATE_CONTROLLED    0
#define STATE_TIMEOUT       1
#define STATE_SAFE          2

// RPM setting variables
int minRPM;
int maxRPM;
int actualRPM1;
int actualRPM2;

#endif	/* XC_HEADER_GLOBAL_H */

