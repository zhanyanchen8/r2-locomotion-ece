/*
 * File:   config.h
 * Author: Syed Tahmid Mahbub
 * Modified by: Bruce Land 
 * Modified again by: Manish Patel
 * Created on October 10, 2014
 * Mod: 24Sept2015
 */

#ifndef CONFIG_H
#define	CONFIG_H
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
// serial stuff
#include <stdio.h>

//=============================================================
// 40 MHz
#pragma config FNOSC = FRCPLL, POSCMOD = OFF, OSCIOFNC = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20 //40 MHz
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2 // PB 40 MHz
#pragma config FWDTEN = OFF,  FSOSCEN = OFF, JTAGEN = OFF

#endif	/* CONFIG_H */

