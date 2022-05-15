/* 
 * File:   global.h
 * Author: simon
 *
 * Created on 7 mai 2021, 14:18
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

extern unsigned char marche,nbVmesure;
extern volatile unsigned long vbat;//sum of vbats
extern unsigned int CycleMoteurD,CycleMoteurG;
extern volatile unsigned int led;
extern volatile unsigned int distance;
extern unsigned volatile char touche[3];//stores remote button value

//Ajouts
//extern unsigned int ad_led = 0x40;
//extern unsigned int ad_tel = 0xA2;
//extern unsigned int ad_sonar = 0xE0;

#endif	/* GLOBAL_H */

