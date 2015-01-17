#line 1 "sketch_jan17a.ino"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
void setup();
void loop();
#line 3
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);                            

void setup()
{
  lcd.begin(16,2);                                                                
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();                              

                                                             
                                                   
  lcd.setCursor(0,0);                                 
  lcd.print("Hello, world!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("HI!YourDuino.com");
  delay(8000);  

                                                                                      
                                                           
  lcd.clear();
  lcd.setCursor(0,0);                                 
  lcd.print("Use Serial Mon");
  lcd.setCursor(0,1);
  lcd.print("Type to display");  


}                     


void loop()                                        
{
  {
                                                     
    if (Serial.available()) {
                                                    
      delay(100);
                         
      lcd.clear();
                                          
      while (Serial.available() > 0) {
                                            
        lcd.write(Serial.read());
      }
    }
  }

}                          


                 


