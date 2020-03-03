#ifndef XC_HEADER_FROMUART_H
#define	XC_HEADER_FROMUART_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define DIMENSION 25

// Definition for acknowledgement message type
#define ERR 0

#define REF_N 1     // for a negative ack to a reference command
#define REF_P 2     // for a positive ack to a reference command 

#define SAT_N 3     // for a negative ack to a saturation command
#define SAT_P 4     // for a positive ack to a saturation command

#define ENA_N 5     // for a negative ack to a enabling command
#define ENA_P 6     // for a positive ack to a enabling command


int tempVar = 0;      // Variable for reading uart msgs
char tempConv;

// Decoding support variables
int parseFlag = 0;
int decodeFlag = 0;
int bufferFlag = 0;

// Support rpm values
int tempRPM1 = 0;
int tempRPM2 = 0;
// Support saturation values
int tempMin = 0;
int tempMax = 0;

// Main function
int fromUart(void);

// Support function to decode messages
int decodeMessage(char* msg_type, char* msg_payload);


#endif	/* XC_HEADER_FROMUART_H */

