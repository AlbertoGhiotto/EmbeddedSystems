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

#define TMIN 0.001
#define TMAX 0.002

void tmr1_setup_period(int ms);
void tmr1_wait_period();

void setupADC();

void printToLCD(char string[]);
void clearLCD();
void setLCD();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second

int main(void) {
    setupADC();
    setLCD();
    tmr1_setup_period(100);

    // Variables for reading ADC value
    float ADCPotValue;
    float temp;
    // Variables for printing on LCD
    char potent[16];
    char value[16];

    /*In the Free Running mode, the PWM time base counts
    upwards until the value in the Time Base Period register
    (PTPER) is matched.*/
    PTCONbits.PTMOD = 0;

    //PWMCON2bits.PWM2H = 1; 
    // Setup the PWM2H pin
    PWMCON1bits.PEN2H = 1;
    //T = 20ms -> f_PWM = 1/ 0.02
    float T = 0.02;
    float f_pwm = 1 / T; // 50 Hz
    // PTPER must fit in 15 bits -> <32767
    // PTER = Fcy/(f_pwm * PTMR_Prescaler) -1
    // With prescaler 1 PTER = 36683 -> Set prescaler at 4
    PTCONbits.PTCKPS = 1; // prescaler at 1:4
    // Could develop a function which return the prescaler value for the multiplication given the value of PTCKPS
    int PTMR_Prescaler = 4;

    PTPER = Fcy / (f_pwm * PTMR_Prescaler) - 1; //9215

    // Wait 1 second at startup
    tmr1_wait_period();

    PTCONbits.PTEN = 1; // Turns on the PWM time base module



    while (1) {
        ADCON1bits.SAMP = 1; //enable bit sampling 
        while (IFS0bits.ADIF == 0);
        // Read buffer value immediately after checking the done bit
        ADCPotValue = ADCBUF0;
        IFS0bits.ADIF = 0; // resetting DONE flag
        clearLCD();

        // Potentiometer
        ADCPotValue = ADCPotValue / 1023.0; // Get ADC value 
        sprintf(potent, "%f", ADCPotValue);
        // Print potentiometer on LCD's first row
        printToLCD("Potent = ");
        printToLCD(potent);

        //Define the duty cycle
        temp = (TMIN + (ADCPotValue) * (TMAX - TMIN));
        float dutyCycle = (temp / T) * 2 * PTPER;
        PDC2 = dutyCycle;
        sprintf(value, "%d", PDC2);

        // Print PDC2 value on LCD's second row
        //Move cursor to second row
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = 0xC0;
        printToLCD("Control = ");
        printToLCD(value);


        tmr1_wait_period();
    }


    return 0;
}

void setupADC() {
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
}

void setLCD() {
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8-bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    SPI1CONbits.SPRE = 6; // 6:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI
}

// Clear function clears both rows
void clearLCD() {
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80;

    int i;
    for (i = 0; i < 25; i++) { //should be 16 since there are only 16 columns
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    //Move cursor to second row
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0xC0;
    for (i = 0; i < 25; i++) { //should be 16 since there are only 16 columns
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    
    // Reposition cursor on first row
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