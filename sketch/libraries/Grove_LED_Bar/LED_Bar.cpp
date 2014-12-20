/*
  LED bar library V1.0
  2010 Copyright (c) Seeed Technology Inc.  All right reserved.

  Original Author: LG

  Modify: Loovee, 2014-2-26
  User can choose which Io to be used.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>

#include "LED_Bar.h"


LED_Bar::LED_Bar(int pinClk, int pinDta)
{
    __pinClk = pinClk;
    __pinDta = pinDta;

    __led_state = 0x00;
    
    pinMode(__pinClk, OUTPUT);
    pinMode(__pinDta, OUTPUT);
	
	// Voltmeter
	/*EDU US*/ m_voltmeter.pinModeOk=false;     // Default config analogPin = not ok !
    /*EDU US*/ m_voltmeter.range =1023;         // Default range CAN 0~1023
	/*EDU US*/ m_voltmeter.nbrOfLed =EDU_LEDBAR_NBR_LED;       
	
	for(char i=0;i<EDU_LEDBAR_NBR_LED;i++){ledMemory[i]=0;} // Init. table of led to 0
	clearAll();
	
}

void LED_Bar::latchData()
{
    digitalWrite(__pinDta, LOW);
    delayMicroseconds(10);

    for(int i=0; i<4; i++)
    {
        digitalWrite(__pinDta, HIGH);
        digitalWrite(__pinDta, LOW);
    }

}

void LED_Bar::send16bitData(unsigned int data)
{
    for(int i=0; i<16; i++)
    {
	    // HEX : 8000 => 1111 0000 0000 0000
        unsigned int state = data&0x8000 ? HIGH : LOW;
        digitalWrite(__pinDta, state);

        state = digitalRead(__pinClk) ? LOW : HIGH;
        digitalWrite(__pinClk, state);

        data <<= 1;
    }
}

// set led single bit, a bit contrl a led
// such as, index_bits = 0x05, then led 0  and led 3 will on, the others will off
void LED_Bar::ledIndexBit(unsigned int index_bits)
{

    send16bitData(CMDMODE);

    for (int i=0;i<12;i++)
    {
        unsigned int state = (index_bits&0x0001) ? ON : SHUT;
        send16bitData(state);

        index_bits = index_bits>>1;
    }

    latchData();
}

// set level, there level 0-10
// level 0 means all leds off while level 5 means 5led on and the other will off
void LED_Bar::setLevel(int level)
{

    if(level>10)return;

    send16bitData(CMDMODE);

    for(int i=0;i<12;i++)
    {
        unsigned int state1 = (i<level) ? ON : SHUT;
        
        send16bitData(state1);
    }

    latchData();
}

// control a single led
// num - which led
// st - 1: on   0: off
void LED_Bar::setSingleLed(int num, int st)
{
    if(num>10)return;
    __led_state = st ? (__led_state | (0x01<<num)) : (__led_state & ~(0x01<<num));
    ledIndexBit(__led_state);

}

/*EDU FR*/ void LED_Bar::effacerTout(void){clearAll();}
/*EDU US*/ void LED_Bar::clearAll(void){
 	for(int i=0;i<10;i++){
		setSingleLed(i,0);
	}
} 

void LED_Bar::allumerTout(void){
 	for(int i=0;i<10;i++){
		setSingleLed(i,1);
	}
} 

/*EDU FR*/ void LED_Bar::afficher(int a,int b,int c, int d,int e,int f,int g,int h,int i,int j){
		
		display(a,b,c,d,e,f,g,h,i,j);
}
				
/*EDU US*/ void LED_Bar::display(int a,int b,int c, int d,int e,int f,int g,int h,int i,int j){
	int segments[] = {a,b,c,d,e,f,g,h,i,j};
	for(int i=0;i<10;i++){
		setSingleLed(9-i,segments[i]);
	}
}

/*EDU US*/ float LED_Bar::afficherTensionEntree(int const analogPin){return displayVoltage(analogPin);}
/*EDU US*/ float LED_Bar::displayVoltagePin(int const analogPin ){
	// Save configuration
		m_voltmeter.pin=analogPin;
	// Set the pin mode if not ok 
		if(!m_voltmeter.pinModeOk){
			pinMode(m_voltmeter.pin,INPUT);
			m_voltmeter.pinModeOk=true;
		}

	// Measure
		m_voltmeter.measureCan=analogRead(m_voltmeter.pin);
		delay(15);

	    return displayVoltage(m_voltmeter.measureCan);
}

float LED_Bar::afficherTension(int valCan){displayVoltage(valCan);}
float LED_Bar::displayVoltage(int valCan){
		// Convertion in volts
		int alimInMv = 5001;
		float resultVolts;
		resultVolts = map(valCan,0,m_voltmeter.range,0,alimInMv);
		m_voltmeter.measureInVolts = resultVolts/1000;
		// /*debug*/ Serial.println(m_voltmeter.measureInVolts);
	
	// Convertion in number of led
		int resultNbrLed;
		resultNbrLed = map(resultVolts,0,alimInMv,0,m_voltmeter.nbrOfLed+1);
		// If change in the number of led
		if(resultNbrLed!=m_voltmeter.measureInNbrOfLed){
			m_voltmeter.measureInNbrOfLed = resultNbrLed;
			// /*debug*/ Serial.println("-----------------------------");
			// /*debug*/ Serial.println(m_voltmeter.measureInNbrOfLed);
	
			// Save result in the ledMemory
			for(char i=0;i<EDU_LEDBAR_NBR_LED;i++){ledNewSet[i]=0;}        // Clear
			for(char i=0;i<m_voltmeter.measureInNbrOfLed;i++){             // Save
				ledNewSet[i]=1;
				// /*debug*/ Serial.println(ledNewSet[i]);
			}
		
			// displaying 
			for(int i=0;i<EDU_LEDBAR_NBR_LED;i++){
				if(ledNewSet[i]!=ledMemory[i]){
					//Toggle
					setSingleLed(9-i,(int)ledNewSet[i]);
					// Save change
					ledMemory[i]=ledNewSet[i];
				}
			}
		} // END if
	
	return m_voltmeter.measureInVolts;
}

