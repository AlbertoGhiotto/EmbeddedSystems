#ifndef XC_HEADER_BUTTONS_H
#define	XC_HEADER_BUTTONS_H

// S5 button ISR
void __attribute__((__interrupt__, _auto_psv_)) _INT0Interrupt();
// S6 buttons ISR
void __attribute__((__interrupt__, _auto_psv_)) _INT1Interrupt();
// Debouncing timer ISR
void __attribute__((__interrupt__, _auto_psv_)) _T2Interrupt();
// Debouncing timer
void tmr3_setup_period(int ms);

// Function enabling flag of buttons
void buttonSetup();

#endif	/* XC_HEADER_BUTTONS_H */

