#include <Shieldbot.h>

Shieldbot monBot = Shieldbot();


int S1,S2,S3,S4,S5;

void setup(){
   monBot.setMaxSpeed(50,50);
   monBot.stop();
}

void loop(){
    // Lecture des capteurs
    S1 = monBot.readS1();
    S2 = monBot.readS2();
    S3 = monBot.readS3();
    S4 = monBot.readS4();
    S5 = monBot.readS5();   
    
    if(S3==LOW )
    {
      monBot.forward();
      delay(100);
    } 
    else 
    {
      gererVirage();
    }
        
}

void gererVirage(){
  // Tourner un peu  
  if(S2==LOW){
    monBot.drive(127,0);
    delay(100);
  }
  if(S4==LOW){
    monBot.drive(0,127);
    delay(100);
  }
  
  // Tourner beaucoup
    if(S1==LOW){
    monBot.drive(127,-128);
    delay(100);
  }
  if(S5==LOW){
    monBot.drive(-128,127);
    delay(100);
  }
  
}
   
    
    
    
  
  
  
