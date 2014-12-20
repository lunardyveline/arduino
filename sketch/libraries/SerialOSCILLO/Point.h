/*
	Librairie créée par David Souder - 01/08/2014
	Cette librairie est librement utilisable
	www.duinoedu.com

*/

// Voir infos en bas de page


#ifndef Point_H
#define Point_H

#include "EDU_debug.h"

//----- Nécessaire si utilisation de String

	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif

//----- Début classe
class Point {

public:

		/*EDU US*/	Point(int8_t pin);
		/*EDU US*/	void write();

		//---- Mémoire
		int8_t 		m_pin;						
		int16_t		m_conv;
		int32_t		m_lastMillis;
		char			m_type;										// vaut A si c'est de l'analogique
		

			

		

protected:
		
		


		
};
#endif







