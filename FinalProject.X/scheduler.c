/*
 * File:   scheduler.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 25 December 2019, 18:52
 */

#include "xc.h"
#include "scheduler.h"
#include "global.h"

void init_heartbeat_n(){
    // Set n = 0. The scheduler will update it on the go. ------------------ Already taken care of in definition
    /*
    schedInfo[0].n = 0;
    schedInfo[1].n = 0;
    schedInfo[2].n = 0;
    schedInfo[3].n = 0;
    schedInfo[4].n = 0;
    schedInfo[5].n = 0;
   */ 
    schedInfo[0].N = 1;     // UART
    schedInfo[1].N = 1;     // Temp acquisition
    schedInfo[2].N = 10;    // Temp average and send
    schedInfo[3].N = 2;     // Feedback msg to pc
    schedInfo[4].N = 10;    // Blinking leds
    schedInfo[5].N = 1;     //  LCD update
}


void scheduler() {
    int i;
    for (i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if (schedInfo[i].n == schedInfo[i].N) {
            switch (i) {
                case 0:
                    //task1
                    break;
                case 1:
                   //task2
                    break;
                case 2:
                    //task3
                    break;
                case 3:
                    //task4
                    break;
                case 4:
                    //task5
                    break;
                case 5:
                    //task6
                    break;
            }
            schedInfo[i].n = 0;
        }
    }
}