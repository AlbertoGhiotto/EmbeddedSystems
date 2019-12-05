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
void printToLCD(char string[]);
void clearLCD();
void setLCD();

long int Fosc = 7372800; // 7.3728 MHz
long int Fcy; // number of clocks in one second = 1,843,200 clocks for each second


int main() {
    //InitDevices();
    tmr1_setup_period(1000); // Init timer to wait 1 second at startup
    // Set the N 
    //Heartbeat is = 5 so 
    // N1 = 1 N2 = 250/5 = 50; N3 = 500/5 = 100
    // Set n =0 and update it on the go after
    setLCD();  // Setup LCD
    // ...
    while (1) {
        scheduler();
        tmr1_wait_period();
    }
}


void scheduler() {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i ].n++;
        if (schedInfo[i ].n == schedInfo[i].N) {
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
    // do something for TASK 1
}

void task2() {
    // do something for TASK 2
}

void task3() {
    // do something for TASK 3
}



