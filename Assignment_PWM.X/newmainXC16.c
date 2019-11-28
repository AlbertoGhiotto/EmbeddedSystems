/*
 * File:   newmainXC16.c
  * Author:   Filippo Gandolfi    S4112879
 *            Alberto Ghiotto     S4225586
 *
 * Created on 28 November 2019, 10:27
 */

//FOSC
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

/*Read the value of the potentiometer (AN2). Use the
potentiometer to create a PWM signal with a 20ms period and
with high pulse width between 1ms and 2ms. Configure the
PWM peripheral to create the PWM signal on the PWM2H pin*/

#include "xc.h"
#include <stdio.h>
#include <string.h>

#define VMIN 0.0
#define VMAX 5.0

void tmr1_setup_period(int ms);
void tmr1_wait_period();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second

int main(void) {
    
    
    // Variables for reading ADC value
    float ADCPotValue;
    // Variables for printing on LCD
    char potent[16];

    tmr1_wait_period();
    
    PTCON.PTEN = 1;
    PWMCON2.PWM2H = 1;
    //T = 20ms -> f_PWM = 1/ 0.02
    float T = 0.02;
    float f_pwm = 1 / T;
    // PTPER must fit in 15 bits -> <32767

    while (1) {
      
        // Potentiometer
        ADCPotValue = (VMIN + ADCBUF0 / 1023.0 * (VMAX - VMIN)); // Get ADC value already normalized
        sprintf(potent, "%.2f", ADCPotValue);
       
        tmr1_wait_period();
    }
    
    
    return 0;
}

void setupADC()
{
    ADCON1bits.ASAM = 1; // Set automatic start
    ADCON1bits.SSRC = 7; //selects how the conversion should start (0 = manual, 7 = internal counter)

    ADCON2bits.CHPS = 0; //selects the channels

    ADCON3bits.SAMC = 15; // sample time 15 Tad: how long the sampling should last [0 Tad - 31 Tad]
    //(matters only if SSRC = 7)

    ADCON3bits.ADCS = 63; //selects how long is one Tad [1/2 Tcy - 32 Tcy]

    // ADCHS: selects the inputs to the channels
    ADCHSbits.CH0NA = 0;
    ADCHSbits.CH0SA = 2;

    ADPCFG = 0xFFFF; // Selects which pin should be used for A/D

    ADPCFGbits.PCFG2 = 0; //connect AN2 as CH0 input

    ADCON1bits.ADON = 1; // turns on the ADC module

    tmr1_setup_period(1000); // Wait 1 second at startup
}



// Timer 1 setup function
void tmr1_setup_period(int ms) {
    TMR1 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR1 = (Fcy) / 64.0 * (ms / 1000.0); //must be function of PR1

    T1CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T1CONbits.TON = 1; // starts the timer!
}

// Temporization function using timer 1
void tmr1_wait_period() {
    while (IFS0bits.T1IF == 0) //wait for the timer to finish
    {
    }

    IFS0bits.T1IF = 0; //set the timer flag to zero to be notified of a new event    
}