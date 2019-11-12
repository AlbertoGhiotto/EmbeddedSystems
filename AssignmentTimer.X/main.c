/*
 * File:   main.c
 * Author: Alberto
 *
 * Created on 08 October 2019, 11:28
 */

// DSPIC30F4011 Configuration Bit Settings

// 'C' source line config statements

// FOSC
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
1. The first function setups T1 to count for the specified amount of
milliseconds. The function should support values up to one
second (at least). The second function should use the T1 flag to
wait until T1 has expired.
2. Use the two above defined functions to blink D3 led at 1 Hz
frequency (500ms time on, 500ms off)  
*/

void tmr1_setup_period(int ms);
void tmr1_wait_period();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy;            // number of clocks in one second = 1,843,200 clocks for each second
 

//I need to support timer up to at leat 1 second -> I can put at max 65535 in a register 
// hence I set a prescaler of 64 -> 28800 clock step for each second -> up to a bit more than 2 seconds

int main() {
    // initialization code
    TRISBbits.TRISB0 = 0;   // set the led pin as output
    
    tmr1_setup_period(500);
    while (1) {
        // code to blink LED
        if(LATBbits.LATB0 == 1)
            LATBbits.LATB0 = 0;     
        else 
            LATBbits.LATB0 = 1;
        
        tmr1_wait_period();
        }
}

void tmr1_setup_period(int ms) {
    TMR1 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR1 = (Fcy)/64.0 * (ms/1000.0) ;  //must be function of PR1
    
    T1CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T1CONbits.TON = 1; // starts the timer!
}

void tmr1_wait_period()
{
    while(IFS0bits.T1IF == 0) //wait for the timer to finish
    {}
    
    IFS0bits.T1IF = 0; //set the timer flag to zero to be notified of a new event    
}