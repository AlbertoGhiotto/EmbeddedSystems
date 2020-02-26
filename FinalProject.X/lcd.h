#ifndef XC_HEADER_LCD_H
#define	XC_HEADER_LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Define cursor position
#define STA 0x80
#define TEM 0x88
#define RPM 0xC0

void printTemp(char string[]);
void printStatus(char string[]);
void printRPM(char string[]);
void setLCD();
void clearLCD();

#endif	/* XC_HEADER_LCD_H */


