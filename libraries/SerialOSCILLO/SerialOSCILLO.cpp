
#include "SerialOSCILLO.h"

#include <avr/pgmspace.h>

//////////////////// PUBLIC

// << CONSTRUCTOR >>
/*EDU US*/ SerialOSCILLO::SerialOSCILLO(){
		
	//---- On initialise les pointeurs à 0
		for(int8_t i;i<SerialOSCILLO_NbrPins;i++){m_pinsActivesPoint[i]=NULL;}
		

}


/*EDU FR*/	void SerialOSCILLO::brancher(int32_t speed){branch(speed);}//*/
/*EDU US*/	void SerialOSCILLO::begin(int32_t speed){branch(speed);}//*/
/*EDU US*/	void SerialOSCILLO::branch(int32_t speed){
		
		Serial.begin(speed);
		Serial.println("OSCILLO OK");
												
}//*/

/*EDU US*/	void SerialOSCILLO::write(int8_t pin){

	//---- Construction du pointCourbe
		if(m_pinsActivesPoint[pin]==0){
			//--- On construit l'objets et on met le pointeur dans le tableau des pointeurs de pointsCourbe
			m_pinsActivesPoint[pin] = new Point(pin);	
			Serial.println("Construction");
		}
	//---- Ecriture du point sur le port série
		m_pinsActivesPoint[pin]->write();
		
		

			
	
		

}

SerialOSCILLO SerialOscillo;







////////////////////:: Reste







