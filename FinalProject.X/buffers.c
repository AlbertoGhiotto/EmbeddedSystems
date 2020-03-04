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

void initTxBuffer(circularBuffer *buffer) {
    //buffer->buffer = struct content ( actual buffer content );
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
}

int sizeBuf(circularBuffer *buffer) {
    // The dimension is the area to be still read. 
    // Hence the one from the readIndex to the WriteIndex.
    /*
     * |-------TXBUFFDIM-------|
     * 
     * [=======R------>W=======]
     * |-------|SIZEBUF|-------|
     * 
     * [------>W=========R-----]
     * |SIZEBUF|---------|SIZEB|
     * 
     * "="   is the content of buffer
     * "-->" is the area to be still read -> sizeBuf
     */
    if (buffer->writeIndex >= buffer->readIndex) {
        return (buffer->writeIndex - buffer->readIndex);
    } else {
        return (TXBUFFDIM - (buffer->readIndex - buffer->writeIndex));
    }
    return -1; // Something went wrong
}

void writeOnTempBuff(float data) {
    temp[tempIndex] = data;
    tempIndex++;
}

void writeOnCircBuffer(circularBuffer *buffer, int data) {
    buffer->buffer[buffer->writeIndex] = data;
    buffer->writeIndex++;
    if (buffer->writeIndex == TXBUFFDIM)
        buffer->writeIndex = 0;
}

int readCircBuffer(circularBuffer *buffer, int *data) {
    IEC1bits.U2RXIE = 0; // Disable interrupt of UART
    if (buffer->readIndex == buffer->writeIndex) { // We've finished reading
        IEC1bits.U2RXIE = 1; // Enable interrupt of UART
        return 0;
    }
    *data = buffer->buffer[buffer->readIndex];
    buffer->readIndex++;
    if (buffer->readIndex == TXBUFFDIM)
        buffer->readIndex = 0;
    IEC1bits.U2RXIE = 1; // Enable interrupt of UART
    return 1;
}