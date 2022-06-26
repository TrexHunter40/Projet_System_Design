/*
 * File:   interruption.c
 * Author: AMEUR
 *
 * Created on 03 may 2022
 */

#include <p18cxxx.h>
#include "Interruptions.h"
#include "global.h"
#include "I2C.h"
#include <stdio.h>

unsigned int i=0,j=0,k=0;//variables inc
char val[]="";        //vbat value str
volatile unsigned int tmr1tick = 0;
volatile unsigned int flagdebounce = 1;

int vreal=0;    //moyenne de vbat
float vrealconv = 0;
int val_mv;
//high priority vector
#pragma code HighVector=0x08
void IntHighVector(void)
{
    _asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR
void HighISR(void)
{
    //IT télécommande
   if(INTCONbits.INT0IF) //if flag = 1
   {
      INTCONbits.INT0IF = 0; //flag = 0

      Lire_i2c_Telecom(0xA2, touche);//lecture
     
      if(touche[1]==0x33)//bouton marche/arrêt (0x33)
      {
          
       if(flagdebounce == 1){
          if(marche==0)
          {
              marche = 1;
              //printf("Appui bouton telecommande\r\n");
              led = 0b01111111 & led;   //allumage premiere led
              Write_PCF8574(0x40, led);
              flagdebounce = 0;
          }
          else
            {
              marche = 0;
              //printf("Appui bouton telecommande\r\n");
              led = 0b10000000 | led;   //allumage deuxieme led
              Write_PCF8574(0x40, led);
              flagdebounce = 0;
          }
        }
      }
   }
   //IT Timer0
   if(INTCONbits.TMR0IF)
   {
        INTCONbits.TMR0IF=0;
        TMR0H=0x85;             //Interruption toutes les secondes
        TMR0L=0xEE;
        ADCON0bits.GO=1;
    }
   

   
   //IT ADC
   if(PIR1bits.ADIF==1)
   {
      ADCON0bits.GO=0;
      PIR1bits.ADIF=0;

      vbat+=ADRESH;
      nbVmesure++;
      if(nbVmesure==8){
          vreal = vbat >> 3;
          vrealconv = vreal/15.9375 +0.7; //  x/15.9375 = x*(5/255)*3.2
          val_mv = vrealconv*1000;
          nbVmesure=0;
          vbat=0;

          if (val_mv<9000){

              //printf("%d\r\n",vrealconv);
              PORTBbits.RB5 = 1;
              led = 0b11111110 & led;             //allumage derniere led
              Write_PCF8574(0x40, led);
              printf("Défaut batterie ! Tension actuelle : %d mV\r\n", val_mv);
              arret();
              marche=0;
            }
          else {
            PORTBbits.RB5 = 0;
            led = 0b00000001 | led;//eteint dernière led
            Write_PCF8574(0x40, led);
            
            printf("Vbat = %d mV\r\n",val_mv); //ecrit en mV
          
            } 
       }
   }
   
     //Timer 1
   if(PIR1bits.TMR1IF){
       
       PIR1bits.TMR1IF=0;
       TMR1H = 0x3C;
       TMR1L = 0xB0;
       if(marche == 1) {
           distance=SONAR_Read(0xE0,0x02);
           //printf("%d",distance);
           SONAR_Write(0xE0,0x51);
       }

       
       if(flagdebounce == 0 ) {
           tmr1tick++;
           if(tmr1tick == 3) {      //300ms
               flagdebounce = 1;
               tmr1tick = 0;
           }
       }
    }
}

