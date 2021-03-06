/*
 * File:   fromUart.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 28 December 2019, 10:47
 */

#include <stdio.h>
#include <string.h>
#include "xc.h"
#include "global.h"
#include "fromUart.h"
#include "toUart.h"
#include "lcd.h"
#include "pwm.h"
#include "parser.h"
#include "timer.h"
#include "buttons.h"
#include "buffers.h"

int fromUart(void) {

    int tempVar = 0;    // Variable for reading uart msgs
    char tempConv;      // Variable for converting int to ascii
    // Decoding support variables
    int parseFlag;
    int decodeFlag;
    int bufferFlag;

    while (sizeBuf(&transmissionBuffer) > 0) {
        bufferFlag = readCircBuffer(&transmissionBuffer, &tempVar);      // Read msg from PC
        tempConv = tempVar;                                              // Convert int into corresponding char ascii code
        parseFlag = parse_byte(&pstate, tempConv);                       // Parse each byte 

        if (parseFlag == NEW_MESSAGE) {
            decodeFlag = decodeMessage(pstate.msg_type, pstate.msg_payload);    // Get type of message
            sendACK_enInt(decodeFlag);                                          // Send relative ack
        }
    }
    while (U2STAbits.URXDA == 1) {                                        // Indicates that the receive buffer has data available
        tempVar = U2RXREG;                                                // Read receive buffer
        tempConv = tempVar;                                               // Convert int into corresponding char ascii code
        parseFlag = parse_byte(&pstate, tempConv);                      

        if (parseFlag == NEW_MESSAGE) {
            decodeFlag = decodeMessage(pstate.msg_type, pstate.msg_payload);
            sendACK_enInt(decodeFlag);
        }
    }
    return 0;
}

int decodeMessage(char* msg_type, char* msg_payload)
{   
     // Support rpm values
    int tempRPM1 = 0;
    int tempRPM2 = 0;
    // Support saturation values
    int tempMin = 0;
    int tempMax = 0;
    
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
            if(!normalizeDC(&tempRPM1, &tempRPM2))      // Set the new PWM values
            {
                // Set the new RPM values to be printed
                actualRPM1 = tempRPM1;          
                actualRPM2 = tempRPM2;
                
                return REF_P;       // Positive reference ack signal
            }
            else                    // If the message didn't get sent correctly
            {
                return REF_N;       // Negative reference ack signal
            }
        }
        return REF_N;               // In safe mode no new messages are accepted    
    }
    else if(strcmp(msg_type, "HLSAT") == 0)             // Message is of type HLSAT
    {
        // Construct the message with the rpm
        sscanf(msg_payload, "%d,%d", &tempMin, &tempMax);
        if(!updateRange(tempMin, tempMax))          // Update the saturation values
        {
            normalizeDC(&actualRPM1, &actualRPM2);  // The PWM is refreshed to comply with the new saturation values
            return SAT_P;                           // Positive saturation ack signal
        }
        return SAT_N;                               // Negative saturation ack signal
    }
    else if (strcmp(msg_type, "HLENA") == 0)        // Message is of type HLENA
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

void sendACK_enInt(int decodeFlag) {
    switch (decodeFlag) {
        case REF_N:
            sendToPC("MCACK,REF,0");
            break;
        case REF_P:
            sendToPC("MCACK,REF,1");
            tmr2_restart_timer();       // Restart timer since a new reference arrived
            break;
        case SAT_N:
            sendToPC("MCACK,SAT,0");
            break;
        case SAT_P:
            sendToPC("MCACK,SAT,1");
            break;
        case ENA_N:
            sendToPC("MCACK,ENA,0");
            break;
        case ENA_P:
            sendToPC("MCACK,ENA,1");
            
            tmr2_restart_timer();       // Enable timer 2 interrupts for timeout mode and restart timer
            setButton();              // Re-enable buttons interrupts for safe mode
            break;
        default:
            //sendToPC("ERR");
            break;
    }
    return;
}