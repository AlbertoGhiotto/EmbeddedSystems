/*
 * File:   Ex3.c
 * Author:  Filippo Gandolfi    S4112879
 *          Alberto Ghiotto     S4225586
 *
 * Created on 25 November 2019, 16:01
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


#include "xc.h"
#include <stdio.h>
#include <string.h>

void tmr1_setup_period(int ms);
void tmr1_wait_period();

void printToLCD(char string[]);
void clearLCD();
void setLCD();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second

int main(void) {
    ADCON1bits.ASAM = 1; // Set automatic start
    ADCON1bits.SSRC = 7; //selects how the conversion should start (0 = manual, 7 = internal counter)
    ADCON2bits.CSCNA = 0;
    ADCON2bits.CHPS = 1; //selects the channels

    ADCON3bits.SAMC = 15; // sample time 15 Tad: how long the sampling should last [0 Tad - 31 Tad]
    //(matters only if SSRC = 7)

    ADCON3bits.ADCS = 63; //selects how long is one Tad [1/2 Tcy - 32 Tcy]

    // ADCHS: selects the inputs to the channels
    

    //connect AN2 as CH0 input
    ADCHSbits.CH0SA = 2;
    ADCHSbits.CH0NA = 0;
    
    //connect AN3 as CH0 input
    ADCHSbits.CH123SA = 1;
    ADCHSbits.CH123NA = 0;
    
    
    ADPCFGbits.PCFG2 = 0;
    ADPCFGbits.PCFG3 = 0; 


    ADPCFG = 0xFFFF; // Selects which pin should be used for A/D

    //ADPCFGbits.PCFG2 = 0; //connect AN2 as CH0 input? (same as ADCHS = 0x0002;)

    ADCON1bits.ADON = 1; // turns on the ADC module

    tmr1_setup_period(1000); // Wait 1 second at startup

    setLCD();

    char temp[16];
    char potent[16];
    int ADCPot, ADCTemp;
    float value;

    tmr1_wait_period();

    while (1) {
        ADCON1bits.SAMP = 1;
        while (ADCON1bits.DONE == 0);

        clearLCD();

        ADCPot = ADCBUF0; // get ADC value

        sprintf(potent, "%d", ADCPot);
        printToLCD(potent);

        tmr1_wait_period();

        ADCTemp = ADCBUF1; // get ADC value
        //Move cursor to second row
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = 0xC0;

        value = (((ADCTemp / 1024.0) * 5000) - 525)*0.1;

        sprintf(temp, "%2.2f", value);

        printToLCD(temp);

        tmr1_wait_period();
    }

    return 0;
}

void setLCD() {
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8-bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    SPI1CONbits.SPRE = 6; // 6:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI
}

void clearLCD() {
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80;

    int i;
    for (i = 0; i < 20; i++) {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80;
}

void printToLCD(char string[]) {
    int i;
    int n = strlen(string);

    for (i = 0; i < n; i++) {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = string[i];
    }
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
