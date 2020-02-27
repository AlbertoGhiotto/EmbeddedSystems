/*
 * File:   buffers.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 15:52
 */

#include "xc.h"
#include "global.h"
#include "buffers.h"

void writeOnTempBuff(float data){
    temp[tempIndex] = data;
    tempIndex++;
}

void writeOnCircBuffer(circularBuffer *buffer, int data){
    buffer->buffer[buffer->writeIndex] = data;
    buffer->writeIndex++;
    if (buffer->writeIndex == TXBUFFDIM)
        buffer->writeIndex = 0;
}

void readCircBuffer(circularBuffer *buffer, int *data){
    IEC1bits.U2RXIE = 0; // Disable interrupt of UART
    if (buffer->readIndex == buffer->writeIndex){ // We've finished reading
        IEC1bits.U2RXIE = 1; // Enable interrupt of UART
        return 0;
    }
    *data = buffer->buffer[buffer->readIndex];
    buffer->readIndex++;
    if(buffer->readIndex == TXBUFFDIM)
        buffer->readIndex = 0;
    IEC1bits.U2RXIE = 1; // Enable interrupt of UART
    return 1;
}