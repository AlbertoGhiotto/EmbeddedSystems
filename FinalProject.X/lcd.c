/*
 * File:   lcd.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 16:56
 */


#include "xc.h"
#include "lcd.h"
#include "global.h"
#include "buttons.h"
#include <string.h>

void lcdOutput(){
    
    if (flagS6 == S6NOTPRESSED)        // When S6 is not clicked
    {
        clearLCD();
        setTitles();                    // TBD: optimize!!
        // First part of first row: board state.  Second part of first row is printed in averageTemperature.c
        switch (board_state){
            case STATE_CONTROLLED:
                printToLCD('C', STA + 0x04);
                break;
            case STATE_TIMEOUT:
                printToLCD('T', STA + 0x04);
                break;
            case STATE_SAFE:
                printToLCD('H', STA + 0x04);
                break;
        }
        // Second row: rpm
        sprintf(printf, "%d,%d", actualRPM1, actualRPM2);
        printToLCD(printf, RPM + 0x04);
    }
    else                // When S6 is clicked
    {
        clearLCD();
        // First row: saturation values
        sprintf(print, "SAT MIN: %d MAX: %d", minRPM, maxRPM )
        printToLCD(print, STA);
        
        // Second row: pwm duty cycle values
        sprintf(print, "RPM: %.2f,%.2f", dutyCycle1, dutyCycle2);
        printToLCD(print, RPM );
    }
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

void setTitles(){
    printToLCD(mander, STA);
    printToLCD(meleon, TEM);
    printToLCD(izard,  RPM);
}

void setTitlesS6(){
    printToLCD(izarditeX, STA);
    printToLCD(izarditeY, STA + );
    
    printToLCD(izard,  RPM);
}

void printToLCD(char string[], int position){
    int i;
    int n = strlen(string);
    
    // Reposition cursor where given by the parameter
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = position;
    
    for (i = 0; i < n; i++) {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = string[i];
    }
}