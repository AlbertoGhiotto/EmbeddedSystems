#ifndef XC_HEADER_SCHEDULER_H
#define	XC_HEADER_SCHEDULER_H

#include <xc.h> // include processor files - each processor file is guarded.  

typedef struct {
    int n;  // Set n to 0, the scheduler will update it on the go
    int N;
} heartbeat;

void init_heartbeat_n();
void scheduler();

#endif	/* XC_HEADER_SCHEDULER_H */

