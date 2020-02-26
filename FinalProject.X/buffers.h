#ifndef XC_HEADER_BUFFER_H
#define	XC_HEADER_BUFFER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define TEMPBUFFDIM 10      // Temperature has to be averaged among 10 values
#define TXBUFFDIM 100       // Transmission buffer

// float temp[TEMPBUFFDIM]  = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float temp[TEMPBUFFDIM]  = {0.0};       // Temperature buffer
int tempIndex = 0;

void writeOnTempBuff(float data);       // Function to write on temp buffer

#endif	/* XC_HEADER_BUFFER_H */