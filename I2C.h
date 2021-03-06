/* 
 * File:   I2C.h
 * Author: simon
 *
 * Created on 7 mai 2021, 14:16
 */

#ifndef I2C_H
#define	I2C_H

void MI2CInit(void);
char Detecte_i2c(char adresse_i2c);

// Pour les transactions du SONAR
void SONAR_Write( char adresse_i2c, char commande);
int SONAR_Read( char adresse_i2c, char registre_ditance);

// Pour la tÚlÚcommande
char Lire_i2c_Telecom(char adresse_i2c, char* Recv_Buff);
char Ecrire_i2c_Telecom(char adresse_i2c, char* Send_Buff);

// Pour les transactions des PCF8574
char Write_PCF8574(char adresse_i2c, char data );

#endif	/* I2C_H */



