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


int sendMCFBK(void){
    
/*   switch (board_state)
    {
        case HALT:          state = "H";
        case TIMEOUT:       state = "T";
        case CONTROLLED:    state = "C";
    }
*/  
    
// Convert 
sprintf(msg, "MCFBK,%d,%d,%d", actualRPM1, actualRPM2, board_state);

// Send message to PC
//sendToPc(msg);

}