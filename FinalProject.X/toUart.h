#ifndef XC_HEADER_TOUART_H
#define	XC_HEADER_TOUART_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Setup uart
void setUART();

// Send to PC
int sendToPC(char* msg);

// ISR 
void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt ();

#endif	/* XC_HEADER_TOUART_H */

