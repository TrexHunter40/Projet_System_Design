/* 
 * File:   global.h
 * Author: simon
 *
 * Created on 7 mai 2021, 14:18
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

extern unsigned char marche,nbVmesure;
extern volatile unsigned int vbat;//sum of vbats
extern unsigned int CycleMoteurD,CycleMoteurG;
extern volatile unsigned int led;
extern volatile unsigned int distance;
extern unsigned volatile char touche[3];//stores remote button value

int arret(void);



#endif	/* GLOBAL_H */

    