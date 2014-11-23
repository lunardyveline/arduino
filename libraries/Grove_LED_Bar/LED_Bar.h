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
#ifndef LED_Bar_H
#define LED_Bar_H

#define CMDMODE     0x0000                          // Work on 8-bit mode
#define ON          0x00ff                          // 8-bit 1 data
#define SHUT        0x0000                          // 8-bit 0 data

// LED par LED
   #define allumerLed(val)       setSingleLed(10-val,1)
   #define eteindreLed(val)      setSingleLed(10-val,0)

// Toutes les LEDs
   // .effacerTout
   // .allumerTout
   #define effacer               effacerTout

// Certaines LEDs
   // .afficher(1,0...)
   
   
class LED_Bar
{

private:

    int __pinClk;                                   // pin of clk
    int __pinDta;                                   // pin of data
    
    unsigned int __led_state;                       // led state
    
private:

    void send16bitData(unsigned int data);          // send a word to led bar
    void latchData(void);                           // latch data
    
public:
    
    LED_Bar(int pinClk, int pinDta);                // initialize
	void effacerTout(void);                         // EDU
	void allumerTout(void);                         // EDU
	void afficher(int a=0, int b=0,int c=0,int d=0,int e=0,int f=0,int g=0,int h=0,int i=0,int j=0);  // EDU
	void ledIndexBit(unsigned int index_bits);      // index bit led
    void setLevel(int level);                       // set level
    void setSingleLed(int num, int st);             // set si



};

#endif
