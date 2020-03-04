/*
 * File:   button.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 27 December 2019, 11:21
 */

#include "xc.h"
#include "pwm.h"
#include "buttons.h"
#include "global.h"

// S5 button ISR
void __attribute__((__interrupt__, _auto_psv_)) _INT0Interrupt() {
    IFS0bits.INT0IF = 0; // reset interrupt flag
    IEC0bits.INT0IE = 0; // Disable interrupt of button s5
    
    // Stop motors
    int rpm1 = 0;
    int rpm2 = 0;
    normalizeDC(&rpm1, &rpm2);
    actualRPM1 = 0;
    actualRPM2 = 0;
    
    // Set safe state -> halt
    board_state = STATE_SAFE;
    
    tmr3_setup_period(15);   // Start debouncing timer
}

// S6 buttons ISR
void __attribute__((__interrupt__, _auto_psv_)) _INT1Interrupt(){
    IFS1bits.INT1IF = 0;        // reset interrupt flag
    IEC1bits.INT1IE = 0;        // Disable interrupt of button s5
    
    flagS6 = !flagS6;            // Toogle state of the flag
    
    tmr3_setup_period(15);       // Start debouncing timer 
}

// Debouncing timer ISR
void __attribute__((__interrupt__, _auto_psv_)) _T3Interrupt() {
    IFS0bits.T3IF = 0; // reset interrupt flag of timer 3
    
    IFS0bits.INT0IF = 0; // reset interrupt flag of button s5
    IEC0bits.INT0IE = 1; // Enable interrupt of button s5
    
    T3CONbits.TON = 0; // Stop debouncing timer
}

// Debouncing timer
void tmr3_setup_period(int ms){
    TMR3 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR3 = (Fcy) / 64.0 * (ms / 1000.0);

    T3CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T3CONbits.TON = 1; // starts the timer!
}

// Function enabling flag of buttons
void buttonSetup(){
    IEC0bits.T2IE = 1; // Enable interrupt of deboucing timer t2
    // To be filled
}
