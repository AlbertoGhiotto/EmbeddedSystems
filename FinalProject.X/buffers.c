/*
 * File:   buffers.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 26 December 2019, 15:52
 */

#include "xc.h"
#include "global.h"
#include "buffers.h"

void writeOnTempBuff(float data){
    temp[tempIndex] = data;
    tempIndex++;
}