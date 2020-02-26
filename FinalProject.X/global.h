#ifndef XC_HEADER_GLOBAL_H
#define	XC_HEADER_GLOBAL_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MAX_PROP_VEL +8000      // Maximum allowed propeller velocity
#define MIN_PROP_VEL -8000      // Minimum allowed propeller velocity

// Timer variables
long int Fosc = 7372800; // 7.3728 MHz
long int Fcy = Fosc / 4; // number of clocks in one second = 1,843,200 clocks for each second

// Board state variable
enum state{HALT, TIMEOUT, CONTROLLED};
enum state board_state;

// RPM setting variables
extern int maxRPM;
extern int minRPM;
extern int actualRPM1;
extern int actualRPM2;

#endif	/* XC_HEADER_GLOBAL_H */

