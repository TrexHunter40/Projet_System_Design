/*
 * File:   interruption.c
 * Author: AMEUR
 *
 * Created on 03 may 2022
 */

#include <p18cxxx.h>
#include "Interruptions.h"
#include "global.h"
#include "main.h"
#include "I2C.h"

char msg_arret[] = "Arret moteurs\r\n";
//char msg_mV[] = " mV\r\n";
char msg_bat[]= "vbat=";
char msg_defaut_bat[] = "defaut batterie\r\n";
unsigned int i=0,j=0,k=0;//variables inc
char val[]="";        //vbat value str
volatile unsigned int tmr1tick = 0;
volatile unsigned int flagdebounce = 1;

unsigned float vreal=0;//vbat mid value
unsigned float vrealconv = 0;
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
      //Ecrire_i2c_Telecom(0xA2, 0x33);       //demande à la telecommande
      //while(Detecte_i2c(0xA2));//ack
      Lire_i2c_Telecom(0xA2, touche);//lecture
     
      if(touche[1]==0x33)//bouton marche/arrêt (0x33)
      {
          
       if(flagdebounce == 1){
          if(marche==0)
          {
              marche = 1;
              led = 0b01111111 & led;   //allumage premiere led
              Write_PCF8574(0x40, led);
              flagdebounce = 0;
          }
          else
            {
              marche = 0;
              printf("%s",msg_arret);
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
//        LATBbits.LATB4=~LATBbits.LATB4;
    }
   

   
   //IT ADC
   if(PIR1bits.ADIF==1)
   {
      ADCON0bits.GO=0;
      PIR1bits.ADIF=0;
      //
//      LATBbits.LATB6=~LATBbits.LATB6;
      //
      vbat+=ADRESH;
      nbVmesure++;
      if(nbVmesure==8 && vbat/8<133){   //
          vreal=vbat/8.0;
          vrealconv = vreal*(5/255) *3.2 + 0.7;
          printf("%d",vrealconv*1000);
          PORTBbits.RB5 = 1;
          led = 0b11111110 & led;             //allumage derniere led
          Write_PCF8574(0x40, led);
          //printf("%s",msg_defaut_bat);
          nbVmesure=0;
          arret();
          marche=0;
      }
      else if(nbVmesure==8){
          PORTBbits.RB5 = 0;
          led = 0b00000001 | led;//eteint dernière led
          Write_PCF8574(0x40, led);
          vreal=vbat/8.0;
          printf("%d",vreal);
          vrealconv = vreal*(5/255) *3.2 + 0.7;
          //printf("%s",msg_bat);
          printf("%d",vrealconv*1000); //ecrit en mV
          
          vbat=0;
          nbVmesure=0;
      }
   }
   
     //Timer 1
   if(PIR1bits.TMR1IF){
       //
//       LATBbits.LATB7=~LATBbits.LATB7;
       //
       PIR1bits.TMR1IF=0;
       TMR1H = 0x3C;
       TMR1L = 0xB0;
       if(marche == 1) {
           distance=SONAR_Read(0xE0,0x02) - 6; //-3 car calibrage
           printf("%d",distance);
           //SONAR_Write(0xE0,0x00);
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

