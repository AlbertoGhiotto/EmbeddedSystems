#ifndef XC_HEADER_ACQTEMP_H
#define	XC_HEADER_ACQTEMP_H

#include <xc.h> // include processor files - each processor file is guarded.  

void acquireTemp();
float getTemp();
void setupADC();

#endif	/* XC_HEADER_ACQTEMP_H */

