/*File: Initialisation.c
 * Author: Yazid
 *
 * Created on 2 May 2022, 15:55
 */

#include <p18f2520.h>

void Init(void){

//    TRISBbits.RB5=0;  // RB5 en sortie
//    TRISBbits.RB6=0;  // RB5 en sortie
    //Horloge à 8Mhz
    OSCCONbits.IRCF=7;
/**********************************************************************/
    //Desactiver infrarouge
    PORTBbits.RB1 = 1;   //En input, comme ca n influe pas sur le MOS de controle du capteur IR (Q6)
/**********************************************************************/
    //Configuration ADC. (p225-227 datasheet)
    ADCON0bits.ADON = 1; //A/D converter module is enabled
    ADCON0bits.CHS = 2;  //L'ADC lit la tension sur l entree AN2
    ADCON0bits.GO = 1;   //A/D conversion in progress 

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
    INTCON2bits.INTEDG1 = 0;    //External interrupt 1 Falling Edge

    PIR1bits.ADIF = 0; //Effacement Flag IT ADC (p98 doc)
    PIE1bits.ADIE = 1; //Enables the A/D interrupt

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1; // Validation INT Périphériques
/*****************************************************************/
    //Timer0    (p125 datasheet) --> pour it adc
    T0CONbits.T08BIT=0;//Timer0 configurer en 16 bitsr
    T0CONbits.T0CS=0;//source d'horloge interne
    T0CONbits.PSA=0;//prescaler is assigned
    T0CONbits.T0PS=5;//Prescalar selection 1:64
    INTCONbits.TMR0IE=1;//enable overflow interrupt
    T0CONbits.T0SE = 0; //Increment on low-to-high transition on T0CKI pin
    /* Reglage start time pour overflow a 1s*/  //!!!!!a refaire a chaque fois dans l'interruption
    TMR0H=0x85; //Start time         1.097152*65536/1.097152 = 34286
    TMR0L=0xEE;

    T0CONbits.TMR0ON=1;//activer  timer0
/******************************************************************/
     //Timer1                   --> pour lecture distance
    T1CONbits.T1RUN=1;      //activer timer 1
    T1CONbits.RD16=1;       //Mode 16 bits
    T1CONbits.T1CKPS=2;     //prescaler=1:4
    T1CONbits.T1OSCEN=0;    //Oscillator disabled
    T1CONbits.T1SYNC=0;     //Ignored bit
    T1CONbits.TMR1CS=0;     //Internal clock

    PIR1bits.TMR1IF=0;
    PIE1bits.TMR1IE=1;
    /* Reglage start time pour overflow a 100ms*/
    TMR1H = 0x3C;           //15536
    TMR1L = 0xB0;

    T1CONbits.TMR1ON = 1;

  /******************************************************************/
    


    


    
}
