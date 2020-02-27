#ifndef XC_HEADER_MCFBK_H
#define	XC_HEADER_MCFBK_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Message variable
char msg[20];
// Support variable for state printing
//char state[1];

int sendMCFBK(void);

#endif	/* XC_HEADER_MCFBK_H */

