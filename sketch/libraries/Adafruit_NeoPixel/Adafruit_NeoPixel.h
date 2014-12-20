/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/
  
  /*------------------------------------------------------------------
  Librairie augmentée par DuinoEDU
  Dernières modifications : 10/2014
  -------------------------------------------------------------------*/

#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

// 'type' flags for LED pixels (third parameter to constructor):
#define NEO_RGB     0x00 // Wired for RGB data order
#define NEO_GRB     0x01 // Wired for GRB data order
#define NEO_BRG     0x04
  
#define NEO_COLMASK 0x01
#define NEO_KHZ800  0x02 // 800 KHz datastream
#define NEO_SPDMASK 0x02
// Trinket flash space is tight, v1 NeoPixels aren't handled by default.
// Remove the ifndef/endif to add support -- but code will be bigger.
// Conversely, can comment out the #defines to save space on other MCUs.
#ifndef __AVR_ATtiny85__
#define NEO_KHZ400  0x00 // 400 KHz datastream
#endif



/*EDU FR*/
#define RUBAN_60_LED_GROVE	Adafruit_NeoPixel(60, 8, NEO_GRB + NEO_KHZ800)
#define RUBAN_30_LED_GROVE	Adafruit_NeoPixel(30, 8, NEO_GRB + NEO_KHZ800)

#define STRIP_60_LED_GROVE	Adafruit_NeoPixel(60, 8, NEO_GRB + NEO_KHZ800)
#define STRIP_30_LED_GROVE	Adafruit_NeoPixel(30, 8, NEO_GRB + NEO_KHZ800)

class Adafruit_NeoPixel {

 public:

  // <<Constructor>> : number of LEDs, pin number, LED type
   Adafruit_NeoPixel(uint16_t n=60, uint8_t p=6, uint8_t t=NEO_GRB + NEO_KHZ800);
  ~Adafruit_NeoPixel();


	//----- Start
	/*EDU FR*/	void brancher();
	/*EDU US*/	void branch();


	/*ARD US*/  void	setPin(uint8_t p);
	
	/*EDU FR*/	void 	preparerPixel(uint16_t n, uint16_t r, uint16_t g, uint16_t b);
	/*EDU US*/	void 	setPixel(uint16_t n, uint16_t r, uint16_t g, uint16_t b);
    /*ARD US*/  void	setPixelColor(uint16_t n, uint16_t r, uint16_t g, uint16_t b);

	/*EDU FR*/	void 	ecrirePixel(uint16_t n, uint16_t r, uint16_t g, uint16_t b);
	/*EDU US*/	void 	writePixel(uint16_t n, uint16_t r, uint16_t g, uint16_t b);
	
	/*EDU FR*/	void 	preparerPixel(uint16_t n, uint32_t c);
	/*EDU US*/	void 	setPixel(uint16_t n, uint32_t c);
    /*ARD US*/  void	setPixelColor(uint16_t n, uint32_t c);
   
 	/*EDU US*/	void 	ecrireLuminosite(uint16_t b);  
    /*ARD US*/  void	setBrightness(uint8_t b);
	
	/*EDU US*/  void effacerTout(void);
	/*EDU US*/  void clearAll(void);		
	
		
	/*ARD US*/  void 	begin(void);
	/*EDU FR*/	void  afficher(void) ;
    /*ARD US*/  void 	show(void);
	
		uint8_t
			*getPixels(void) const;
		uint16_t
			numPixels(void) const;
		static uint32_t
			Color(uint8_t r, uint8_t g, uint8_t b);
		uint32_t
			getPixelColor(uint16_t n) const;

		

    
 private:

	uint16_t
		numLEDs,       		// Number of RGB LEDs in strip
		numBytes;      		// Size of 'pixels' buffer below (x3)
	uint8_t
		pin,           			// Output pin number
		brightness,
		*pixels,       			// Holds LED color values (3 bytes each)
		rOffset,       			// Index of red byte within each 3-byte pixel
		gOffset,       			// Index of green byte
		bOffset;       			// Index of blue byte
  
  const uint8_t
		type;          			// Pixel flags (400 vs 800 KHz, RGB vs GRB color)
		uint32_t
		endTime;       		// Latch timing reference

#ifdef __AVR__
  const volatile uint8_t
		*port;         			// Output PORT register
		uint8_t
		pinMask;       		// Output PORT bitmask
#endif

};

#endif // ADAFRUIT_NEOPIXEL_H



/*EDU FR*/ // INFOS ----------------------


