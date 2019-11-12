/*
 * File:   newmainXC16.c
 * Author: Alberto
 *
 * Created on 12 November 2019, 10:19
 */
#pragma config FPR = XT                 // Primary Oscillator Mode (XT)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>

#include "xc.h"

void tmr1_setup_period(int ms);
void tmr2_setup_period(int ms);
void tmr1_wait_period();


long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second


int value = 0;  // Declare counter variable global in order to make it resettable from S5 button's ISR
int n;          // Declare counter dimension global for same reason

int main(void) {

    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8?bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    SPI1CONbits.SPRE = 3; // 5:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI
    /* while (SPI1STATbits.SPITBF == 1); // wait until not full
     //SPI1BUF = 'Hello '; */

    tmr1_setup_period(1000);

    IEC0bits.INT0IE = 1; // Enable interrupt of button s5
    IEC0bits.T2IE = 1; // Enable interrupt of timer t2

    tmr1_wait_period();

    
    int i;
    char string[11] = "Counter is:";
    n = sizeof (string);

    for (i = 0; i < n; i++) {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = string[i];
    }
    
    tmr1_setup_period(50);
    
    char temp[6];
    while (1) {
        tmr1_wait_period();
       
        // Attempt to deleting elements before rewriting 
       /* if (value % 10 == 0) {
            while (SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = 0x80 + n + 5;
            while (SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = ' ';
        }*/
        
        sprintf(temp, "%d", value);
        for (i = 0; temp[i] != '\0'; i++) {
            while (SPI1STATbits.SPITBF == 1); // wait until not full
            SPI1BUF = temp[i];
        }
        value++;
         // // Reposition cursor before rewriting
         while (SPI1STATbits.SPITBF == 1); // wait until not full
         SPI1BUF = 0x80 + n  ;
        }

    //while (1);

    return 0;
}

// Per cancellare dall'lcd mettere degli spazi nella posizione desidesrata


// Debouncing timer ISR
void __attribute__((__interrupt__, _auto_psv_)) _T2Interrupt() {
    IFS0bits.T2IF = 0; // reset interrupt flag
    
    IFS0bits.INT0IF = 0; // reset interrupt flag
    IEC0bits.INT0IE = 1; // Enable interrupt of button s5
    
    T2CONbits.TON = 0; // Stop debouncing timer
}

// S5 button ISR
void __attribute__((__interrupt__, _auto_psv_)) _INT0Interrupt() {
    IFS0bits.INT0IF = 0; // reset interrupt flag
    
    value = 0;  // Reset timer value
    int i;
    for (i = 0; i < n; i++)
    {
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80 + n + i  ;           // Reposition cursor
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = ' ' ;           // Delete eventual elements
    }
    
    IEC0bits.INT0IE = 0; // Disable interrupt of button s5
    tmr2_setup_period(15);   // Start debouncing timer
}

// Debouncing timer
void tmr2_setup_period(int ms) {
    TMR2 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR2 = (Fcy) / 64.0 * (ms / 1000.0); //must be function of PR1

    T2CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T2CONbits.TON = 1; // starts the timer!
}


void tmr1_setup_period(int ms) {
    TMR1 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR1 = (Fcy) / 64.0 * (ms / 1000.0); //must be function of PR1

    T1CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T1CONbits.TON = 1; // starts the timer!
}

void tmr1_wait_period() {
    while (IFS0bits.T1IF == 0) //wait for the timer to finish
    {
    }

    IFS0bits.T1IF = 0; //set the timer flag to zero to be notified of a new event    
}
