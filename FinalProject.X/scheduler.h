#ifndef XC_HEADER_SCHEDULER_H
#define	XC_HEADER_SCHEDULER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MAX_TASKS 6 // Define the number of task to perform

typedef struct {
    int n = 0;  // Set n to 0, the scheduler will update it on the go
    int N;
} heartbeat;

heartbeat schedInfo[MAX_TASKS];

void init_heartbeat_n();
void scheduler();

#endif	/* XC_HEADER_SCHEDULER_H */

