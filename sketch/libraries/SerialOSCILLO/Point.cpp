
#include "Point.h"

#include <avr/pgmspace.h>

//////////////////// PUBLIC

// << CONSTRUCTOR >>
/*EDU US*/ Point::Point(int8_t pin){
	
	//---- On enregistre à la construction le n° de pin
		m_pin=pin;
	
	//---- On détermine tout de suite s'il s'agit d'une entrée analogique 
		if(pin>13&&pin<22)	{m_type='A';}
		else							{m_type='D';}
	
	//---- On déclare millis valant 0
		m_lastMillis=0;
		
		
		
		
		
}//*/

/*EDU US*/ void Point::write(){

	//INFO : Permet d'ajouter un point à la courbe 
		// On utilise pour cela un motif
		// Motif de départ 			: OS_
		//	N° de broche				: nn				[0/1... /53]
		// Type Ana ou nom ?		: T               	[A/N]
		// valeur de conv			: 0000			[0/1... /1023]
		// plus							: +				
		// Interval en ms			: tttttttt		[0/1... /9999999]
		// END							: E				[' ']
		// Espace						: ' '
		
	//INFO : Protocole complet : OS_nn-0000+ttttttttE OS_nn-0000+ttttttttE OS_nn-0000+ttttttttE... (point une entrée)
	
	//---- Construction de la chaîne pour un échantillon
		String pointString;
		pointString="OS_";
		pointString+=m_pin;
		pointString+=m_type;
		if(m_type)	{ pointString+=String(analogRead(m_pin));}
		else				{ pointString+=String(digitalRead(m_pin));}
		pointString+='+';
		//--- On calcule l'interval entre les mesures 
		int32_t inter;
		if(m_lastMillis==0)	{															// Premier point
			inter=0;																			// Interval considéré nul au premier passage
			m_lastMillis=millis();
		}											
		else							{
			inter=millis()-m_lastMillis;													// Deuxième point et autres...
			m_lastMillis=millis();
		}
		pointString+=inter;
		pointString+='E';
		pointString+='\n';
		Serial.print(pointString);

}//*/









