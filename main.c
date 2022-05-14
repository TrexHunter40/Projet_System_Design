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
#include "global.h"
#include "Initialisation.h"
#include "Interruptions.h"
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
    InitMot();
    marche=0;
    while(1)
    {
        if (marche==1)
        {
            tempo(520000);
            avancerPhase1();
            tempo(520000);

            avancerPhase2();
        }
    if(marche==0) arret();
    }

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


void tempo(unsigned int T){
    unsigned int j;
    for(j=0;j<T;j++);}


int avancerPhase1(void){
    char message1[30]="Demarrage phase 1\r\n";
    led = 0b10111111 & led;//allumage deuxieme led
    Write_PCF8574(0x40, led);
    ecrireChar(message1);
    PORTAbits.RA6=1;
    PORTAbits.RA7=1;

    while(distance>45 && distance<160 && marche==1)
    {
        CycleMoteurD = 30; // <50
        CycleMoteurG = 30;
        CCPR1L = CycleMoteurD * 2.5;
        CCPR2L = CycleMoteurG * 2.5;
    }
    arret();
    led = 0b01000000 | led;//eteint deuxieme led
    Write_PCF8574(0x40, led);
    return 1;
}

int avancerPhase2(void){
    char message2[30]="Demarrage phase 2\r\n";
    led = 0b11011111 & led;//allumage troisieme led
    Write_PCF8574(0x40, led);
    ecrireChar(message2);
    PORTAbits.RA6=1;
    PORTAbits.RA7=1;

    while(marche==1)
    {
        if(distance>60) {
            CycleMoteurD = 40; // <50
            CycleMoteurG = 40;
            CCPR1L = CycleMoteurD * 2.5;
            CCPR2L = CycleMoteurG * 2.5;
        }
        else {
            arret();
        }
    }
    led = 0b00100000 | led;//eteint troisieme led
    Write_PCF8574(0x40, led);
    return 1;
}

//message uart
void ecrireChar(char c[30]){
    printf("%s",c);
}

void ecrireInt( int k){
    printf("%d\r\n",k);
}


int arret(void)
{
  CCPR1L = 0;
  CCPR2L = 0;
}

