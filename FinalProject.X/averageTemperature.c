/*
 * File:   averageTemperature.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 16:03
 */
#include <stdio.h>
#include "xc.h"
#include "buffers.h"
#include "global.h"
#include "lcd.h"
#include "toUart.h"
#include "averageTemperature.h"

int averageTemperature(void) {
    int i;
    
    //  Support variable to store MCTEM message
    char pcMsg[15];
    
    // Reset average
    avgTemp = 0.0;
    // Compute average of last 10 temperature readings contained in the buffers
    for (i = 0; i <= TEMPBUFFDIM; i++) {
        avgTemp = avgTemp + tempBuffer.temp[i];
    }
    avgTemp = avgTemp / TEMPBUFFDIM;

    // Send data to pc
    sprintf(pcMsg,"MCTEM,%.2f",avgTemp);
    sendToPC(pcMsg);

    // Print data on LCD
    //sprintf(printTemp, "%f", avgTemp);
    //printToLCD("TEMP", TEM + 0x04); // Print right after the "TEM" word
    //printToLCD("TEM:", TEM);

  return 0;
}