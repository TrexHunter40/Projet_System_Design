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

void main(void) {
    Initialisation();
    InitialiserMoteurs();
    marche=0;
    while(1)
    {
        //CCPR1L = 70;
        //CCPR2L = 70;
        if (marche==1)
        {
            //tempo(520000);
            avancerPhase1();
            //tempo(520000);

            avancerPhase2();
        }
        if(marche==0){
            arret();
        }
    }
}


void tempo(unsigned int T){
    unsigned int j;
    for(j=0;j<T;j++);}
	
int avancerPhase1(void){
    char message1[30]="Demarrage phase 1\r\n";
    char message12[30] = "Fin phase 1 \r\n";
    led = 0b10111111 & led;         //allumage deuxieme led
    Write_PCF8574(0x40, led);
    //printf("Demarrage phase 1\r\n");
//    ecrireChar(message1);

    
    while(distance>45 && distance<160 && marche==1)
    {
        CCPR1L = 30 ;
        CCPR2L = 30 ;
    }
    arret();
    //printf("Fin phase 1 \r\n");
//    ecrireChar(message12);
    led = 0b01000000 | led;//eteint deuxieme led
    Write_PCF8574(0x40, led);
    return 1;
}

int avancerPhase2(void){
    char message2[30]="Demarrage phase 2\r\n";
    char message22[30] = "Fin phase 2 \r\n";
    led = 0b11011111 & led;//allumage troisieme led
    Write_PCF8574(0x40, led);
    //printf("Demarrage phase 2\r\n");
//    ecrireChar(message2);


    while(marche==1)
    {
        if(distance>60) {
            
            CCPR1L = 30 ;
            CCPR2L = 30 ;
        }
        else {
            arret();
            
        }
    }
    //printf("Fin phase 2\r\n");
//    ecrireChar(message22);
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

void ecrireFloat (float k){
    printf("%f\r\n",k);
}

int arret(void)
{
  CCPR1L = 0;
  CCPR2L = 0;
}
