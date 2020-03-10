/*
 * File:   blinkLeds.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 12:08
 */

#include "xc.h"
#include "global.h"
#include "blinkLeds.h"

void setLedPins()
{
    // Set the led pins as output
    TRISBbits.TRISB0 = 0;   // D3   
    TRISBbits.TRISB1 = 0;   // D4
}

void blinkLed() {
    // Blink led D3
    if(LATBbits.LATB0 == 1)
        LATBbits.LATB0 = 0;     
    else 
        LATBbits.LATB0 = 1;
    
    /*if(board_state == STATE_TIMEOUT)        // If board is in timeout state
    {   // Blink led D4
        if(LATBbits.LATB1 == 1) 
            LATBbits.LATB1 = 0;     
        else 
            LATBbits.LATB1 = 1;
    }
    else                        // If board is NOT in timeout state
        LATBbits.LATB1 = 0;     // Switch off led D4 
    //return 0;*/
}