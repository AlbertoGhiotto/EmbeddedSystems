/*
 * File:   sendMCFBK.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 27 December 2019, 12:57
 */

#include <stdio.h>
#include "xc.h"
#include "sendMCFBK.h"
#include "global.h"
#include "toUart.h"

int sendMCFBK() {

    // Message variable
    char msg[50];
    char state;

    
    switch (board_state) {
        case STATE_CONTROLLED:  state = 'C';  break;
        case STATE_TIMEOUT:     state = 'T';  break;
        case STATE_SAFE:        state = 'H';  break;
    }

    // Convert 
    sprintf(msg, "MCFBK,%d,%d,%c. S6:%d", actualRPM1, actualRPM2, state, flagS6);

    // Send message to PC
    sendToPC(msg);

    return 0;
}