#ifndef XC_HEADER_GLOBAL_H
#define	XC_HEADER_GLOBAL_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "buffers.h"
#include "parser.h"
#include "scheduler.h"

#define MAX_PROP_VEL +8000      // Maximum allowed propeller velocity
#define MIN_PROP_VEL -8000      // Minimum allowed propeller velocity

#define MAX_TASKS 6 // Define the number of task to perform

// Board state definitions
#define STATE_CONTROLLED    0
#define STATE_TIMEOUT       1
#define STATE_SAFE          2

// Timer variables
extern long int Fosc;// = 7372800; // 7.3728 MHz
extern long int Fcy; // = 1843200; // Fosc / 4; // number of clocks in one second = 1,843,200 clocks for each second

// Board state variable: Controlled, Timeout, Halt(safe mode))
enum state{C, T, H};
extern enum state board_state;

// RPM setting variables
extern int minRPM;
extern int maxRPM;
extern int actualRPM1;
extern int actualRPM2;

extern double dutyCycle1;
extern double dutyCycle2;

extern temperatureBuffer tempBuffer;           // Temperature buffer
extern circularBuffer transmissionBuffer;      // Transmission buffer

// Flag s6 pressed
extern int flagS6;

extern parser_state pstate;                     // Parser state

extern heartbeat schedInfo[MAX_TASKS];          // Scheduling info for scheduler

#endif	/* XC_HEADER_GLOBAL_H */

