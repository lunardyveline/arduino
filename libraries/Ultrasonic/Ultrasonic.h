/*
  Ultrasonic.h - Ultrasonic driver Library
  2011 Copyright (c) Seeed Technology Inc.  All right reserved.
  For Ultrasonic v1.0 firmware.
 
  Original Author: LG
  
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
  
  EDU : Modifié par DuinoEDU pour compatibiliser Grove et SR04
*/
#ifndef Ultrasonic_H
#define Ultrasonic_H

#define prendreUneMesure  MeasureInCentimeters
#define distance          RangeInCentimeters
#define duree             duration

class Ultrasonic
{
	public:
		Ultrasonic(int pin, int pinTrig=0);

		/*EDU US*/ long RangeInMillimeters;
		
		long RangeInCentimeters;
		long RangeInInches;
		long duration;

		/*EDU FR*/ long mesurerEnMm(void);
        /*EDU US*/ long MeasureInMillimeters(void);
		
		/*EDU FR*/ long mesurer(void);    
		/*ARD US*/ void MeasureInCentimeters(void);
		
		void MeasureInInches(void);
	
	private:
		int _pin;
		int _pinTrig;    // Si Dupont=SR04 (Trig+Echo)
};
#endif
