/*
 * File:   timer.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 25 December 2019, 19:32
 */

#include "xc.h"
#include "global.h"
#include "pwm.h"
#include <stdlib.h>

// Scheduler Timer - setup function
void tmr1_setup_period(int ms) {
    T1CONbits.TON = 0;      // Stops the timer
     
    TMR1 = 0;               // Reset timer counter
    PR1 = (Fcy) / 8.0 * (ms / 1000.0); 

    IFS0bits.T1IF = 0;      // Resets interrupt flag
    
    T1CONbits.TCKPS = 0b01; // Prescaler 1:8    -> up to a bit more than 2 seconds
    T1CONbits.TON = 1;      // Starts the timer!
}

// Timer 1 temporization function 
void tmr1_wait_period() {
    while (IFS0bits.T1IF == 0)  // Wait for the timer to finish
    {
    }
    IFS0bits.T1IF = 0;          // Set the timer flag to zero to be notified of a new event    
}

// Timeout mode timer - setup function
void tmr2_setup_period(int ms)
{
    T2CONbits.TON = 0;                      // Stops the timer
    
    TMR2 = 0;                               // Reset timer counter
    PR2 = (Fcy) / 256.0 * (ms / 1000.0); 
    
    // Set prescaler to 256. 7200 clock step for each second -> up to more than 5 seconds
    T2CONbits.TCKPS = 0b11;                 // Prescaler 1:256    
    T2CONbits.TON = 1;                      // Starts the timer!
    IEC0bits.T2IE = 1;                      // Enable interrupt of timer t2
}

// Timer 2 temporization function 
void tmr2_wait_period() {
    while (IFS0bits.T2IF == 0){              // Wait for the timer to finish
    }
    IFS0bits.T2IF = 0;                      // Set the timer flag to zero to be notified of a new event    
}

void tmr2_restart_timer(){
    T2CONbits.TON = 0;      // Stops the timer
    IEC0bits.T2IE = 1;      // Enable timer 2 interrupt
    TMR2 = 0;
    IFS0bits.T2IF = 0;      // Set the timer flag to zero to be notified of a new event
    T2CONbits.TON = 1;      // Starts the timer
}

// Timer 2 ISR - Set motor velocity to zeros and blink led D4
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt () {
    IEC0bits.T2IE = 0;         // Disable interrupt of timer t2
    // Set timeout state
    board_state = STATE_TIMEOUT;
    // Set motor velocity to zero
    int rpm1 = 0;
    int rpm2 = 0;
    normalizeDC(&rpm1, &rpm2);
    actualRPM1 = 0;
    actualRPM2 = 0;
    
    IFS0bits.T2IF = 0;          // Reset interrupt flag for timer 2
    T2CONbits.TON = 0;          // Stop the timer
    TMR2 = 0;                   // Reset the timer 
}