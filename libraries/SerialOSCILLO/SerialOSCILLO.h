/*
	Librairie créée par David Souder - 01/08/2014
	Cette librairie est librement utilisable
	www.duinoedu.com

*/

// Voir infos en bas de page


#ifndef SerialOSCILLO_H
#define SerialOSCILLO_H

#include <SoftwareSerial.h>
#include "Point.h"

#include "EDU_debug.h"

//----- Nécessaire si utilisation de String

	#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include <WProgram.h>
	#include "Wire.h"
	#endif

//----- Langage EDU
#define faire void
#define initialisation setup()
#define boucle loop()

#define portSerie Serial.begin(9600)

#define lectureEtat digitalRead

#define utiliser 

#define chaine String
#define nombre int
#define grandNombre long

#define si if
#define sinon else


//----- Début classe
class SerialOSCILLO {

public:

		/*EDU US*/	SerialOSCILLO();

	//---- Initialisation
		/*EDU FR*/	void brancher(int32_t speed=9600);
		/*EDU FR*/	void begin(int32_t speed=9600);
		/*EDU US*/	void branch(int32_t speed=9600);

		
	//---- Commandes d'écriture
		/*EDU US*/	void write(int8_t pin);
			
		
		
	//---- Modification options
		
		
		
	//---- Variables publiques
		
			

		

protected:
	//---- Mémoire des points créés
		#define SerialOSCILLO_NbrPins	54
		Point* m_pinsActivesPoint[SerialOSCILLO_NbrPins];			// Tableau de pointeurs vers mes PointsCourbes
		
	
		

		
};

extern SerialOSCILLO SerialOscillo;

#endif







