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

volatile char str[30]="vbat=|0";
char msg_arret[30] = "Arret moteurs\r\n";
unsigned int i=0,j=0,k=0;//variables inc
char val[12]="";        //vbat value str
volatile unsigned int tmr1tick = 0;
volatile unsigned int flagdebounce = 1;
volatile unsigned long vreal=0;//vbat mid value
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
    //IT t�l�commande
   if(INTCONbits.INT0IF) //if flag = 1
   {
      INTCONbits.INT0IF = 0; //flag = 0
      Ecrire_i2c_Telecom(0xA2, 0x33);       //demande � la telecommande
      while(Detecte_i2c(0xA2));//ack
      Lire_i2c_Telecom(0xA2, touche);//lecture
      if(touche[1]==0x33)//bouton marche/arr�t (0x33)
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
              ecrireChar(msg_arret);
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
        TMR0H=0x85;//Interruption toutes les secondes
        TMR0L=0xEE;
        ADCON0bits.GO=1;
    }
   
   //Ecrire Vbat
   if(PIR1bits.TXIF&&PIE1bits.TXIE)
   {
        PIR1bits.TXIF=0;
        if(str[i]!='|') //vbat=|xx
        {
             TXREG=str[i];
             i++;
             k=(int)(12*(double)vreal/49088.0); //49088            
        }
        else{
            
            while(k!=0) //int to char
            {
                val[j]=k%10+48;
                k/=10;
                j++;
            }
            if(j>0)//invert
            {
                j--;
                TXREG=val[j];
            }
            else //reinit + retour chariot
            {
                TXREG=13;
                i=0;j=0;k=0;
                PIE1bits.TXIE=0;
            }
        }
   }

   
   //IT ADC
   if(PIR1bits.ADIF==1)
   {
      ADCON0bits.GO=0;
      PIR1bits.ADIF=0;
      vbat+=ADRESH*256+ADRESL & 0x0000FFFF;
      nbVmesure++;
      if(nbVmesure==8 && vbat/8<31424){// 2,4V ~ 8V -> 31424
          PORTBbits.RB5 = 1;
          led = 0b11111110;             //allumage derniere led
          Write_PCF8574(0x40, led);
          //shut down moteur & reinit
          arret();
          marche=0;
      }
      else if(nbVmesure==8){
          vreal=vbat/8;
          PIE1bits.TXIE=1;
          vbat=0;
          nbVmesure=0;
      }
   }
   
   //Timer 2
   if(PIR1bits.TMR2IF){
        PIR1bits.TMR2IF=0;
   }
   
     //Timer 1
   if(PIR1bits.TMR1IF){
       PIR1bits.TMR1IF=0;
       TMR1H = 0x3C;
       TMR1L = 0xB0;
       if(marche == 1) {
           distance=SONAR_Read(0xE0,0x02);
           ecrireInt(distance);
           SONAR_Write(0xE0,0x00);
           SONAR_Write(0xE0,0x51);
       }
       if(flagdebounce == 0 ) {
           tmr1tick++;
           if(tmr1tick == 5) {
               flagdebounce = 1;
               tmr1tick = 0;
           }
       }
    }
}

