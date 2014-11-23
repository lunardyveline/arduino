#include <IRremote.h>


IRrecv monRecepteur(3);

void setup(){
  monRecepteur.brancher();
  Serial.begin(9600);
  
}

void loop(){
  
  unsigned long code = monRecepteur.lireCodeIr();
  Serial.println(code);
  delay(500);
}

