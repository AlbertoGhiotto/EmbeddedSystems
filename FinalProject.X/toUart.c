/*
 * File:   toUart.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 27 December 2019, 15:26
 */

#include <string.h>
#include <stdio.h>
#include "xc.h"
#include "global.h"
#include "buffers.h"
#include "toUart.h"

void setUART() {
    // Baud Rate generator prescaler
    U2BRG = 11; // (7372800 / 4) / (16 * 9600) - 1  |   ( Fosc / 4) / (16 * Baud Rate)
   
    U2MODEbits.UARTEN = 1;      // Enable UART
    U2STAbits.UTXEN = 1;        // Enable U2TX (must be after UARTEN)
    
    IEC1bits.U2RXIE = 1;        // Enable rx interrupt for UART
    
    U2STAbits.URXISEL = 0b10;   //  0b01 for data word transmission -> 0b10 for longer words
}

void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt () {
    IFS1bits.U2RXIF = 0;                            // Reset rx interrupt flag
    int val = U2RXREG;                              // Read from rx register
    writeOnCircBuffer(&transmissionBuffer, val);    // Save value in buffer
}

int sendToPC(char* msg) {
    char sendMsg[sendDIM];
    int i = 0;

    sprintf(sendMsg, "$%s*", msg); // $ and * are required from msgs

    for (i = 0; i < sendDIM && sendMsg[i]!= '\0' ; i++) {
        if (U2STAbits.UTXBF == 0) { // If buffer is not full
            U2TXREG = sendMsg[i];
        }

        while (U2STAbits.UTXBF == 1) {
        } // Wait for space in buffer

        U2STAbits.OERR = 0; // Reset buffer overrun error
    }
    return 0;
}