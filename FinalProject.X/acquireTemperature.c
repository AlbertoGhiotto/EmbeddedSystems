/*
 * File:   acquireTemperature.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 12:59
 */

#include "xc.h"
#include "global.h"
#include "acquireTemperature.h"
#include "buffers.h"

#define VMIN 0.0                
#define VMAX 5.0
    
void acquireTemp() {
    // Variable to store temp and print it on temp buffer
    float degTemp;
    // Get temperature
    degTemp = getTemp();
    // Save temp on buffer to be averaged every 1 hz
    writeOnTempBuff(degTemp);
    return;
}

float getTemp(){
    // Variables for reading ADC value
    float ADCTempValue = 0;
    // Variable for doing normalization and conversion
    float degTemp;
   
    //ADCON1bits.SAMP = 1; //enable bit sampling (only when using manual start) 
    while (IFS0bits.ADIF == 0);    // Wait for the conversion to finish (done bit)
    IFS0bits.ADIF = 0;             // Reset the done bit 
    ADCTempValue = ADCBUF0;        // Read buffer value immediately after checking the done bit 
    
    ADCTempValue = (VMIN + ADCTempValue / 1023.0 * (VMAX - VMIN));  // Get ADC value already normalized
    degTemp = 25 + ((ADCTempValue * 1000) - 750) / 10;              // Convert the value in celsius degree
    
    return degTemp;
}

void setADC(){
    ADCON3bits.ADCS = 63;   // Selects how long is one Tad [1/2 Tcy - 32 Tcy]
    //ADCON1bits.ASAM = 0;    // Set manual start
    ADCON1bits.ASAM = 1;    // Set auto start
    
    ADCON1bits.SSRC = 7;    // Selects how the conversion should start (0 = manual, 7 = internal counter)

    ADCON2bits.CHPS = 0;    // Selects the channels

    ADCON3bits.SAMC = 15;   // Sample time 15 Tad: how long the sampling should last [0 Tad - 31 Tad]
                            //(matters only if SSRC = 7)
    
    // ADCHS: selects the inputs to the channels - link CH0 to AN3 for temp
    ADCHSbits.CH0NA = 0;
    ADCHSbits.CH0SA = 3;

    ADPCFG = 0xFFFF;        // Selects which pin should be used for A/D
    
    ADPCFGbits.PCFG3 = 0;   // Connect AN3 as CH0 input

    ADCON1bits.ADON = 1;    // Turns on the ADC module
}