/*
 * File:   Part2.c
 * Author:  Filippo Gandolfi    S4112879
 *          Alberto Ghiotto     S4225586
 *
 * Created on 24 October 2019, 10:52
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

/*
Blink D3 led at 1 Hz frequency (500ms time on, 500ms off)
without using interrupts; every time the button S5 is pressed,
toggle the led D4 using interrupts.
 */

void tmr1_setup_period(int ms);
void tmr2_setup_period(int ms);
void tmr1_wait_period();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second


//I need to support timer up to at leat 1 second -> I can put at max 65535 in a register 
// hence I set a prescaler of 64 -> 28800 clock step for each second -> up to a bit more than 2 seconds

int main() {
    // initialization code
    TRISBbits.TRISB0 = 0; // set the led D3 pin as output
    TRISBbits.TRISB1 = 0; // set the led D4 pin as output

    tmr1_setup_period(500);
    
   
    IEC0bits.INT0IE = 1; // Enable interrupt of button s5
    IEC0bits.T2IE = 1; // Enable interrupt of timer t2

    while (1) {
        // code to blink LED D3
        if (LATBbits.LATB0 == 1)
            LATBbits.LATB0 = 0;
        else
            LATBbits.LATB0 = 1;

        tmr1_wait_period();
    }
}

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
    if (LATBbits.LATB1 == 1)
        LATBbits.LATB1 = 0;
    else
        LATBbits.LATB1 = 1;
    
    IEC0bits.INT0IE = 0; // Disable interrupt of button s5
    tmr2_setup_period(15);   // Start debouncing timer
}

// Debouncing timer
void tmr2_setup_period(int ms)
{
    TMR2 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR2 = (Fcy) / 64.0 * (ms / 1000.0); //must be function of PR1

    T2CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T2CONbits.TON = 1; // starts the timer!
}


// Led temporization timer
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