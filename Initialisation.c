/*File: Initialisation.c
 * Author: Yazid
 *
 * Created on 2 May 2022, 15:55
 */

#include <p18f2520.h>
#include "global.h"
#include "Interruptions.h"
#include "I2C.h"
#include "main.h"


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
    T1CONbits.T1RUN=1;      //
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

    T1CONbits.TMR1ON = 1;   //Enables Timer1

  /******************************************************************/
    //Init I2C
    SSPSTAT = 0x80;	// Slew rate 100KHz
    SSPCON1 = 0x28;	// Master Mode Enable, Sclock = FOSC/(4 * (SSPADD + 1)) I2C bus,
    SSPCON2 = 0x00;
    SSPADD  = 0x13;     // Prediviseur pour SCL = 100KHz a 8MHz

  /******************************************************************/
    //uart
    TXSTAbits.SYNC=0;       //Asynchronous mode
    TXSTAbits.BRGH=1;       //High speed
    TXSTAbits.TXEN=1;       //transmit enabled
    
    BAUDCONbits.BRG16=0;    //Baud rate

    RCSTAbits.SPEN=1;       //enable serial port
    RCSTAbits.CREN=1;       //enables receiver

    
    PIR1bits.TXIF=0;
    PIE1bits.TXIE=0;        //Enables the EUSART transmit interrupt


  /******************************************************************/

    //Premiere ecriture sonar
    SONAR_Write(0xE0,0x00);
    SONAR_Write(0xE0,0x51);

}
  /******************************************************************/

    void InitMot(void){
    char message[30]="Fin des initialisation\r\n";
    TRISAbits.RA6=0; //DIRD sortie
    TRISAbits.RA7=0; // DIRG sotie
    TRISAbits.RA4=1; //acquisition moteur droit
    TRISCbits.RC0=1; //acquisition moteur gauche

    /*init Timer 2*/
    /* Configuration E/S*/
    TRISCbits.RC1 = 0;          // RC1 en sortie PWM droite
    TRISCbits.RC2 = 0;          // RC2 en sortie PWM gauche

    /* Configuration TIMER2 */
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 0;      // CLK /4
    PR2 = 499;                  // Reglage periode FPWM = Fosc/(4*(PR2+1)*PRE)
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
    CCPR1L = CycleMoteurD * 2.5;
    CCPR2L = CycleMoteurG * 2.5;
    INTCONbits.GIE=1;  // Validation globale des INT     //?
    ecrireChar(message);

}
