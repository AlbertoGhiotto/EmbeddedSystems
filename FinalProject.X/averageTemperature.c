/*
 * File:   averageTemperature.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 16:03
 */


#include "xc.h"
#include "buffers.h"
#include "global.h"
#include "lcd.h"


int averageTemperature(void) {
    // Compute average of last 10 temperature readings contained in the buffers
    for(int i = 0 ; i< TEMPBUFFDIM; i++)
    {
    avgTemp = avgTemp + temp[i];
    }
    avgTemp = avgTemp / TEMPBUFFDIM;
    
    // Send data to pc
    
    // Print data on LCD
    sprintf(printTemp, "%.2f", avgTemp);
    printStringToLCD(printTemp);
    
    // Reset average
    avgTemp = 0;
    
    return 0;
}
