/*
 * File:   scheduler.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 25 December 2019, 18:52
 */

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

void init_heartbeat_n(){
    // Set n = 0. The scheduler will update it on the go.   
    schedInfo[0].n = 0;
    schedInfo[1].n = 0;
    schedInfo[2].n = 0;
    schedInfo[3].n = 0;
    schedInfo[4].n = 0;
    schedInfo[5].n = 0;
    
    // Heartbeat is = 100, hence: 
    // N[i] = period of task / heartbeat
    schedInfo[0].N = 1;     // UART                     10 Hz
    schedInfo[1].N = 1;     // Temp acquisition         10 Hz
    schedInfo[2].N = 10;    // Temp average and send     1 Hz
    schedInfo[3].N = 2;     // Feedback msg to pc        5 Hz 
    schedInfo[4].N = 5;     // Blinking leds              1 Hz ( 500ms on, 500ms off)
    schedInfo[5].N = 1;     // LCD update               10 Hz
}

void scheduler() {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if (schedInfo[i].n == schedInfo[i].N) {
            switch (i) {
                case 0:
                    fromUart();
                    break;
                case 1:
                    //acquireTemp();
                    break;
                case 2:
                    averageTemperature();
                    break;
                case 3:
                    sendMCFBK();
                    break;
                case 4:
                    blinkLed();
                    break;
                case 5:
                    lcdOutput();
                    break;
            }
            schedInfo[i].n = 0;
        }
    }
}