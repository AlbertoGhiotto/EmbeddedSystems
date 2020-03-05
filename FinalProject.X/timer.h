#ifndef XC_HEADER_TIMER_H
#define	XC_HEADER_TIMER_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Timer 1
void tmr1_setup_period(int ms);
void tmr1_wait_period();

// Timer 2
void tmr2_setup_period(int ms);
void tmr2_wait_period();
void tmr2_restart_timer();

// Timer 2 ISR
//void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt();

#endif	/* XC_HEADER_TIMER_H */

