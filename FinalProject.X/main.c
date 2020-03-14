/*
 * File:   main.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 25 December 2019, 13:06
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
#include <stdio.h>
#include "xc.h"
#include "acquireTemperature.h"
#include "averageTemperature.h"
#include "blinkLeds.h"
#include "buffers.h"
#include "buttons.h"
#include "fromUart.h"
#include "global.h"
#include "lcd.h"
#include "parser.h"
#include "pwm.h"
#include "scheduler.h"
#include "sendMCFBK.h"
#include "timer.h"
#include "toUart.h"

// Custom enum variable to store board state
enum state board_state;

// RPM setting variables
int minRPM = MIN_PROP_VEL;      // Min allowed velocity
int maxRPM = MAX_PROP_VEL;      // Max allowed velocity
// Variables to store RPMs
int actualRPM1 = 0;             
int actualRPM2 = 0;

// Variable to store PWM duty cycle
double dutyCycle1 = 0.0;
double dutyCycle2 = 0.0;

// Circular buffer for UART transmission
circularBuffer transmissionBuffer;
// Buffer for temperature average
temperatureBuffer tempBuffer;
// Parser state variable
parser_state pstate;
// Scheduling info for scheduler
heartbeat schedInfo[MAX_TASKS];
// Flag for button S6
int flagS6;

// Frequency of board oscillator
long int Fosc = 7372800;
// Number of clocks per seconds
long int Fcy = 1843200;

// Variable to store computed average temperature
double avgTemp;

int main(void) {
    // All setup and initializations
    
    // Data structures
    initBuffers(&transmissionBuffer, &tempBuffer);
    setParser();
    init_heartbeat_n();
    
    // Peripherals
    setUART();
    setADC();
    setLCD();
    setButton();
    setLedPins();
    setPWM();
    
    // Print titles on LCD
    setTitles();
    
    // Set S6's flag as not pressed (0)
    flagS6 = S6NOTPRESSED;
    
    tmr1_setup_period(1000);            // Wait 1 second at startup for ADC
    tmr1_wait_period();

    int hBeat = 100;                    // Set heartbeat of scheduler to 100ms -> 10 Hz
    tmr1_setup_period(hBeat);           // Init timer to work as the heartbeat: 5 ms 

    tmr2_setup_period(5000);            // Init timer for timeout mode
    
    // loop
    while (1) {
        scheduler();
        tmr1_wait_period();
    }
    
    return 0;
}
