/*
  NewServo
  2014 Copyright (c) www.duinoedu.com. 
  For Ultrasonic v1.1 firmware.
 
  Original Author: David Souder
  
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

	/*EDU FR*/	uint8_t 	brancher(int pin);
	/*EDU US*/	uint8_t 	branch(int pin);
	/*EDU US*/	uint8_t 	attach(int pin);
	
	/*EDU US*/	void 			ecrire(int value, int speed=1000); 
	/*EDU US*/	void 			ecrireAngle(int value, int speed=1000); 
	/*EDU US*/	void 			write(int value, int speed=1000); 
	/*EDU US*/	void 			writeMicroseconds(int value, int speed=1000); 
	
	Servo* 			servo;					// Le servo de base auquel on ajoute des méthodes
	
	
	
	
	
	
	
	
	protected:
	
	uint16_t		m_curentMicroPos;					// Durée ACTUELLE de l'impultion en microsecondes
	uint16_t		m_finalMicroPos;						// Durée SOUHAITEE de l'impultion en microsecondes
	uint16_t		m_speed;									// Vitesse à laquelle doit s'éfectuer le changement de position du servo










};
#endif
