/*
 * File:   newmainXC16.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 05 December 2019, 11:06
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

#define MAX_TASKS 3

typedef struct {
    int n;
    int N;
} heartbeat;

heartbeat schedInfo[MAX_TASKS];

// Timer functions
void tmr1_setup_period(int ms);
void tmr1_wait_period();

// LCD functions
void printChar(char element);
void setLCD();

// Timer for s5 deboucing
void tmr2_setup_period(int ms);

void scheduler();
void task1();
void task2();
void task3();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second

// Define struct with string to print and relative index
struct sentences {
    char sentence[26];
    int index;
    int printedChar;
};

struct sentences sentence1 = {"This is a very long string", 0, 0};

// Define the flag to print on LCD. Set to 1 to print
int lcdFlag = 1;
// Define the flag for the s5 button interrupt
int s5Flag = 1; // 1 means not pressed -> sliding enabled
// Variables for sliding motion
int spacesToPrint = 0;
int spaces = 0;


int main() {
    TRISBbits.TRISB0 = 0; // set the led D3 pin as output
    TRISBbits.TRISB1 = 0; // set the led D4 pin as output
    
    setLCD(); // Setup LCD
    tmr1_setup_period(1000); // Wait 1 second at startup
    tmr1_wait_period();

    tmr1_setup_period(5); // Init timer to work as the heartbeat: 5 ms 
    // Set the Ns -> Heartbeat is = 5 so: 
    // N1 = 1 N2 = 250/5 = 50; N3 = 500/5 = 100
    schedInfo[0].N = 1;
    schedInfo[1].N = 50;
    schedInfo[2].N = 100;
    // Set n = 0. The scheduler will update it on the go
    schedInfo[0].n = 0;
    schedInfo[1].n = 0;
    schedInfo[2].n = 0;
    
    IEC0bits.INT0IE = 1; // Enable interrupt of button s5
    
    while (1) {
        scheduler();
        tmr1_wait_period();
    }

    return 0;
}

void scheduler() {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if (schedInfo[i].n == schedInfo[i].N) {
            switch (i) {
                case 0:
                    task1(); // Write current character to LCD
                    break;
                case 1:
                    if (s5Flag)
                    task2(); // Slide controller
                    break;
                case 2:
                    task3(); // Blink led
                    break;
            }
            schedInfo[i].n = 0;
        }
    }
}

void task1() {
    if( spaces > 0 && lcdFlag)
    {
        printChar(' ');
        spaces = spaces - 1 ;
        //sentence1.printedChar = sentence1.printedChar + 1; // Increment # of printed char
    }
    else if (lcdFlag) // Print to LCD if the flag is at 1
    {
        if((sentence1.index + sentence1.printedChar) > 25)  // This check avoids to address not initialized memory areas when sliding
           printChar(' ');
        else
            printChar(sentence1.sentence[sentence1.index + sentence1.printedChar]); // Print single element of sentence to LCD
        // Sentence.index = sentence.index +  1 ;  // Increment index of string
        sentence1.printedChar = sentence1.printedChar + 1; // Increment # of printed char
    }
    if ((sentence1.printedChar + spacesToPrint) == 16) { 
        // Set the LCD printing flag to 0    
        lcdFlag = 0;
        // Reset number of printed char
        sentence1.printedChar = 0;
        // Reposition cursor on first row
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = 0x80;
    }
}

void task2() {
    if (!lcdFlag) // if lcdFlag is zero
    {
        lcdFlag = 1; // set it to 1 -> enable task 1 to print
        
        // If there are no more spaces to be printed it means we have to start sliding the sentence out from the left
        if(spacesToPrint == 0)               
        sentence1.index = sentence1.index++; // increment the sentence index to start printing the next letter
        
        if(spacesToPrint > 0)   // If there are still spaces to be printed but the printing flag is at zero
        spacesToPrint = spacesToPrint - 1 ;     // We have to slide the sentence to the left -> put one space less
        
        if (sentence1.index == 26) // When it gets to 26 (the length of the string) it resets to zero the sentence index
        {
            sentence1.index = 0;
            spacesToPrint = 15; // Set 15 spaces to make the "T" appears as soon as the "g" leaves the LCD
        }
        spaces = spacesToPrint;  // Set # of spaces to print for task1
    }
}

void task3() {
    // code to blink LED D3
    if (LATBbits.LATB0 == 1)
        LATBbits.LATB0 = 0;
    else
        LATBbits.LATB0 = 1;
}

// S5 button ISR
void __attribute__((__interrupt__, _auto_psv_)) _INT0Interrupt() {
    IFS0bits.INT0IF = 0; // reset interrupt flag
    
    if (s5Flag == 0)
        s5Flag = 1;
    else
        s5Flag = 0;
    
    IEC0bits.INT0IE = 0; // Disable interrupt of button s5
    tmr2_setup_period(15);   // Start debouncing timer
}

void setLCD() {
    SPI1CONbits.MSTEN = 1; // master mode
    SPI1CONbits.MODE16 = 0; // 8-bit mode
    SPI1CONbits.PPRE = 3; // 1:1 primary prescaler
    SPI1CONbits.SPRE = 6; // 6:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI
}

// Single char print to LCD
void printChar(char element) {
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = element;
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

// Debouncing timer
void tmr2_setup_period(int ms)
{
    TMR2 = 0; // reset timer counter
    Fcy = (Fosc / 4.0);
    PR2 = (Fcy) / 64.0 * (ms / 1000.0); //must be function of PR1

    T2CONbits.TCKPS = 0b10; // prescaler 1:64    -> up to a bit more than 2 seconds
    T2CONbits.TON = 1; // starts the timer!
    IEC0bits.INT0IE = 1; // Re - enable interrupt of button s5
}
