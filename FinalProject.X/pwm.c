/*
 * File:   pwm.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 25 December 2019, 13:11
 */

#include "xc.h"
#include "global.h"
#include "pwm.h"
#include "buffers.h"
//#include headers

void setPWM() { 
    int f_pwm = 1000; // The frequency must be 1 kHz
    //float T_pwm = 1/f_pwm;
    
    /*In the Free Running mode, the PWM time base counts
    upwards until the value in the Time Base Period register
    (PTPER) is matched.*/
    // PTCONbits.PTMOD = 0;
    
    PWMCON1bits.PEN2H = 1;  // Setup the PWM2H pin
    PWMCON1bits.PEN3H = 1;  // Setup the PWM3H pin
    
    // PTPER must fit in 15 bits -> <32767
    // PTER = Fcy/(f_pwm * PTMR_Prescaler) -1
    // Given our frequency requirement, we can set the prescaler at 1
    // With prescaler at 1, PTER = 1842
    
    int PTMR_Prescaler = 1;  // support variable for PTPER computation
    PTPER = Fcy / (f_pwm * PTMR_Prescaler) - 1; //1842
    
    PTCONbits.PTCKPS = 0;   // prescaler 1:1
    
    PTCONbits.PTEN = 1;     // Turns on the PWM time base module
    
    normalizeDC(&actualRPM1,&actualRPM2);
}

int updateRange(int min, int max){

    // Check if the provided values are between the safe range, are consistent 
    // values of min and max and that zero values are allowed
    if (max < min || min < MIN_PROP_VEL || max > MAX_PROP_VEL || (min <= 0 && max >= 0)){
        return 1; // Negative ack
    }
    // Update new values
    minRPM = min;
    maxRPM = max;
    
    return 0;
}

// Function to compute the duty cycle range
int normalizeDC(int* rpm1, int* rpm2){
    long int newPTPER = PTPER;

    *rpm1 = satRPM(*rpm1);  // Pass a int* when the declaration has a simple int?
    *rpm2 = satRPM(*rpm2);
    
    // Define the duty cycle
    dutyCycle1 = ((*rpm1 - MIN_DC) * 2 * newPTPER ) / (MAX_DC - MIN_DC);
    dutyCycle2 = ((*rpm2 - MIN_DC) * 2 * newPTPER ) / (MAX_DC - MIN_DC);
    // Assign it to the corresponding output pins
    PDC2 = dutyCycle1;
    PDC3 = dutyCycle2;
    
    return 0;
}

int satRPM(int rpm) {
    
    if(rpm > maxRPM) {
        rpm = maxRPM;
    } else if (rpm < minRPM) {
        rpm = minRPM;
    }
    return rpm;
}
