/*
  NewServo
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder - souder.d@gmail.com
  Date de derni�re modification : 11/02/2014
  
*/

#ifndef NewServo_H
#define NewServo_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


#include <Servo.h>
#include "EDU_debug.h"

#define DEBUG

class ServoPlus{

	public:
	
	/*EDU FR*/	ServoPlus();

	/*EDU FR*/	uint8_t 	brancher(int pin, int buteeHaute=165, int buteeBasse=15);
	/*EDU US*/	uint8_t 	branch(int pin, int buteeHaute=165, int buteeBasse=15);
	/*EDU US*/	uint8_t 	attach(int pin, int buteeHaute=165, int buteeBasse=15);
	
	/*EDU US*/	void 		ecrire(int value, int speed=1000); 
	/*EDU US*/	void 		ecrireAngle(int value, int speed=1000); 
	/*EDU US*/	void 		write(int value, int speed=1000); 
	/*EDU US*/	void 		writeMicroseconds(int value, int speed=1000); 
	
	Servo* 			servo;					// Le servo de base auquel on ajoute des m�thodes
	
	
	
	
	
	
	
	
	protected:
	
	uint16_t		m_curentMicroPos;					// Dur�e ACTUELLE de l'impultion en microsecondes
	uint16_t		m_finalMicroPos;					// Dur�e SOUHAITEE de l'impultion en microsecondes
	uint16_t		m_speed;							// Vitesse � laquelle doit s'�fectuer le changement de position du servo
    uint16_t		m_buteeBasse;					
	uint16_t		m_buteeHaute;						// 
	String			m_version;							// Version du module








};
#endif
