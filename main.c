/*File: main.c
 * Author: Yazid
 *
 * Created on 9 May 2022, 9:31
 */

#include <p18f2520.h>
#include "Initialisation.h"
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON

void main(void){
    Init();
//    while(1) {
//       if(INTCONbits.TMR0IF){
//          INTCONbits.TMR0IF=0;      // RAZ flag
//          TMR0H=0x85; //Start time         1.097152*65536/1.097152 = 34286
//          TMR0L=0xEE;
//          LATBbits.LATB5=~LATBbits.LATB5;
//       }
//       if(PIR1bits.TMR1IF){
//            PIR1bits.TMR1IF=0;       // RAZ flag
//            TMR1H = 0x3C;           //15536
//            TMR1L = 0xB0;
//            LATBbits.LATB6=~LATBbits.LATB6;
//
//            //test
//       }
//       }
    while(1);
}


int arret(void)
{
  CCPR1L = 0;
  CCPR2L = 0;
}

