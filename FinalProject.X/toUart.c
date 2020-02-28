/*
 * File:   toUart.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 27 December 2019, 15:26
 */

#include "xc.h"
#include "global.h"
#include "buffers.h"
#include "toUart.h"

void setUART() {
    U2BRG = 11; // (7372800 / 4) / (16 * 9600) - 1
     //U2BRG = 115200;
    U2MODEbits.UARTEN = 1;      // Enable UART
    U2STAbits.UTXEN = 1;        // Enable U2TX (must be after UARTEN)
    // U2STAbits.URXEN = 1; // enable U2RX (must be after UARTEN)
    
    IEC1bits.U2RXIE = 1;        // Enable rx interrupt for UART
    // ----------COMMENT Interrupt arrives when UART2 receiver is 3/4 full 
    U2STAbits.URXISEL = 0b10;   //  0b01 for data word transmission
}

void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt () {
    IFS1bits.U2RXIF = 0;                            // Reset rx interrupt flag
    int val = U2RXREG;                              // Read from rx register
    writeOnCircBuffer(&transmissionBuffer, val);    // Save value in buffer
}

int sendToPC(char* msg){
    
    char sendMsg[sendDIM];
    
    sprintf(sendMsg, "$%s*" ,msg);      // $ and * are required from msgs
    
    for (int i = 0; i < sendDIM; i++){
        if(U2STAbits.UTXBF == 0)    // If buffer is not full
            U2TXREG = sendMsg[i];
    }
        
    while(U2STAbits.UTXBF == 1){}     // Wait for space in buffer
    
    U2STAbits.OERR = 0;               // Reset buffer overrun error
    
}