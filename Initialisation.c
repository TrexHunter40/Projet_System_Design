/*
 * File:   Initialisation.c
 * Author: AMEUR
 *
 * Created on 03 may 2022
 */

#include <p18f2520.h>
#include "Initialisation.h"
#include "global.h"
#include "Interruptions.h"
#include "I2C.h"
#include "main.h"



void Initialisation(){

    OSCCONbits.IRCF=7; //Horloge à 8Mhz
    
    //On valide les interruptions
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0; //Front descendant interruption 0

    //Desactivation des infrarouges
    PORTBbits.RB1 = 1;
    
    //ADC -> p225-227
    TRISBbits.RB5=0;        //output led test
    //TRISAbits.RA2=1;                                      
    ADCON1bits.VCFG=0;//Vref- = Vss & Vref+ =VDD
    ADCON1bits.PCFG=12;//Port AN2 opened
    
    ADCON2bits.ADCS2  = 0;     // 8Tosc Tad>0.7 us
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS0 = 1;

    ADCON2bits.ACQT2 = 0;   //6Tad (p.128 du cours) -> Tacq >4.2 us
    ADCON2bits.ACQT1 = 1;
    ADCON2bits.ACQT0 = 1;

    ADCON0bits.CHS=2;//Channel 2 ->AN2
    ADCON2bits.ADFM=0;//Format left justified
    ADCON0bits.ADON=1;//Enable converter

    PIR1bits.ADIF=0;//reset flag ->100
    PIE1bits.ADIE=1;//allow interrupt
    INTCON2bits.INTEDG0 = 0;//Falling Edge
    INTCON2bits.INTEDG1 = 0;//Falling Edge

    ADCON0bits.GO=1;//launch converter

 
    //Timer 0 
    T0CONbits.TMR0ON=1;//Enable timer0
    T0CONbits.T08BIT=0;//16 bits timer
    T0CONbits.T0CS=0;//Clock source internal
    T0CONbits.PSA=0;//prescaler is assigned
    T0CONbits.T0PS=5;//Prescalar selection 1:64
    INTCONbits.TMR0IE=1;//enable overflow interrupt
    TMR0H=0x85;//Start time (to overflow at 1s)
    TMR0L=0xEE;

    //Initialisation I2C
    MI2CInit();

    //uart
    TXSTAbits.SYNC=0;//Asynchronous mode
    TXSTAbits.BRGH=1;//High speed
    BAUDCONbits.BRG16=0;//Baud rate 8 bits
    SPBRG=51;
    RCSTAbits.SPEN=1;
    TXSTAbits.TXEN=1;//transmit enable
    PIR1bits.TXIF=0;
    PIE1bits.TXIE=0;
    RCSTAbits.CREN=1;//enables receiver

     //Init Timer1
    T1CONbits.RD16=1;//16 bits operation
    T1CONbits.T1RUN=1;//Start timer 1
    T1CONbits.T1CKPS=2;//prescalar=1:4
    T1CONbits.T1OSCEN=0;//Oscillator disabled
    T1CONbits.T1SYNC=0;//Ignored bit
    T1CONbits.TMR1CS=0;//Internal clock
    PIR1bits.TMR1IF=0;
    PIE1bits.TMR1IE=1;
    TMR1H = 0x3C; //15535
    TMR1L = 0xB0;
    T1CONbits.TMR1ON = 1;

    //Premiere ecriture sonar
    //SONAR_Write(0xE0,0x00);
    SONAR_Write(0xE0,0x51); //Demande de distance en cm
}

void InitialiserMoteurs(void){
    char message[30]="Fin des initialisations\r\n";
    TRISAbits.RA6=0; //DIRD sortie
    TRISAbits.RA7=0; // DIRG sotie
    TRISAbits.RA4=1; //acquisition moteur droit
    TRISCbits.RC0=1; //acquisition moteur gauche
    
    PORTAbits.RA6=1;
    PORTAbits.RA7=1;
    /*init Timer 2*/
    /* Configuration E/S*/
    TRISCbits.RC1 = 0;          // RC1 en sortie PWM droite
    TRISCbits.RC2 = 0;          // RC2 en sortie PWM gauche

    /* Configuration TIMER2 */  //p146
    T2CONbits.T2CKPS0 = 0;
    T2CONbits.T2CKPS1 = 1;      // Prescaler = 16
    PR2 = 125;                  // Reglage periode FPWM = Fosc/(4*(PR2+1)*PRE)
    T2CONbits.T2OUTPS=9;        // postscaler = 9       
    /* Reglage rapport cyclique */
    CCP1CONbits.DC1B0 = 0;
    CCP1CONbits.DC1B1 = 0;
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;
    /* Selection Mode PWM */
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP2CONbits.CCP2M3 = 1;     
    CCP2CONbits.CCP2M2 = 1;     
    /* Configuration interruption TMR2*/
    PIE1bits.TMR2IE=0;  // Validation TMR2IF (TMR2IP =1 par défault)     
    INTCONbits.PEIE=1;  //Validation des interruptions des périphériques 
    T2CONbits.TMR2ON = 1;    //Lance le moteur
    
    INTCONbits.GIE=1;  // Validation globale des INT     //?
    ecrireChar(message);
}