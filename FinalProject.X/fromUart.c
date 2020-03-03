/*
 * File:   fromUart.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 28 December 2019, 10:47
 */


#include "xc.h"
#include "global.h"
#include "fromUart.h"
#include "lcd.h"
#include "pwm.h"
#include "parser.h"

int fromUart(void) {
    
    while (U2STAbits.URXDA == 1) {  // Indicates that the receive buffer has data available
            tempVar = U2RXREG;      // Read receive buffer
            // U2TXREG = tempBuff;
            tempConv = tempVar;     // Convert int into corresponding char ascii code
            parseFlag = parse_byte(&pstate, tempConv);
           
            if(parseFlag == NEW_MESSAGE){
                decodeFlag = decodeMessage(pstate.msg_type, pstate.msg_payload);
            }
            
        }
        if (U2STAbits.URXDA == 1) {  // reset URXDA flag
            tempVar = U2RXREG;
        }
    
    
    return 0;
}
int decodeMessage(char* msg_type, char* msg_payload)
{   
    if(strcmp(msg_type, "HLREF") == 0)              // Message is of type HLREF
    {         
        if(board_state != STATE_SAFE )
        {
            if(board_state == STATE_TIMEOUT)    
            {
                // A new message (reference) arrived -> get out of timeout state
                board_state = STATE_CONTROLLED; 
            }
            // Construct the message with the rpm
            sscanf(msg_payload, "%d,%d", &tempRPM1, &tempRPM2);
            if(!normalizeDC(&tempRPM1, &tempRPM2))
            {
                actualRPM1 = tempRPM1;
                actualRPM2 = tempRPM2;
                
                return REF_P;       // Positive reference ack signal
            }
            else    // If the message didn't get sent correctly
            {
                return REF_N;       // Negative reference ack signal
            }
        }
        return REF_N;               // In safe mode no new messages are accepted    
    }
    else if(strcmp(msg_type, "HLSAT") == 0)             // Message is of type HLSAT
    {
        // Construct the message with the rpm
        sscanf(msg_payload, "%d,%d", &tempMin, &tempMin);
        if(!updateRange(actualRPM1, actualRPM2))
        {
            return SAT_P;           // Positive saturation ack signal
        }
        return SAT_N;               // Negative saturation ack signal
    }
    else if (strcmp(msg_type, "HLENA") == 0) // Message is of type HLENA
    {
        if (board_state == STATE_SAFE) 
        {
            board_state = STATE_CONTROLLED;
            return ENA_P;
        }
        else
        {
            return ENA_N;
        }
    }   
    return ERR;
}