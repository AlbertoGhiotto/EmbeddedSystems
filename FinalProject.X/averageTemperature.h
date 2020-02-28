#ifndef XC_HEADER_AVGTEMP_H
#define	XC_HEADER_AVGTEMP_H

#include <xc.h> // include processor files - each processor file is guarded.  

int averageTemperature(void);

// Variable to store computed average temperature
float avgTemp = 0;

//  Support variable to store MCTEM message
char pcMsg[15];

// Support variable to print on LCD
char printTemp[4];

#endif	/* XC_HEADER_AVGTEMP_H */

