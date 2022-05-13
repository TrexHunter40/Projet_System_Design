/* 
 * File:   global.h
 * Author: leoch
 *
 * Created on 13 mai 2022, 15:43
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

extern unsigned char marche,nbVmesure;
extern volatile unsigned long vbat;//sum of vbats
extern unsigned int CycleMoteurD,CycleMoteurG;
extern volatile unsigned int led;
extern volatile unsigned int distance;
extern unsigned volatile char touche[3];//stores remote button value

#endif	/* GLOBAL_H */
