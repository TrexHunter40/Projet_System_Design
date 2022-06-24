#include <p18f2520.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <i2c.h>
#include "global.h"
#include "Initialisation.h"
#include "Interruptions.h"
#include "I2C.h"
#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON


unsigned char marche,nbVmesure;
volatile unsigned int vbat;
unsigned int CycleMoteurD,CycleMoteurG;
volatile unsigned int led = 0b11111111;
volatile unsigned int distance;
unsigned volatile char touche[3];

void main(void) {
    Initialisation();
    InitialiserMoteurs();
    
    while(1)
    {
        if (marche==1)
        {
            
            phase1();

            phase2();
        }
        if(marche==0){
            arret();
        }
    }
}

	
int phase1(void){
    led = 0b10111111 & led;         //allumage deuxieme led
    Write_PCF8574(0x40, led);
    //printf("Demarrage phase 1\r\n");


    
    while(distance>40 && distance<150 && marche==1)
    {
        CCPR1L = 50 ;
        CCPR2L = 45 ;
    }
    arret();
    //printf("Fin phase 1 \r\n");
    led = 0b01000000 | led;//eteint deuxieme led
    Write_PCF8574(0x40, led);
    return 1;
}

int phase2(void){
    led = 0b11011111 & led;//allumage troisieme led
    Write_PCF8574(0x40, led);
    //printf("Demarrage phase 2\r\n");



    while(marche==1)
    {
        if(distance>50) {
            
            CCPR1L = 50 ;
            CCPR2L = 45 ;
        }
        else {
            arret();
            
        }
    }
    //printf("Fin phase 2\r\n");
    led = 0b00100000 | led;         //eteint troisieme led
    Write_PCF8574(0x40, led);
    //printf("Arrêt du robot\r\n");
    return 1;
}

int arret(void)
{
  CCPR1L = 0;
  CCPR2L = 0;
  return 1;
}
