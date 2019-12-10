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

//void InitDevice();
// Timer functions
void tmr1_setup_period(int ms);
void tmr1_wait_period();

// LCD functions
void printChar(char element);
void printToLCD(char string[]);
void clearLCD();
void setLCD();

void scheduler();
void task1();
void task2();
void task3();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second

// char string[24] = "This is very long string";

// Define struct with string to print and relative index
struct sentences{
	char sentence[25];
	int index;
    int printedChar ;
};

struct sentences sentence = {"This is a very long string", 0, 0};

// Define the flag to print on LCD. Set it to print
int lcdFlag = 1;

int main() {
    setLCD();  // Setup LCD
    tmr1_setup_period(1000); // Wait 1 second at startup
    tmr1_wait_period();
    
    
    //tmr1_setup_period(5); // Init timer to work as the heartbeat. 5 ms 
    // Set the Ns -> Heartbeat is = 5 so: 
    // N1 = 1 N2 = 250/5 = 50; N3 = 500/5 = 100
    /*schedInfo[0].N = 1;
    schedInfo[1].N = 50;
    schedInfo[2].N = 100;
    // Set n = 0. The scheduler will update it on the go
    schedInfo[0].n = 0;
    schedInfo[1].n = 0;
    schedInfo[2].n = 0;*/
    
    // while (SPI1STATbits.SPITBF == 1); // wait until not full
     //SPI1BUF = sentence.sentence[sentence.index + sentence.printedChar];    
    clearLCD();
     //printChar("C");
     //printToLCD(sentence.sentence[sentence.index + sentence.printedChar]);
     printToLCD("Ciao");
    
    /*
    while (1) {
        scheduler();
        tmr1_wait_period();
    }*/
}


void scheduler() {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if (schedInfo[i].n == schedInfo[i].N) {
            switch (i) {
                case 0:
                    task1();
                    break;
                case 1:
                    task2();
                    break;
                case 2:
                    task3();
                    break;
                    
            }
            schedInfo[i].n = 0;
        }
    }
}

void task1() {
    if(lcdFlag) // Print to LCD if the flag is at 1
    {
    //print single element of sentence to lcd
    printToLCD(sentence.sentence[sentence.index + sentence.printedChar]);
    //sentence.index = sentence.index +  1 ;  // Increment index of string
    sentence.printedChar = sentence.printedChar + 1 ;  // Increment #printed char
    }
   
    if(sentence.printedChar == 15)
    {
    // Set the LCD priting flag to 0    
    lcdFlag = 0;
    // Reposition cursor on first row
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80;
    }
}

void task2() {
    if(!lcdFlag) // if lcdFlag is zero
    {
        lcdFlag = 1; // set it to 1
        sentence.index = sentence.index++;   // increment the sentence index to start printing the next letter
        if (sentence.index == 24)  // when it gets to 24 (the length of the string) it resets to zero
            sentence.index = 0;
    }
    
}

void task3() {
    // do something for TASK 3
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
    for (i = 0; i < 16; i++) { //should be 16 since there are only 16 columns
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    //Move cursor to second row
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0xC0;
    for (i = 0; i < 16; i++) { //should be 16 since there are only 16 columns
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    
    // Reposition cursor on first row
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0x80;
}

void printChar(char element) {    
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = element;
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


