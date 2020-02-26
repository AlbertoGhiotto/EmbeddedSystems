/*
 * File:   lcd.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 16:56
 */


#include "xc.h"
#include "lcd.h"
#include <string.h>

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
    char mander[5] = "STA: ";
    char meleon[5] = "TEM: ";
    char izard[5]  = "RPM: ";
    
    printToLCD(mander, STA);
    printToLCD(meleon, TEM);
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