/*File: Initialisation.c
 * Author: Yazid
 *
 * Created on 2 May 2022, 15:55
 */

#include <p18f2520.h>

void Init(void){
 
    //Horloge à 8Mhz
    OSCCONbits.IRCF=7;
/**********************************************************************/
    //Desactiver infrarouge
    PORTBbits.RB1 = 1;   //En input, comme ca n influe pas sur le MOS de controle du capteur IR (Q6)
/**********************************************************************/
    //Configuration ADC. (p225-227 datasheet)
    ADCON0bits.ADON = 1; //On active l'ADC
    ADCON0bits.CHS = 2;  //L'ADC lit la tension sur l entree AN2
    ADCON0bits.GO = 1;   //A/D conversion in progress  il faut le mettre à 1 mtn ???

    ADCON1bits.VCFG1 = 0;  //Vref- = Vss
    ADCON1bits.VCFG0 = 0;  //Vref+ =VDD
    ADCON1bits.PCFG = 12; //Entree 2 en mode analogique

    ADCON2bits.ADFM = 0;    // Left justified car prend moins de RAM
    ADCON2bits.ACQT2 = 0;   //6Tad (p.128 du cours) -> Tacq >4.2 us
    ADCON2bits.ACQT1 = 1;
    ADCON2bits.ACQT0 = 1;

    ADCON2bits.ADCS2  = 0;     // 8Tosc Tad>0.7 us
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS0 = 1;
/*****************************************************************/
    //Interruptions
    INTCONbits.INT0IE = 1;  //INT0 ON
    INTCON2bits.INTEDG0 = 0; //INT0 sur front descendant

    PIR1bits.ADIF = 0; //Effacement Flag IT ADC (p98 doc)
    PIE1bits.ADIE = 1; //Validation INT ADC

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1; // Validation INT Périphériques
/*****************************************************************/
    //Timer0    (p125 datasheet)
    T0CONbits.TMR0ON = 1;       //activer  timer0
    T0CONbits.T08BIT = 0;       //Timer0 configurer en 16 bits
    T0CONbits.T0CS = 0;         //source d'horloge interne
    T0CONbits.T0SE = 1;         //increment on low-to-high transition
    T0CONbits.PSA = 0;          //prescaler is assigned
    T0CONbits.T0PS = 1;         //Prescalar selection 1:4 ->0.0000005 *65536*4=0.131 secondes
    /* Reglage start time pour overflow a 100ms*/  //!!!!!a refaire a chaque fis dans l'interruption
    TMR0H = 0x3C;
    TMR0L = 0x94;
/******************************************************************/
    //Init I2C


    TRISBbits.RB5=0;  // RB5 en sortie
    

    
}