/*File: main.c
 * Author: Yazid
 *
 * Created on 9 May 2022, 9:31
 */

#include <p18f2520.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <i2c.h>

#include "main.h"
//#include "global.h"
#include "Initialisation.h"
//#include "Interruptions.h"
#include "I2C.h"


#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON


unsigned char marche,nbVmesure;
volatile unsigned long vbat;
unsigned int CycleMoteurD,CycleMoteurG;
volatile unsigned int led = 0b11111111;
volatile unsigned int distance;
unsigned volatile char touche[3];


void main(void){
    Init();
    //InitMoteurs();
    while(1)
    {
        //bonsoir
    }
}