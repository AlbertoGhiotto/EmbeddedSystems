/*
 * File:   Part2.c
 * Author:  Filippo Gandolfi    S4112879
 *          Alberto Ghiotto     S4225586
 *
 * Created on 14 November 2019, 11:05
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
#include <string.h>

void tmr1_setup_period(int ms);
void tmr1_wait_period();

void printToLCD(char string[]);
void clearLCD();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second

//int n;

int main(void) {

    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8?bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    SPI1CONbits.SPRE = 3; // 5:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI

    U2BRG = 11; // (7372800 / 4) / (16  9600) ? 1
    // U2BRG = 9600; 
    U2MODEbits.UARTEN = 1; // enable UART
    U2STAbits.UTXEN = 1; // enable U2TX (must be after UARTEN)
    //  U2STAbits.URXEN = 1; // enable U2RX (must be after UARTEN)
    //U2BRG = 115200;

    tmr1_setup_period(1000); // Wait 1 second at startup
    tmr1_wait_period();

    printToLCD("Waiting for data");

    // Wait until there are some characters to be read on UART2 
    while (U2STAbits.URXDA == 0) {

    }

    //clearLCD();

    char c = U2RXREG;
    //printToLCD(c);
    // U2RXREG // CONTAINS RECEIVED CHAR
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = c;


    printToLCD("Data received");

    return 0;
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

    clearLCD();

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
