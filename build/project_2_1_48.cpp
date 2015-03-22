#line 1 "project_2_1_48.ino"
#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>


#include "Arduino.h"
void setup();
void loop();
#line 6
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;



void setup(){
    	pinMode(13,OUTPUT);
    
}

void loop(){
    
    digitalWrite(13,1);
    delay(1000*1);
    digitalWrite(13,0);
    delay(1000*1);
    
}


