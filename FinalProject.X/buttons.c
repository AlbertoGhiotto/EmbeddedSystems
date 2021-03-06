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
void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt() {
    IFS0bits.INT0IF = 0;             // Reset interrupt flag
    IEC0bits.INT0IE = 0;             // Disable interrupt of button s5
    
    // Stop motors
    int rpm1 = 0;
    int rpm2 = 0;
    normalizeDC(&rpm1, &rpm2);
    actualRPM1 = 0;
    actualRPM2 = 0;
    
    // Set safe state -> halt
    board_state = STATE_SAFE;
    // Stop timeout mode timer
    TMR2 = 0;
    T2CONbits.TON = 0;
    IEC0bits.T2IE = 0;              // Disable interrupt for timeout mode
    IFS0bits.T2IF = 0;              // Set the flag = 0
    
    tmr3_setup_period(50);          // Start debouncing timer
}

// S6 buttons ISR
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(){
    IFS1bits.INT1IF = 0;            // Reset interrupt flag
    IEC1bits.INT1IE = 0;            // Disable interrupt of button s5
    
    // Toggle state of the flag
    flagS6 = (flagS6 + 1 ) % 2;     // If flag = 0 -> flag = 1. If flag = 1 -> flag = 0
    
    tmr3_setup_period(50);          // Start debouncing timer - previously was 15 but sometimes it still bounced
}

// Debouncing timer ISR
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt() {
    IFS0bits.T3IF = 0;              // Reset interrupt flag of timer 3
    
    IFS0bits.INT0IF = 0;            // Reset interrupt flag of button s5
    IEC0bits.INT0IE = 1;            // Enable interrupt of button s5
    
    IFS1bits.INT1IF = 0;            // Reset interrupt flag of button s6
    IEC1bits.INT1IE = 1;            // Enable interrupt of button s6
    
    T3CONbits.TON = 0;              // Stop debouncing timer
}

// Debouncing timer
void tmr3_setup_period(int ms){
    TMR3 = 0;                       // Reset timer counter
    Fcy = (Fosc / 4.0);
    PR3 = (Fcy) / 64.0 * (ms / 1000.0);

    T3CONbits.TCKPS = 0b10;         // Prescaler 1:64    -> up to a bit more than 2 seconds
    T3CONbits.TON = 1;              // Starts the timer!
}

// Function enabling flag of buttons
void setButton(){
    IEC0bits.T3IE = 1;              // Enable interrupt of debouncing timer t3
        
    IFS0bits.INT0IF = 0;            // Reset interrupt flag for S5 button
    IFS1bits.INT1IF = 0;            // Reset interrupt flag for S6 button
    IEC0bits.INT0IE = 1;            // Enable interrupt for S5 button
    IEC1bits.INT1IE = 1;            // Enable interrupt for S6 button
}
