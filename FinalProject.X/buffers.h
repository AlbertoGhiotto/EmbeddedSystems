#ifndef XC_HEADER_BUFFER_H
#define	XC_HEADER_BUFFER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define TEMPBUFFDIM 10      // Temperature has to be averaged among 10 values
#define TXBUFFDIM 100       // Transmission buffer

// float temp[TEMPBUFFDIM]  = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//double temp[TEMPBUFFDIM];       // Temperature buffer declared and initialized
//int tempIndex;            // pd doesn't work!!!!

typedef struct {
    float temp[TEMPBUFFDIM];
    int tempIndex;
} temperatureBuffer;

typedef struct {
    int buffer[TXBUFFDIM];
    int readIndex;
    int writeIndex;
} circularBuffer;

// Function to init transmission buffer
void initBuffers(circularBuffer *buffer, temperatureBuffer *tempBuffer);

// Function to write on temp buffer
void writeOnTempBuff(float data);       

// Function to write on transmission buffer
void writeOnCircBuffer(circularBuffer *buffer, int data);
// Function to read content of transmission buffer
int readCircBuffer(circularBuffer *buffer, int *data);

// Function to compute size of buffer to be read
int sizeBuf(circularBuffer *buffer);

#endif	/* XC_HEADER_BUFFER_H */