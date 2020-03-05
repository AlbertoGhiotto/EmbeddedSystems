#ifndef XC_HEADER_LCD_H
#define	XC_HEADER_LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Define cursor position
#define STA 0x80
#define TEM 0x88
#define RPM 0xC0

void printToLCD(char string[], int position);
void setTitles();
void setLCD();
void clearLCD();

// String to print on LCD before data
char mander[5];
char meleon[5];
char izard[5]; 

// Support variable for printing second row
char print[10];

#endif	/* XC_HEADER_LCD_H */


