/*
 * File:   newmainXC16.c
 * Author: Alberto
 *
 * Created on 01 October 2019, 11:43
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

int main(void) {




    /*// Turn on led D3
     TRISBbits.TRISB0 = 0;   // set the pin as output
     LATBbits.LATB0 = 1;     // set the pin high
     */


    //Read the button S5; while the button is pressed turn on the led
    //D3, otherwise turn it off.
    /* int pinValue;
    TRISBbits.TRISB0 = 0;    // set the led pin as output
    TRISEbits.TRISE8 = 1;   // set the button  pin as input
    while(1)
    {
    pinValue = PORTEbits.RE8;   //read pin value
    if (pinValue == 0 )         //button s5 is pressed
        LATBbits.LATB0 = 1;     // turn led on
    else
        LATBbits.LATB0 = 0;     // turn led off
    }*/


    //Whenever the button S5 is pressed, toggle the status of the led
    //D3 (once per button click).

    int pinValue, pinValueCorr, count;

    TRISBbits.TRISB0 = 0; // set the led pin as output
    TRISEbits.TRISE8 = 1; // set the button  pin as input
    //LATBbits.LATB0 == 0;    // initially set the led as off

    pinValue = PORTEbits.RE8; //read pin value
    while (1) {
        pinValueCorr = PORTEbits.RE8;
        if (pinValue - pinValueCorr == 1 && count > 10000) //fronte di discesa (button pressed)
        {
            if (LATBbits.LATB0 == 0)
                LATBbits.LATB0 = 1; // turn led on
            else
                LATBbits.LATB0 = 0; // turn led off
            
            count = 0;
        }

        pinValue = pinValueCorr; //read pin value
        
        if(count <= 10000)
            count++;
    }
    return 0;

}
