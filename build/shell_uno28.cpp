#line 1 "shell_uno28.ino"

                     
                                                                          





                                                                                                        
                                                                                                        
                                                                                                        
                                                                                                        
                                                                                                        
                                                                                                         
                                                                                                        
                                                                                                        
                                                                                                        
                                                                                                        

                                                                                                           
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
                                                                                                         
#define Activate_Numeric_Output                                   
#define Activate_Numeric_Input                                    
#define Activate_Analogic_Input                                   
#define Activate_Pwm                                              
#define Activate_Lcd                                                
                                                                      
#define Activate_Generic_I2c                                        
#define Activate_Compiler                                           
                                                                      
#define Activate_Servo_Motor                                        
#define Activate_Buzzer                                           

#if defined(Activate_Servo_Motor)
#include <Servo.h>
#endif
#include <EEPROM.h>
#include <Wire.h>
#if defined(Activate_Ir_Remote)
#include <IRremote.h>
#endif
#if defined(Activate_Lcd)
#include <LiquidCrystal_I2C_TechnoZone51.h>
#endif
                                       
#include <DS1307new.h>
        

#if defined(__mega_ver)
  #define first_pin_eeprom 0                                                             
  #define last_pin_eeprom 70                                                          
  #define first_name_eeprom 71                                                                                           
  #define autorun 77                                                                                                                                                  
  #define lcd_connected 78                                                                                                                              
  #define lcd_i2c_add 79                                                         
  #define run 80                                                                                                                           
                                                                                                                  
                                                                                                            
  #define name_slot 113                                                                                
                                                                                                                           

  #define lcd_col 377                                                           
  #define lcd_row 378                                                        
  #define baud_rate 379                                                                 

  #define first_opcode 410                                                                    
                                                
  #define MEM_ADDR 0x50


#else

#define first_pin_eeprom 0                                                             
#define last_pin_eeprom 19                                                          
#define first_name_eeprom 30                                                                                           
#define autorun 37                                                                                                                                                 
#define lcd_connected 38                                                                                                                              
#define lcd_i2c_add 39                                                         
#define run 40                                                                                                                           
                                                                                                                
                                                                                                          
#define name_slot 49                                                                               
                                                                                                                         
#define lcd_col 130                                                          
#define lcd_row 131                                                        
#define baud_rate 132                                                                 

#define first_opcode 140                                                                    
#define max_slot 8                                              


                                              
#define MEM_ADDR 0x50

#endif


                                                   
#if defined(__mega_ver)
#include "Arduino.h"
void buzz(int targetpin,long frequency,long length);
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data );
unsigned long g(byte j,byte m2, byte y2);
unsigned long diff_date(byte j1,byte m1, byte y1, byte j2, byte m2, byte y2);
void duree(byte h1,byte h2,byte h3);
byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress );
byte get_bank(unsigned int slot);
byte lire_eeprom(unsigned int slot, unsigned int eeaddress );
void affiche_celsius(int valeur);
void affiche_lux(int valeur);
void identification();
char ith(int value);
int lire_octet();
word get_first_pc(unsigned int no);
void interpreteur();
void fixbaudrate();
void setup();
void loop();
#line 109
boolean debug=0;                                                        
#endif

uint8_t hor;                                     
int incomingByte = 0;	                           
boolean time_out;                          
int no=0;                                              
int n1;
int n2;
int n3;
int n4;
int n5;
int n6;
int n7;
int n8;
int n9;
int n10;
int n11;
int n12;
int n13;
int value=0;                                   
int digit_u=0;                                        
int digit_d=0;                                          
int digit_c=0;                                           
unsigned long time_d;                                                                      
unsigned long time_f;                                                                   
                                                                                                    
byte slot_p;                                                                           
word pc;                                                                                                  

#if defined(__mega_ver)
 word pcs[33];                                                              
 int variable[26];                                                                                                   
 int variables[828];                                                                      
 unsigned long intempo[33];                                                                                
                                                             
#if defined(Activate_Servo_Motor)
 Servo myservo14;
 Servo myservo15;
 Servo myservo16;
 Servo myservo17;
 Servo myservo18;
 Servo myservo19;
 Servo myservo22;
 Servo myservo23;
 Servo myservo24;
 Servo myservo25;
 Servo myservo26;
 Servo myservo27;
#endif 
#else
word pcs[9];                                                             
int variable[26];                                                                                                   
int variables[234];                                                                    
unsigned long intempo[9];                                                                                
#if defined(Activate_Servo_Motor)
 Servo myservo4;
 Servo myservo5;
 Servo myservo6;
 Servo myservo7;
 Servo myservo8;
 Servo myservo9;
 Servo myservo10;
 Servo myservo11;
 Servo myservo12;
 Servo myservo13;
#endif 
#endif

                            
#if defined(Activate_Lcd)
LiquidCrystal_I2C lcd(EEPROM.read(lcd_i2c_add),EEPROM.read(lcd_col),EEPROM.read(lcd_row));                                                                  
#endif
                                                                
#if defined(Activate_Ir_Remote)
int RECV_PIN = 11;                                            
IRrecv irrecv(RECV_PIN);
decode_results results;
#endif
                                                  
#if defined(Activate_Buzzer)
void buzz(int targetpin,long frequency,long length){
 long delayvalue=1000000/frequency/2;                                         
 long numcycles=frequency*length/1000;                               
 for (long i=0;i<numcycles;i++){
   digitalWrite(targetpin,HIGH);
   delayMicroseconds(delayvalue);
   digitalWrite(targetpin,LOW);
   delayMicroseconds(delayvalue);
 }  
}
#endif
                                         
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((byte)(eeaddress >> 8));       
  Wire.write((byte)(eeaddress & 0xFF));       
  Wire.write(rdata);
  Wire.endTransmission();
}

                                                       
#if defined(Activate_Real_Time_Clock)
unsigned long g(byte j,byte m2, byte y2) {
 byte m=(m2 +9)%12;
 unsigned long y=(y2+2000)-m/10;
 return 365*y+y/4-y/100+y/400+(m*306+5)/10+(j-1); 
}  
#endif

                                                    
#if defined(Activate_Real_Time_Clock)
unsigned long diff_date(byte j1,byte m1, byte y1, byte j2, byte m2, byte y2) {
  return g(j2,m2,y2)-g(j1,m1,y1);
}  
#endif

                            
#if defined(Activate_Real_Time_Clock)
void duree(byte h1,byte h2,byte h3){
 byte retenu=0;
 byte a;
 byte b;
                                       
 RTC.getRAM(h2*7+5,&hor,1);
 a=hor;
 RTC.getRAM(h1*7+5,&hor,1);
 b=hor;
 if (b>a) 
 {
   hor=a+60-b;
   retenu=1;
 } else
 {
   hor=a-b;
   retenu=0;
 }
 RTC.setRAM(h3*7+5,&hor,1);
                                      
 RTC.getRAM(h2*7+4,&hor,1);
 a=hor;
 RTC.getRAM(h1*7+4,&hor,1);
 b=hor+retenu;
 if (b>a) 
 {
   hor=a+60-b;
   retenu=1;
 } else
 {
   hor=a-b;
   retenu=0;
 }
 RTC.setRAM(h3*7+4,&hor,1);
                                     
 RTC.getRAM(h2*7+3,&hor,1);
 a=hor;
 RTC.getRAM(h1*7+3,&hor,1);
 b=hor+retenu;
 if (b>a) 
 {
   hor=a+24-b;
   retenu=1;
 } else
 {
   hor=a-b;
   retenu=0;
 }
 RTC.setRAM(h3*7+3,&hor,1);
                                   
 byte j1,j2,m1,m2,a1,a2;
 RTC.getRAM(h1*7,&hor,1);
 j1=hor;
 RTC.getRAM(h1*7+1,&hor,1);
 m1=hor;
 RTC.getRAM(h1*7+2,&hor,1);
 a1=hor;
 RTC.getRAM(h2*7,&hor,1);
 j2=hor;
 RTC.getRAM(h2*7+1,&hor,1);
 m2=hor;
 RTC.getRAM(h2*7+2,&hor,1);
 a2=hor;
 hor=(byte)diff_date(j1,m1,a1,j2,m2,a2)-retenu;
 RTC.setRAM(h3*7,&hor,1);
 hor=0;
 RTC.setRAM(h3*7+1,&hor,1);
 RTC.setRAM(h3*7+2,&hor,1);
 RTC.setRAM(h3*7+6,&hor,1);
}  
#endif

                                        
byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((byte)(eeaddress >> 8));       
  Wire.write((byte)(eeaddress & 0xFF));       
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) {
   rdata = Wire.read();
  }
                                   
                              
                      
                            
                 
  return rdata;
}

                                                                        
byte get_bank(unsigned int slot) {
  if (slot<9) { return 0; }
  if ((slot>8)&&(slot<17)) {return 1;}
  if ((slot>16)&&(slot<25)) {return 2;}
  if (slot>24) {return 3;}
 }

                                                                       
byte lire_eeprom(unsigned int slot, unsigned int eeaddress ) {
  if (slot==0) {
                                                     
  return EEPROM.read(eeaddress);
  } else {
                                                            
  return i2c_eeprom_read_byte(MEM_ADDR+get_bank(slot), eeaddress);
  } 
}

                                                              
#if defined(Activate_Lcd)
void affiche_celsius(int valeur) {
 float temperature=valeur*-0.525641025+116.0897436;
 if (temperature>=0) {
                          
   lcd.print("+");
   }
   lcd.print(temperature);
   lcd.write(223);
   lcd.print('C');
}
#endif

                                                              
#if defined(Activate_Lcd)
void affiche_lux(int valeur) {
  int lux;
  switch (valeur) {
   case 0 : lux=1517; 
            break; 
   case 1 : lux=1477; 
            break;
   case 2 : lux=1438; 
            break;
   case 3 : lux=1399; 
            break;
   case 4 : lux=1362; 
            break;
   case 5 : lux=1326; 
            break;
   case 6 : lux=1291; 
            break;
   case 7 : lux=1256; 
            break;
   case 8 : lux=1223; 
            break;
   case 9 : lux=1190; 
            break;
   case 10 : lux=1159; 
            break;
   case 11 : lux=1128; 
            break;
   case 12 : lux=1098; 
            break;
   case 13 : lux=1069; 
            break;
   case 14 : lux=1040; 
            break;
   case 15 : lux=1013; 
            break;
   case 16 : lux=986; 
            break;
   case 17 : lux=960; 
            break;
   case 18 : lux=934; 
            break;
   case 19 : lux=909; 
            break;
   case 20 : lux=885; 
            break;
   case 21 : lux=861; 
            break;
   case 22 : lux=839; 
            break;
   case 23 : lux=816; 
            break;
   case 24 : lux=795; 
            break;
   case 25 : lux=773; 
            break;
   case 26 : lux=753; 
            break;
   case 27 : lux=733; 
            break;
   case 28 : lux=713; 
            break;
   case 29 : lux=694; 
            break;
   case 30 : lux=676; 
            break;
   case 31 : lux=658; 
            break;
   case 32 : lux=640; 
            break;
   case 33 : lux=623; 
            break;
   case 34 : lux=607; 
            break;
   case 35 : lux=591; 
            break;
   case 36 : lux=575; 
            break;
   case 37 : lux=560; 
            break;
   case 38 : lux=545; 
            break;
   case 39 : lux=530; 
            break;
   case 40 : lux=516; 
            break;
   case 41 : lux=502; 
            break;
   case 42 : lux=489; 
            break;
   case 43 : lux=476; 
            break;
   case 44 : lux=463; 
            break;
   case 45 : lux=451; 
            break;
   case 46 : lux=439; 
            break;
   case 47 : lux=427; 
            break;
   case 48 : lux=416; 
            break;
   case 49 : lux=405; 
            break;
   case 50 : lux=394; 
            break;
   case 51 : lux=384; 
            break;
   case 52 : lux=374; 
            break;
   case 53 : lux=364; 
            break;
   case 54 : lux=354; 
            break;
   case 55 : lux=345; 
            break;
   case 56 : lux=335; 
            break;
   case 57 : lux=326; 
            break;
   case 58 : lux=318; 
            break;
   case 59 : lux=309; 
            break;
   case 60 : lux=301;   
            break;
   case 61 : lux=293; 
            break;
   case 62 : lux=285; 
            break;
   case 63 : lux=278; 
            break;
   case 64 : lux=270; 
            break;
   case 65 : lux=263; 
            break;
   case 66 : lux=256; 
            break;
   case 67 : lux=249; 
            break;
   case 68 : lux=243; 
            break;
   case 69 : lux=236; 
            break;
   case 70 : lux=230;   
            break;
   case 71 : lux=224; 
            break;
   case 72 : lux=218; 
            break;
   case 73 : lux=212; 
            break;
   case 74 : lux=206; 
            break;
   case 75 : lux=201; 
            break;
   case 76 : lux=196; 
            break;
   case 77 : lux=190; 
            break;
   case 78 : lux=185; 
            break;
   case 79 : lux=180; 
            break;
   case 80 : lux=176;   
            break;
   case 81 : lux=171; 
            break;
   case 82 : lux=166; 
            break;
   case 83 : lux=162; 
            break;
   case 84 : lux=158; 
            break;
   case 85 : lux=153; 
            break;
   case 86 : lux=149; 
            break;
   case 87 : lux=145; 
            break;
   case 88 : lux=142; 
            break;
   case 89 : lux=138; 
            break;
   case 90 : lux=134;   
            break;
   case 91 : lux=131; 
            break;
   case 92 : lux=127; 
            break;
   case 93 : lux=124; 
            break;
   case 94 : lux=120; 
            break;
   case 95 : lux=117; 
            break;
   case 96 : lux=114; 
            break;
   case 97 : lux=111; 
            break;
   case 98 : lux=108; 
            break;
   case 99 : lux=105; 
            break;
   case 100 : lux=102;   
            break;
   case 101 : lux=100; 
            break;
   case 102 : lux=97; 
            break;
   case 103 : lux=94; 
            break;
   case 104 : lux=92; 
            break;
   case 105 : lux=90; 
            break;
   case 106 : lux=87; 
            break;
   case 107 : lux=85; 
            break;
   case 108 : lux=83; 
            break;
   case 109 : lux=80; 
            break;
   case 110 : lux=78; 
            break;
   case 111 : lux=76; 
            break;
   case 112 : lux=74; 
            break;
   case 113 : lux=72; 
            break;
   case 114 : lux=70; 
            break;
   case 115 : lux=68; 
            break;
   case 116 : lux=67; 
            break;
   case 117 : lux=65; 
            break;
   case 118 : lux=63; 
            break;
   case 119 : lux=61; 
            break;
   case 120 : lux=60; 
            break;
   case 121 : lux=58; 
            break;
   case 122 : lux=57; 
            break;
   case 123 : lux=55; 
            break;
   case 124 : lux=54; 
            break;
   case 125 : lux=52; 
            break;
   case 126 : lux=51; 
            break;
   case 127 : lux=49; 
            break;
   case 128 : lux=48; 
            break;
   case 129 : lux=47; 
            break;
   case 130 : lux=46; 
            break;
   case 131 : lux=44; 
            break;
   case 132 : lux=43; 
            break;
   case 133 : lux=42; 
            break;
   case 134 : lux=41; 
            break;
   case 135 : lux=40; 
            break;
   case 136 : lux=39; 
            break;
   case 137 : lux=38; 
            break;
   case 138 : lux=37; 
            break;
   case 139 : lux=36; 
            break;
   case 140 : lux=35; 
            break;
   case 141 : lux=34; 
            break;
   case 142 : lux=33; 
            break;
   case 143 : lux=32; 
            break;
   case 144 : lux=31; 
            break;
   case 145 : lux=30; 
            break;
   case 146 : lux=30; 
            break;
   case 147 : lux=29; 
            break;
   case 148 : lux=28; 
            break;
   case 149 : lux=27; 
            break;
   case 150 : lux=27; 
            break;
   case 151 : lux=26; 
            break;
   case 152 : lux=25; 
            break;
   case 153 : lux=25; 
            break;
   case 154 : lux=24; 
            break;
   case 155 : lux=23; 
            break;
   case 156 : lux=23; 
            break;
   case 157 : lux=22; 
            break;
   case 158 : lux=21; 
            break;
   case 159 : lux=21; 
            break;
   case 160 : lux=20;   
            break;
   case 161 : lux=20; 
            break;
   case 162 : lux=19; 
            break;
   case 163 : lux=19; 
            break;
   case 164 : lux=18; 
            break;
   case 165 : lux=18; 
            break;
   case 166 : lux=17; 
            break;
   case 167 : lux=17; 
            break;
   case 168 : lux=16; 
            break;
   case 169 : lux=16; 
            break;
   case 170 : lux=16;   
            break;
   case 171 : lux=15; 
            break;
   case 172 : lux=15; 
            break;
   case 173 : lux=14; 
            break;
   case 174 : lux=14; 
            break;
   case 175 : lux=14; 
            break;
   case 176 : lux=13; 
            break;
   case 177 : lux=13; 
            break;
   case 178 : lux=13; 
            break;
   case 179 : lux=12; 
            break;
   case 180 : lux=12;   
            break;
   case 181 : lux=12; 
            break;
   case 182 : lux=11; 
            break;
   case 183 : lux=11; 
            break;
   case 184 : lux=11; 
            break;
   case 185 : lux=10; 
            break;
   case 186 : lux=10; 
            break;
   case 187 : lux=10; 
            break;
   case 188 : lux=10; 
            break;
   case 189 : lux=9; 
            break;
   case 190 : lux=9;   
            break;
   case 191 : lux=9; 
            break;
   case 192 : lux=9; 
            break;
   case 193 : lux=8; 
            break;
   case 194 : lux=8; 
            break;
   case 195 : lux=8; 
            break;
   case 196 : lux=8; 
            break;
   case 197 : lux=7; 
            break;
   case 198 : lux=7; 
            break;
   case 199 : lux=7; 
            break;
   case 200 : lux=7;   
            break;
   case 201 : lux=7; 
            break;
   case 202 : lux=7; 
            break;
   case 203 : lux=6; 
            break;
   case 204 : lux=6; 
            break;
   case 205 : lux=6; 
            break;
   case 206 : lux=6; 
            break;
   case 207 : lux=6; 
            break;
   case 208 : lux=6; 
            break;
   case 209 : lux=5; 
            break;
   case 210 : lux=5; 
            break;
   case 211 : lux=5; 
            break;
   case 212 : lux=5; 
            break;
   case 213 : lux=5; 
            break;
   case 214 : lux=5; 
            break;
   case 215 : lux=5; 
            break;
   case 216 : lux=4; 
            break;
   case 217 : lux=4; 
            break;
   case 218 : lux=4; 
            break;
   case 219 : lux=4; 
            break;
   case 220 : lux=4; 
            break;
   case 221 : lux=4; 
            break;
   case 222 : lux=4; 
            break;
   case 223 : lux=4; 
            break;
   case 224 : lux=4; 
            break;
   case 225 : lux=4; 
            break;
   case 226 : lux=3; 
            break;
   case 227 : lux=3; 
            break;
   case 228 : lux=3; 
            break;
   case 229 : lux=3; 
            break;
   case 230 : lux=3; 
            break;
   case 231 : lux=3; 
            break;
   case 232 : lux=3; 
            break;
   case 233 : lux=3; 
            break;
   case 234 : lux=3; 
            break;
   case 235 : lux=3; 
            break;
   case 236 : lux=3; 
            break;
   case 237 : lux=3; 
            break;
   case 238 : lux=2; 
            break;
   case 239 : lux=2; 
            break;
   case 240 : lux=2; 
            break;
   case 241 : lux=2; 
            break;
   case 242 : lux=2; 
            break;
   case 243 : lux=2; 
            break;
   case 244 : lux=2; 
            break;
   case 245 : lux=2; 
            break;
   case 246 : lux=2; 
            break;
   case 247 : lux=2; 
            break;
   case 248 : lux=2; 
            break;
   case 249 : lux=2; 
            break;
   case 250 : lux=2; 
            break;
   case 251 : lux=2; 
            break;
   case 252 : lux=2; 
            break;
   case 253 : lux=2; 
            break;
   case 254 : lux=2; 
            break;
   case 255 : lux=2; 
            break;
  }
 if (lux>=0) {
                          
   lcd.print("+");
   }
   lcd.print(lux);
   lcd.print(" Lx");
}  
#endif

                                                                                              
void identification() {
  Serial.print("MODEL NAME : ");
  n1=EEPROM.read(first_name_eeprom);
  n2=EEPROM.read(first_name_eeprom+1);
  n3=EEPROM.read(first_name_eeprom+2);
  n4=EEPROM.read(first_name_eeprom+3);
  n5=EEPROM.read(first_name_eeprom+4);
  n6=EEPROM.read(first_name_eeprom+5);
  Serial.write(n1);
  Serial.write(n2);
  Serial.write(n3);
  Serial.write(n4);
  Serial.write(n5);
  Serial.write(n6);
  Serial.println("");
                                    
}  

char ith(int value) {
 switch(value) {
  case 32 : return ' ';
            break;
  case 48 : return '0';
            break;
  case 49 : return '1';
            break;
  case 50 : return '2';
            break;
  case 51 : return '3';
            break;
  case 52 : return '4';
            break;
  case 53 : return '5';
            break;
  case 54 : return '6';
            break;
  case 55 : return '7';
            break;
  case 56 : return '8';
            break;
  case 57 : return '9';
            break;
  case 65 : return 'A';
            break;
  case 66 : return 'B';
            break;
  case 67 : return 'C';
            break;
  case 68 : return 'D';
            break;
  case 69 : return 'E';
            break;
  case 70 : return 'F';
            break;

 } 
}
                                                                                        
                                                                                                                     
                                                                                                                              
                                                                                                                                  
              
int lire_octet() {
   time_d=millis();
   while(Serial.available()==0 && !time_out) {
      time_f=millis();
      if ((time_f-time_d)>100) {time_out=true;}
      }
   if (!time_out) { return Serial.read(); } else { return -1; }
}

word get_first_pc(unsigned int no) {
#if defined(__mega_ver)
 switch (no) {
 case 0 : return first_opcode;
          break;
 case 1 : return 0;
          break;
 case 2 : return 4096;
          break;
 case 3 : return 8192; 
          break;
 case 4 : return 12288;
          break;
 case 5 : return 16384;
          break;
 case 6 : return 20480;
          break;
 case 7 : return 24576;
          break;
 case 8 : return 28672;
          break;
 case 9 : return 0;
          break;
 case 10 : return 4096;
          break;
 case 11 : return 8192; 
          break;
 case 12 : return 12288;
          break;
 case 13 : return 16384;
          break;
 case 14 : return 20480;
          break;
 case 15 : return 24576;
          break;
 case 16 : return 28672;
          break;
 case 17 : return 0;
          break;
 case 18 : return 4096;
          break;
 case 19 : return 8192; 
          break;
 case 20 : return 12288;
          break;
 case 21 : return 16384;
          break;
 case 22 : return 20480;
          break;
 case 23 : return 24576;
          break;
 case 24 : return 28672;
          break;
 case 25 : return 0;
          break;
 case 26 : return 4096;
          break;
 case 27 : return 8192; 
          break;
 case 28 : return 12288;
          break;
 case 29 : return 16384;
          break;
 case 30 : return 20480;
          break;
 case 31 : return 24576;
          break;
 case 32 : return 28672;
          break;
  }
#else
 switch (no) {
 case 0 : return first_opcode;
          break;
 case 1 : return 0;
          break;
 case 2 : return 4096;
          break;
 case 3 : return 8192; 
          break;
 case 4 : return 12288;
          break;
 case 5 : return 16384;
          break;
 case 6 : return 20480;
          break;
 case 7 : return 24576;
          break;
 case 8 : return 28672;
          break;
 }         
#endif
}
                                                                                                        
                                                                                                        
                                                                                                        
#if defined(Activate_Compiler)
void interpreteur() {
                                                                                                  
#if defined(__mega_ver)
  if (debug) {Serial.println("Entr\u00e9e interpr\u00e9teur");}
#endif  
  for (int i=0;i<33;i++) { 
    pcs[i]=get_first_pc(i);
    intempo[i]=0;
    }
                             
                                                                   
                                       
                                     
                                     
                                    
                                            
                 
                
          
                                                                         
  while (Serial.available()>0) {
   value=Serial.read(); 
  }
  boolean fin=false;                                                          
  
  while (!fin) {
   if (Serial.available()>0) {fin=true;}                                                                                  
                            
                                                           
           
   for (int slot=0;slot<33;slot++)
   {
                              
                   
                                                
                                  
          
               
                                                         
     if (EEPROM.read(run+slot)==1) {
                                               
                                                                          
      pc=pcs[slot];
      for (int i=0;i<26;i++) {
        variable[i]=variables[slot*26+i];
      }
      #if defined(__mega_ver)
      if (debug) {
        Serial.print("Execution programme en slot memoire no ");
        Serial.println(slot,DEC);
      }
      #endif
  if (Serial.available()>0) {fin=true;}                                                                                 
  #if defined(__mega_ver)
  if (debug) {
    delay(500);
    Serial.println("*********************************************"); 
    Serial.print("Interpretation opcode ");
    Serial.print(lire_eeprom(slot,pc),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+1),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+2),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+3),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+4),DEC);
    Serial.print(";");
    Serial.print(lire_eeprom(slot,pc+5),DEC);
    Serial.print(" a l'adresse ");
    Serial.println(pc,DEC);
  }
  #endif
  if (intempo[slot]==0) 
  {  
  switch (lire_eeprom(slot,pc)) {
     case 0 :                
              #if defined(__mega_ver)
              if (debug) { Serial.println("NOP"); }
              #endif 
              pc=pc+1;
              break;
     case 1 :                                         
                                       
                             
                                                       
                                    
                                                                                        
                 
                       
              variable[lire_eeprom(slot,pc+1)-65]=lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3);
              pc=pc+4;
              break;
     case 2 :                             
                                      
                            
                                                       
                                    
                                                       
                                       
                 
                      
              variable[lire_eeprom(slot,pc+1)-65]=variable[lire_eeprom(slot,pc+1)-65]+1;
              pc=pc+2;
              break;
     case 3 :                             
                                       
                             
                                                       
                                    
                                                       
                                       
                 
                                   
              variable[lire_eeprom(slot,pc+1)-65]=variable[lire_eeprom(slot,pc+1)-65]-1;
              pc=pc+2;
              break;
     #if defined(Activate_Analogic_Input)         
     case 4 :                                                                                
                                       
                             
                                                            
                                                           
                                                                          
                                                       
                                     
                 
                       
              variable[lire_eeprom(slot,pc+2)-65]=analogRead(lire_eeprom(slot,pc+1))/4;
              pc=pc+3;
              break;
     #endif
     #if defined(Activate_Pwm)     
     case 5 :                                                        
                                       
                             
                                                      
                                                                   
                                                                                  
                                                                
                                             
                         
                              
              analogWrite(lire_eeprom(slot,pc+1),variable[lire_eeprom(slot,pc+2)-65]); 
              pc=pc+3;
              break;
     #endif
     #if defined(Activate_Pwm)     
     case 6 :                                      
                                       
                             
                                                      
                                                                   
                                                       
                                                                      
                         
                              
              analogWrite(lire_eeprom(slot,pc+1),lire_eeprom(slot,pc+2)); 
              pc=pc+3;
              break;
     #endif
     #if defined(Activate_Numeric_Output)     
     case 7 :                                      
              if (lire_eeprom(slot,pc+2)==0) {
                      digitalWrite(lire_eeprom(slot,pc+1), LOW);                         
                                               
                                     
                                                            
                                                                   
                                                        
                         
                              
                    } 
                    else
                    {
                      digitalWrite(lire_eeprom(slot,pc+1), HIGH);                         
                                               
                                     
                                                            
                                                                   
                                                        
                         
                              
                    }                 
              pc=pc+3;
              break;
     #endif
     #if defined(Activate_Servo_Motor)     
     case 8 :                                                
                                       
                             
                                                        
                                                                   
                                                       
                                                                      
                         
                      
              #if defined(__mega_ver)        
              switch(lire_eeprom(slot,pc+1)) {
                    case 14 :
                            myservo14.write(lire_eeprom(slot,pc+2));
                            break;
                    case 15 :
                            myservo15.write(lire_eeprom(slot,pc+2));
                            break;
                    case 16 :
                            myservo16.write(lire_eeprom(slot,pc+2));
                            break;
                    case 17 :
                            myservo17.write(lire_eeprom(slot,pc+2));
                            break;
                    case 18 :
                            myservo18.write(lire_eeprom(slot,pc+2));
                            break;
                    case 19 :
                            myservo19.write(lire_eeprom(slot,pc+2));
                            break;
                    case 22 :
                            myservo22.write(lire_eeprom(slot,pc+2));
                            break;
                    case 23 :
                            myservo23.write(lire_eeprom(slot,pc+2));
                            break;
                    case 24 :
                            myservo24.write(lire_eeprom(slot,pc+2));
                            break;
                    case 25 :
                            myservo25.write(lire_eeprom(slot,pc+2));
                            break;        
                    case 26 :
                            myservo26.write(lire_eeprom(slot,pc+2));
                            break;        
                    case 27 :
                            myservo27.write(lire_eeprom(slot,pc+2));
                            break;        
                            
                     }               
                   #else
                   switch(lire_eeprom(slot,pc+1)) {
                    case 4 :
                            myservo4.write(lire_eeprom(slot,pc+2));
                            break;
                    case 5 :
                            myservo5.write(lire_eeprom(slot,pc+2));
                            break;
                    case 6 :
                            myservo6.write(lire_eeprom(slot,pc+2));
                            break;
                    case 7 :
                            myservo7.write(lire_eeprom(slot,pc+2));
                            break;
                    case 8 :
                            myservo8.write(lire_eeprom(slot,pc+2));
                            break;
                    case 9 :
                            myservo9.write(lire_eeprom(slot,pc+2));
                            break;
                    case 10 :
                            myservo10.write(lire_eeprom(slot,pc+2));
                            break;
                    case 11 :
                            myservo11.write(lire_eeprom(slot,pc+2));
                            break;
                    case 12 :
                            myservo12.write(lire_eeprom(slot,pc+2));
                            break;
                    case 13 :
                            myservo13.write(lire_eeprom(slot,pc+2));
                            break;        
                     }               
                   #endif  
                   pc=pc+3;
                   break;  
     #endif
     #if defined(Activate_Servo_Motor)     
     case 9 :                                                                  
                                       
                             
                                                        
                                                                   
                                                                                  
                                                                
                                             
                         
                      
              #if defined(__mega_ver)        
              switch(lire_eeprom(slot,pc+1)) {
                    case 14 :
                            myservo14.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 15 :
                            myservo15.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 16 :
                            myservo16.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 17 :
                            myservo17.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 18 :
                            myservo18.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 19 :
                            myservo19.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 22 :
                            myservo22.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 23 :
                            myservo23.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 24 :
                            myservo24.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 25 :
                            myservo25.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                    case 26 :
                            myservo26.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                    case 27 :
                            myservo27.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                            
                     }               
                   #else
                   switch(lire_eeprom(slot,pc+1)) {
                    case 4 :
                            myservo4.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 5 :
                            myservo5.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 6 :
                            myservo6.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 7 :
                            myservo7.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 8 :
                            myservo8.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 9 :
                            myservo9.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 10 :
                            myservo10.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 11 :
                            myservo11.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 12 :
                            myservo12.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;
                    case 13 :
                            myservo13.write(variable[lire_eeprom(slot,pc+2)-65]);
                            break;        
                     }                
                   #endif  
                   pc=pc+3;
                   break;  
     #endif
     #if defined(Activate_Numeric_Input)     
     case 10 :                                                                                                                 
                                       
                            
                                                          
                                                          
                                                    
                                                            
                 
                      
              value = digitalRead(lire_eeprom(slot,pc+1));                                                               
              if (value==lire_eeprom(slot,pc+2))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+3)*256+lire_eeprom(slot,pc+4);
               pc=pc+no+4;
              } else
              {
                                                                             
                                        
                             
                                                                                           
                  
                       
              pc=pc+5; 
              }
              break;
     #endif         
     case 11 :                                                           
                                      
                            
                                                     
                                                                   
                                              
                                                                                                 
                         
                              
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              if (value==(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
                                        
                             
                                                                                           
                  
                       
               
                                                                             
              pc=pc+6; 
              }
              break;
     case 12 :                                                           
                                      
                            
                                                     
                                                               
                                              
                                                                                                 
                         
                              
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
                                       
                            
                                                     
                                              
                                              
                                                                                                 
                         
                              
              if (value<(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
                                                                        
               #if defined(__mega_ver)
               if (debug) {
                Serial.print("Le test est positif, on saute de ");
                Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
                Serial.println(" Octets"); 
               }
               #endif
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
                                                                             
               #if defined(__mega_ver)
               if (debug) {
                Serial.println("Le test est negatif, on execute l'instruction suivante");
               }
              #endif 
              pc=pc+6; 
              }
              break;
     case 13 :                                                           
                                       
                             
                                                     
                                                                   
                                              
                                                                                                 
                         
                              
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              if (value>(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
                                                                             
                                        
                             
                                                                                           
                  
                       
              pc=pc+6; 
              }
              break;
      case 14 :                                                            
                                       
                             
                                                     
                                                                   
                                               
                                                                                                 
                         
                              
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              if (value<=(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
                                                                             
                                        
                             
                                                                                           
                  
                       
              pc=pc+6; 
              }
              break;
       case 15 :                                                            
                                       
                             
                                                     
                                                                   
                                               
                                                                                                 
                         
                              
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              if (value>=(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
                                                                             
                                        
                             
                                                                                           
                  
                       
              pc=pc+6;
              }
              break;
       case 16 :                                                            
                                       
                             
                                                     
                                                                   
                                               
                                                                                                 
                         
                              
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              if (value!=(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3)))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } 
              else
              {
                                                                             
                                        
                             
                                                                                           
                  
                       
              pc=pc+6; 
              }
              break;
     case 17 :                              
              intempo[slot]=millis()+(unsigned int)(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2));                                                                                     
                                                                                                          
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Temporisation de ");
                        Serial.print((unsigned int)(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2)),DEC);
                        Serial.println(" ms"); 
                      }
              #endif        
              pc=pc+3;
              break;
     case 18 :                                  
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Saut a l'adresse absolue ");
                        Serial.println(lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2),DEC);
                        }
              #endif          
              no=lire_eeprom(slot,pc+1)*256+lire_eeprom(slot,pc+2);                  
              if (slot==0) {
                                                         
                pc=first_opcode+no-1;
              } else {
                                                        
                pc=get_first_pc(slot)+no-1;
              }  
              break;
     #if defined(Activate_Ir_Remote)         
     case 19 :                                                                                                                                                   
                                          
                                
                                                                  
                         
                              
                 if (irrecv.decode(&results)) {
                                                                                                                                        
                 String s = String(long(results.value),HEX);
                 s.toUpperCase();                           
                 String s2=String(' ');
                 while (s.length()<8) {
                                                                 
                   s.concat(s2);                      
                    } 
                                          
                                
                                                      
                                          
                                                           
                         
                              
                                                                             
                 String s3=(String)ith(lire_eeprom(slot,pc+1))+(String)ith(lire_eeprom(slot,pc+2))+(String)ith(lire_eeprom(slot,pc+3))+(String)ith(lire_eeprom(slot,pc+4))+(String)ith(lire_eeprom(slot,pc+5))+(String)ith(lire_eeprom(slot,pc+6))+(String)ith(lire_eeprom(slot,pc+7))+(String)ith(lire_eeprom(slot,pc+8));
                                                               
                              
                 s3.toUpperCase();
                                          
                                
                                                                      
                                             
                         
                              
                 if (s.equals(s3)) {
                                                                           
                  no=lire_eeprom(slot,pc+9)*256+lire_eeprom(slot,pc+10);
                                           
                                 
                                                                            
                                               
                                                     
                         
                              
                  pc=pc+no+10;
                 } else {
                                                                                
                                            
                                  
                                                                                                 
                           
                                
                   pc=pc+11;    
                 }
                 } else {
                                                
                                          
                                
                                                                                                      
                         
                              
                 if (lire_eeprom(slot,pc+1)==45 & lire_eeprom(slot,pc+2)==49) {
                                                                           
                  no=lire_eeprom(slot,pc+9)*256+lire_eeprom(slot,pc+10);
                                           
                                 
                                                                            
                                               
                                                     
                         
                              
                   pc=pc+no+10;
                 } else {
                                                                                
                                            
                                  
                                                                                                 
                           
                                
                   pc=pc+11;
                 } 
                 }  
                break;
     #endif
     #if defined(Activate_Ir_Remote)     
     case 20 :                              
                                       
                             
                                                                                 
                           
                                
              irrecv.resume();                          
                                
              pc=pc+1; 
              break;           
     #endif
     #if defined(Activate_Lcd)     
     case 21 :                           
                                       
                             
                                                                
                           
                                
              lcd.clear();
              pc=pc+1; 
              break;         
     #endif         
     #if defined(Activate_Lcd)
     case 22 :                                                                  
                                       
                             
                                                                           
                                                                   
                                            
                                                                   
                                               
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+1),lire_eeprom(slot,pc+2));        
              pc=pc+3;
              break;
     #endif
     #if defined(Activate_Lcd)
     case 23 :                                                                                           
                                       
                             
                                                                                       
                         
                              
              n3=1;        
              do
                { 
                  n2 = lire_eeprom(slot,pc+n3);
                  if (n2!=124) {lcd.write(n2);}
                  n3++;
                } while (n2!=124);
              pc=pc+n3;
              break;
     #endif
     #if defined(Activate_Lcd)     
     case 24 :                                                                                                      
                                       
                             
                                                             
                                                                   
                                               
                                                                   
                                            
                                                                   
                                                          
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("   ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              lcd.print(value);
              pc=pc+4;
              break;
      #endif        
      #if defined(Activate_Lcd)        
      case 25 :                                                                                        
                                       
                             
                                                            
                                                                   
                                               
                                                                   
                                            
                                                                   
                                                          
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("   ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = lire_eeprom(slot,pc+1);                            
              lcd.print(value);
              pc=pc+4;
              break;
      #endif
      #if defined(Activate_Lcd)      
      case 26 :                                                                                                                       
                                       
                             
                                                                                    
                                                                   
                                                          
                         
                              
              value = lire_eeprom(slot,pc+1);           
              RTC.getRAM(value*7,&hor,1);             
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write('/');
              RTC.getRAM(value*7+1,&hor,1);            
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write('/');
              RTC.getRAM(value*7+2,&hor,1);             
              lcd.write((hor/10)+48);         
              lcd.write((hor%10)+48);
              pc=pc+2;
              break;        
      #endif        
      #if defined(Activate_Lcd)        
      case 27 :                                                                                                                       
                                       
                             
                                                                                    
                                                                   
                                                          
                         
                              
              value = lire_eeprom(slot,pc+1);           
              RTC.getRAM(value*7+3,&hor,1);                 
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write(':');
              RTC.getRAM(value*7+4,&hor,1);                     
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              lcd.write(':');
              RTC.getRAM(value*7+5,&hor,1);                     
              lcd.write((hor/10)+48);
              lcd.write((hor%10)+48);
              pc=pc+2;
              break;
      #endif
      #if defined(Activate_Real_Time_Clock)      
      case 28 :                                                                                                            
                                       
                             
                                                                                                    
                                                                     
                           
                                
              value = lire_eeprom(slot,pc+1);           
              RTC.getTime();
              hor=RTC.day;
              RTC.setRAM(value*7,&hor,1);
              hor=RTC.month;
              RTC.setRAM(value*7+1,&hor,1);
              hor=RTC.year-2000;
              RTC.setRAM(value*7+2,&hor,1);
              hor=RTC.hour;
              RTC.setRAM(value*7+3,&hor,1);
              hor=RTC.minute;
              RTC.setRAM(value*7+4,&hor,1);
              hor=RTC.second;
              RTC.setRAM(value*7+5,&hor,1);
              hor=RTC.dow;
              RTC.setRAM(value*7+6,&hor,1);
              pc=pc+2;
              break;
     #endif
     #if defined(Activate_Real_Time_Clock)     
     case 29:                                                                          
                                       
                             
                                                           
                                                               
                                                                     
                                                                   
                                                             
                                                                 
                           
                                
              n4=lire_eeprom(slot,pc+1);           
              n2=lire_eeprom(slot,pc+2);                
              switch (n2) {
                          case 'J' : 
                             n5=0;
                             break;
                          case 'M' :
                             n5=1;
                             break;
                          case 'A' :
                             n5=2; 
                             break;
                          case 'h' :
                             n5=3;
                             break;
                          case 'm' :
                             n5=4;
                             break;
                          case 's' :
                             n5=5;
                             break;
                          case 'o' :
                             n5=6;
                             break;   
              }             
              RTC.getRAM(n4*7+n5,&hor,1);
              variable[lire_eeprom(slot,pc+3)-65]=hor;
              pc=pc+4;
              break;
     #endif
     #if defined(Activate_Real_Time_Clock)     
     case 30:                                                                                                               
                                       
                             
                                                                      
                                                                   
                                                
                                                                   
                                                                       
                                                                 
                           
                                
              n2=lire_eeprom(slot,pc+1);                  
              n3=lire_eeprom(slot,pc+2);                 
              n4=lire_eeprom(slot,pc+3);                               
              duree(n2,n3,n4);
              pc=pc+4;
              break;         
     #endif
     #if defined(Activate_Lcd)     
     case 31 :                                                                                                                          
                                       
                             
                                                                 
                                                                   
                                               
                                                                   
                                            
                                                                   
                                                          
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              affiche_celsius(value);
              pc=pc+4;
              break;
      #endif
      #if defined(Activate_Lcd)      
      case 32 :                                                                                                                   
                                       
                             
                                                                 
                                                                   
                                               
                                                                   
                                            
                                                                   
                                                          
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = lire_eeprom(slot,pc+1);                            
              affiche_celsius(value);
              pc=pc+4;
              break;
     #endif
     #if defined(Activate_Lcd)
     case 33 :                                                                                                                         
                                       
                             
                                                                
                                                                   
                                               
                                                                   
                                            
                                                                   
                                                          
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = variable[lire_eeprom(slot,pc+1)-65];                                              
              affiche_lux(value);
              pc=pc+4;
              break;
      #endif
      #if defined(Activate_Lcd)      
      case 34 :                                                                                                                
                                       
                             
                                                                
                                                                   
                                               
                                                                   
                                            
                                                                   
                                                          
                         
                              
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              lcd.print("         ");
              lcd.setCursor(lire_eeprom(slot,pc+2),lire_eeprom(slot,pc+3));
              value = lire_eeprom(slot,pc+1);                            
              affiche_lux(value);
              pc=pc+4;
              break;         
      #endif
      #if defined(Activate_Buzzer)      
      case 35 :                                                                          
                                                                             
                                                                            
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Produire un son de fr\u00e9quence ");
                        Serial.print(lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),DEC);
                        Serial.print(" et de dur\u00e9e ");
                        Serial.print(lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5),DEC);
                        Serial.print(" sur la broche ");
                        Serial.print(lire_eeprom(slot,pc+1),DEC);
                        Serial.println("."); 
                      }
              #endif 
                                                                                                                                
              buzz(lire_eeprom(slot,pc+1),lire_eeprom(slot,pc+2)*256+lire_eeprom(slot,pc+3),lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5));
              pc=pc+6;
              break;         
     #endif         
     case 36 :                                                                                                                                               
              n1=lire_eeprom(slot,pc+1);
              n2=lire_eeprom(slot,pc+2);
              n3=lire_eeprom(slot,pc+3);
              #if defined(__mega_ver)
              if (debug) { 
                Serial.print("lire distance ultrason (");
                Serial.print(n1);
                Serial.print(";");
                Serial.print(n2);
                Serial.print(" et stocke le resulat dans la variable ");
                Serial.write(n3);
                Serial.println("");
              }
              #endif 
                                    
              digitalWrite(n1,HIGH);
              delayMicroseconds(1000);
              digitalWrite(n1,LOW);
                                  
              value=255;
              value=(pulseIn(n2,HIGH)/2)/29.1+2;                                                           
              #if defined(__mega_ver)
              if (debug) { 
                     Serial.print("Distance mesuree =");
                     Serial.print(value,DEC);
                     Serial.println("");
              }
              #endif
              if (value>255) { value=255; }                           
              variable[n3-65]=value;
              pc=pc+4;
              break;              
     case 37 :                                                   
                                      
                            
                                                     
                                                                   
                                              
                                                                                                 
                         
                              
              n1 = variable[lire_eeprom(slot,pc+1)-65];                                       
              n2 = variable[lire_eeprom(slot,pc+2)-65];                                       
              n3 = lire_eeprom(slot,pc+3);                             
                                                    
                                                    
                                                    
                                                     
                                                     
                                                     
              value=0;                             
              switch (n3) {
                case 61: 
                         
                 if (n1==n2) {value=1;} else {value=0;}
                 break;
                case 62: 
                         
                 if (n1>n2) {value=1;} else {value=0;}
                 break;
                case 60: 
                         
                 if (n1<n2) {value=1;} else {value=0;}
                 break;
                case 33: 
                          
                 if (n1!=n2) {value=1;} else {value=0;}
                 break;
                case 115: 
                          
                 if (n1>=n2) {value=1;} else {value=0;}
                 break;
                case 105: 
                          
                 if (n1<=n2) {value=1;} else {value=0;}
                 break; 
              }                
              if (value==1)
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+4)*256+lire_eeprom(slot,pc+5);
               pc=pc+no+5;
              } else
              {
                                        
                             
                                                                                           
                  
                       
               
                                                                             
              pc=pc+6; 
              }
              break;

     case 38 :                                                                                                          
                                      
                            
                                                     
                                                                   
                                              
                                                                                                 
                         
                              
              n1 = variable[lire_eeprom(slot,pc+2)-65];                                       
              n2 = variable[lire_eeprom(slot,pc+4)-65];                                       
              n3 = lire_eeprom(slot,pc+3);                                              
                                                           
                                                               
                                                                 
                                                           
              value=0;                             
              switch (n3) {
                case 43: 
                              
                 value=n1+n2;
                 break;
                case 45: 
                              
                 value=n1-n2;
                 break;
                case 42: 
                              
                 value=n1*n2;
                 break;
                case 47: 
                              
                 value=n1/n2;
                 break;
              }           
                                                              
              variable[lire_eeprom(slot,pc+1)-65]=value;
              pc=pc+5; 
              break;
     case 40 :                                                                          
                                      
                            
                         
                              
              n1 = lire_eeprom(slot,pc+1);                                    
              n2 = lire_eeprom(slot,pc+2);                                     
              Wire.beginTransmission(n1);                                                  
              for (byte i=1;i<=n2;i++){
                                          
                 n3 = lire_eeprom(slot,pc+2+i);                               
                 Wire.write(n3);
                 }
              Wire.endTransmission();                                                     
              pc=pc+3+n2; 
              break;
     case 41 :                                                                                                             
                                      
                            
                         
                              
              n1 = lire_eeprom(slot,pc+1);                                    
              Wire.requestFrom(n1,1);                                                         
              delay(1);
              value=Wire.read();                                        
              delay(1);
              Wire.endTransmission();                                                     
                                                                          
              if (value==lire_eeprom(slot,pc+2))
              {
                                                                        
                                        
                             
                                                                    
                                                                                      
                                             
                  
                       
               no=lire_eeprom(slot,pc+3)*256+lire_eeprom(slot,pc+4);
               pc=pc+no+4;
              } else
              {
                                                                             
                                        
                             
                                                                                           
                  
                       
              pc=pc+5; 
              }
              break;              
     case 42 :                                                                                                       
                                      
                            
                         
                              
              n1 = lire_eeprom(slot,pc+1);                                    
              n2 = lire_eeprom(slot,pc+2);                                
              Wire.beginTransmission(n1);                                                  
              Wire.write(n2);                               
              Wire.write(variable[lire_eeprom(slot,pc+3)-65]);
              Wire.endTransmission();                                                     
              pc=pc+4; 
              break;
       
     case 255 :                                          
              #if defined(__mega_ver)
              if (debug) { 
                        Serial.print("Arret du programme dans le slot ");
                        Serial.println(slot,DEC);
                        }
              #endif          
              Serial.print("PROGRAM ENDING SLOT");
              Serial.println(slot,DEC);
              break;    
          }         
     } else
     {
                                                                     
       #if defined(__mega_ver)
       if (debug) { Serial.println("On est dans la temporisation"); }
       #endif
      if (intempo[slot]<millis()) { 
       intempo[slot]=0;
       #if defined(__mega_ver)
       if (debug) { 
         Serial.println("On sort de la temporisation");
          }
       #endif   
      }                                 
     }                            
                                                              
          pcs[slot]=pc;
          for (int i=0;i<26;i++) {
          variables[slot*26+i]=variable[i];
           }
       }                                     
   }                           
                                             
                      
}          
 #if defined(__mega_ver)
  if (debug) {Serial.println("Sortie interpr\u00e9teur");}
#endif  
}
#endif

void fixbaudrate() {
   switch (EEPROM.read(baud_rate)) {
     case 0 :           
              Serial.begin(300);
              break;
     case 1 :            
              Serial.begin(9600);
              break;
     case 2 :            
              Serial.begin(1200);
              break;
     case 3 :            
              Serial.begin(2400);
              break;
     case 4 :            
              Serial.begin(4800);
              break;
     case 5 :            
              Serial.begin(9600);
              break;
     case 6 :             
              Serial.begin(14400);
              break;
     case 7 :             
              Serial.begin(19200);
              break;
     case 8 :             
              Serial.begin(28800);
              break;
     case 9 :             
              Serial.begin(38400);
              break;
     case 10 :             
              Serial.begin(57600);
              break;
     case 11 :              
              Serial.begin(115200);
              break;
     default :            
              Serial.begin(9600);
    }         
                         
}  

                                                       
void setup()   { 
                                              
                  
  Serial.begin(9600);
  Wire.begin();
                                              
  identification();
                                                                        
  for (int i=first_pin_eeprom;i<=last_pin_eeprom;i++) {
   switch (EEPROM.read(i)) {
                                 
                       
     #if defined(Activate_Numeric_Output)
     case 1 :                             
              pinMode(i,OUTPUT);
              digitalWrite(i,LOW);
                                                       
              break;
     #endif
     #if defined(Activate_Numeric_Input)    
     case 2 :                             
              pinMode(i,INPUT);
                                                      
              break;
     #endif
     #if defined(Activate_Analogic_Input)
     case 3 :                              
                                                       
              break;
     #endif
     #if defined(Activate_Pwm)     
     case 4 :                       
              pinMode(i, OUTPUT);
              analogWrite(i,0);                                        
                                                   
              break;
     #endif
     #if defined(Activate_Servo_Motor)     
     case 5 :                                
              #if defined(__mega_ver) 
              switch (i) {
                case 14 : myservo14.attach(14);
                         break;
                case 15 : myservo15.attach(15);
                         break;
                case 16 : myservo16.attach(16);
                         break;
                case 17 : myservo17.attach(17);
                         break;
                case 18 : myservo18.attach(18);
                         break;
                case 19 : myservo19.attach(19);
                         break;
                case 22 : myservo22.attach(22);
                         break;
                case 23 : myservo23.attach(23);
                         break;
                case 24 : myservo24.attach(24);
                         break;
                case 25 : myservo25.attach(25);
                         break;
                case 26 : myservo26.attach(26);
                         break;  
                case 27 : myservo27.attach(27);
                         break;           
              }             
              #else
              switch (i) {
                case 4 : myservo4.attach(4);
                         break;
                case 5 : myservo5.attach(5);
                         break;
                case 6 : myservo6.attach(6);
                         break;
                case 7 : myservo7.attach(7);
                         break;
                case 8 : myservo8.attach(8);
                         break;
                case 9 : myservo9.attach(9);
                         break;
                case 10 : myservo10.attach(10);
                         break;
                case 11 : myservo11.attach(11);
                         break;
                case 12 : myservo12.attach(12);
                         break;
                case 13 : myservo13.attach(13);
                         break;
                }             
              #endif
                                                           
              break;  
     #endif
     #if defined(Activate_Ir_Remote)         
     case 6 :                                   
              pinMode(i, INPUT);
              IRrecv irrecv(i);
              irrecv.enableIRIn();                                        
                                                 
              break;
     #endif         
   }                     
  }     
   
                                  
  time_out=false;
  #if defined(Activate_Lcd)
  if (EEPROM.read(lcd_connected)==1) {
      lcd.init();                                            
      lcd.backlight();
      lcd.print("  Powered by");
      lcd.setCursor(0,1);
      lcd.print("TECHNOZONE51(c)");
  }
  #endif
  #if defined(Activate_Compiler)
  if (EEPROM.read(autorun)==1) {interpreteur();}                                                    
  #endif
}

                                              
                                   
void loop()                     
{
        if (time_out) {
                                          
          Serial.flush();
          Serial.println("TIME OUT");
          time_out=false;
        }
                                        
        if (Serial.available() > 0) {
		                          
		incomingByte = Serial.read();
                
                if (incomingByte==90) {
                                                                                           
                  Serial.flush();
                  setup();
                }

                #if defined(Activate_Analogic_Input)
                if (incomingByte==65) {
                                                                                              
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                                                          
                   no=digit_d*10+digit_u;
                   value = analogRead(no)/4;                                                                
                   Serial.print("VALUE=");
                   Serial.println(value, DEC);                                                                      
                   }
                 }
                #endif
                #if defined(Activate_Analogic_Input)
                if (incomingByte==97) {
                                                                                               
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                                                          
                   no=digit_d*10+digit_u;
                   value = analogRead(no);                                                                
                   Serial.print("VALUE=");
                   Serial.println(value, DEC);                                                                      
                   }
                 }
                #endif
                
                if (incomingByte==81) {
                                                                                         
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                                       
                  n2=lire_octet()-48;
                                       
                  n3=lire_octet()-48;
                  if (!time_out) {
                                                                                         
                   no=digit_d*10+digit_u;
                   digitalWrite(no,HIGH);
                   delayMicroseconds(1000);
                   digitalWrite(no,LOW);
                                                              
                   no=n2*10+n3;
                   value = (pulseIn(no,HIGH)/2)/29.1+2;                                                           
                   if (value>255) { value=255; }
                   Serial.print("VALUE=");
                   Serial.println(value, DEC);                                                                      
                   }
                 }


                if (incomingByte==66) {
                                                                                                       
                                                         
                  digit_u = lire_octet();
                  if (!time_out) {
                   if (slot_p==0) {
                     EEPROM.write(pc,digit_u);                                                          
                     delay(5);
                     }
                     else {
                     i2c_eeprom_write_byte(MEM_ADDR+get_bank(slot_p), pc ,digit_u);                                                    
                     delay(5);
                     }
                   pc=pc+1;                   
                   Serial.print("DONE B");
                   Serial.println(digit_u, DEC);                             
                   }
                 }
                #if defined(__mega_ver)
                if (incomingByte==68) {
                                                                                        
                                                            
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                   debug = digit_u;  
                  }
                   Serial.print("DONE D");
                   Serial.println(digit_u, DEC);                             
                  }
                #endif
                
                #if defined(Activate_Lcd)
                if (incomingByte==76) {
                                                                             
                                              
                  n1 = lire_octet();
                  if (!time_out) {
                                                    
                    if (n1==67) {
                                                        
                      lcd.clear();
                    }
                    if (n1==83) {
                                                                                                    
                                              
                                          
                                               
                                            
                                                         
                                           
                      n2=lire_octet()-48;
                                           
                      n3=lire_octet()-48;
                                           
                      n4=lire_octet()-48;
                                           
                      n5=lire_octet()-48;
                                           
                      n6=lire_octet()-48;
                                           
                      n7=lire_octet()-48;
                                           
                      n8=lire_octet()-48;
                      if (n2==1 && !time_out)
                      {
                                                  
                        EEPROM.write(lcd_connected,1);
                        EEPROM.write(lcd_col,n3*10+n4);
                        EEPROM.write(lcd_row,n5);
                        EEPROM.write(lcd_i2c_add,n6*100+n7*10+n8);
                      } else
                      {
                                                     
                        EEPROM.write(lcd_connected,0);
                                                                
                      }  
                    }
                    if (n1==71) {
                                                                                         
                                                
                      n2 = lire_octet()-48;
                                                
                      n3 = lire_octet()-48;
                                                
                      n4 = lire_octet()-48;
                                                       
                      value=n2*10+n3;
                                                                  
                      if (!time_out) {lcd.setCursor(value, n4);}
                    }
                    if (n1=='D') {
                                                                                                                                            
                                                
                      n2 = lire_octet()-48;
                      if (!time_out) 
                       {
                        RTC.getRAM(n2*7,&hor,1);             
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write('/');
                        RTC.getRAM(n2*7+1,&hor,1);            
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write('/');
                        RTC.getRAM(n2*7+2,&hor,1);             
                        lcd.write((hor/10)+48);         
                        lcd.write((hor%10)+48);
                       }
                    }                      
                    if (n1=='H') {
                                                                                                                                            
                                                
                      n2 = lire_octet()-48;
                      if (!time_out) 
                       {
                        RTC.getRAM(n2*7+3,&hor,1);                 
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write(':');
                        RTC.getRAM(n2*7+4,&hor,1);                     
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                        lcd.write(':');
                        RTC.getRAM(n2*7+5,&hor,1);                     
                        lcd.write((hor/10)+48);
                        lcd.write((hor%10)+48);
                       }
                    }                      
                    if (n1==87) {
                                                                                                                 
                      do
                       { 
                        n2 = lire_octet();
                        if (!time_out && n2!=124) {lcd.write(n2);}
                       } while (n2!=124&&!time_out);
                    }  
                    if (n1==86) {
                                                                                                      
                                                            
                      digit_c=lire_octet()-48;
                                                           
                      digit_d=lire_octet()-48;
                                                         
                      digit_u=lire_octet()-48;
                                                     
                      n3=lire_octet()-48;
                                                   
                      n4=lire_octet()-48;
                                       
                      n5=lire_octet()-48;
                      value=digit_c*100+digit_d*10+digit_u;
                      if (!time_out) 
                      {
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print("   ");
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print(value);
                      }
                    }
                   if (n1==84) {
                                                                                                                                    
                                                                 
                                                            
                      digit_c=lire_octet()-48;
                                                           
                      digit_d=lire_octet()-48;
                                                         
                      digit_u=lire_octet()-48;
                                                     
                      n3=lire_octet()-48;
                                                   
                      n4=lire_octet()-48;
                                       
                      n5=lire_octet()-48;
                      value=digit_c*100+digit_d*10+digit_u;
                      if (!time_out) 
                      {
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print("         ");
                       lcd.setCursor(n3*10+n4,n5);
                                                            
                       affiche_celsius(value);
                                          
                      }
                    }
                   
                   if (n1==76) {
                                                                                                                                          
                                                                   
                                                            
                      digit_c=lire_octet()-48;
                                                           
                      digit_d=lire_octet()-48;
                                                         
                      digit_u=lire_octet()-48;
                                                     
                      n3=lire_octet()-48;
                                                   
                      n4=lire_octet()-48;
                                       
                      n5=lire_octet()-48;
                      value=digit_c*100+digit_d*10+digit_u;
                      if (!time_out) 
                      {
                       lcd.setCursor(n3*10+n4,n5);
                       lcd.print("         ");
                       lcd.setCursor(n3*10+n4,n5);
                                                             
                       affiche_lux(value);
                                          
                      }
                    } 
                  }
                  
                  if (!time_out) {
                  Serial.print("DONE L");
                  Serial.write(n1);
                  Serial.println("");
                  }
                 }
                #endif
                #if defined(Activate_Pwm)
                if (incomingByte==77) {
                                                                                 
                                            
                  n1 = lire_octet()-48;
                                            
                  n2 = lire_octet()-48;
                                            
                  n3 = lire_octet()-48;
                                            
                  n4 = lire_octet()-48;
                                            
                  n5 = lire_octet()-48;
                  if (!time_out) {
                                                   
                  no=n1*10+n2;
                                            
                  value=n3*100+n4*10+n5;
                                           
                  analogWrite(no,value);                                         
                  Serial.print("DONE M");
                  Serial.write(n1+48);
                  Serial.write(n2+48);
                  Serial.write(n3+48);
                  Serial.write(n4+48);
                  Serial.write(n5+48);
                  Serial.println("");
                  }
                 }
                #endif
                
                if (incomingByte==78) {
                                                                                             
                                            
                  n1 = lire_octet();
                                            
                  n2 = lire_octet();
                                            
                  n3 = lire_octet();
                                            
                  n4 = lire_octet();
                                            
                  n5 = lire_octet();
                                            
                  n6 = lire_octet();
                  if (!time_out) {
                                                               
                  EEPROM.write(first_name_eeprom,n1);
                  EEPROM.write(first_name_eeprom+1,n2);
                  EEPROM.write(first_name_eeprom+2,n3);
                  EEPROM.write(first_name_eeprom+3,n4);
                  EEPROM.write(first_name_eeprom+4,n5);
                  EEPROM.write(first_name_eeprom+5,n6);
                  Serial.print("DONE N");
                  Serial.write(n1);
                  Serial.write(n2);
                  Serial.write(n3);
                  Serial.write(n4);
                  Serial.write(n5);
                  Serial.write(n6);
                  Serial.println("");
                  }
                 }

                if (incomingByte==80) {
                                                                                                       
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                                             
                   value = lire_octet()-48;
                                                 
                   if (!time_out) {
                                                                                             
                     no=digit_d*10+digit_u;
                     if (value==0) {
                      digitalWrite(no, LOW);                         
                      } 
                      else
                      {
                      digitalWrite(no, HIGH);                         
                      }                 
                   Serial.print("DONE P");
                   Serial.print(digit_d,DEC);
                   Serial.print(digit_u,DEC);
                   Serial.println(value,DEC);
                   }
                 }

                #if defined(Activate_Numeric_Input)
                if (incomingByte==82) {
                                                                               
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                                                         
                  no=digit_d*10+digit_u;
                  if (!time_out) {
                                                           
                    no=digit_d*10+digit_u;
                    value = digitalRead(no);                                                               
                    Serial.print("VALUE=");
                    Serial.println(value, DEC);                                                                      
                    }
                 }
                 #endif
                 #if defined(Activate_Ir_Remote) 
                 if (incomingByte==73) {
                                                                              
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                    if (irrecv.decode(&results)) {
                         String s=String(long(results.value), HEX);
                         s.toUpperCase();
                         Serial.print("VALUE=");
                         Serial.println(s);                                                                      
                                      
                                                                    
                     } else {Serial.println("VALUE=-1");}
                   }
                 }
                 #endif
                 #if defined(Activate_Ir_Remote) 
                 if (incomingByte==74) {
                                                                                      
                  irrecv.resume();                          
                  Serial.println("DONE J");                
                 }
                 #endif
                if (incomingByte==84) {
                                                                          
                                            
                  value = lire_octet()-48;
                                                
                  if (value==0) {
                                             
                     EEPROM.write(autorun,0); 
                    } 
                    else
                    {
                                           
                      EEPROM.write(autorun,1); 
                    }                 
                   Serial.print("DONE T");
                   Serial.println(value,DEC);
                 }
                #if defined(Activate_Numeric_Output)
                if (incomingByte==87) {
                                                                                     
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                                            
                  value = lire_octet()-48;
                                                
                  if (!time_out) {
                                                                                            
                    no=digit_d*10+digit_u;
                    if (value==0) {
                      digitalWrite(no, LOW);                         
                    } 
                    else
                    {
                      digitalWrite(no, HIGH);                         
                    }                 
                   Serial.print("DONE W");
                   Serial.print(digit_d,DEC);
                   Serial.print(digit_u,DEC);
                   Serial.println(value,DEC);
                   }
                 }
                #endif
                if (incomingByte==69) {
                                                                      
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                                            
                  value = lire_octet()-48;
                  if (!time_out) {
                                                 
                                                   
                   no=digit_d*10+digit_u; 
                   EEPROM.write(no,value);                                   
                   Serial.print("DONE E");
                   Serial.print(digit_d,DEC);
                   Serial.print(digit_u,DEC);
                   Serial.println(value,DEC);
                   }
                 }  
                #if defined(Activate_Compiler)
                if (incomingByte==70) {
                                                                                                                             
                                                                                                                                                          
                  digit_d=lire_octet()-48;                                 
                  digit_u = lire_octet()-48;                             
                  slot_p=digit_d*10+digit_u;                              
                  if (!time_out) {
                     pc=get_first_pc(slot_p);                                                                                      
                                                                                                            
                     Serial.print("DONE F");
                     Serial.print(digit_d,DEC);
                     Serial.println(digit_u,DEC);
                    }
                 }  
                #endif
                if (incomingByte==71) {
                                                                                                                            
                                                                                                                                                          
                  digit_d=lire_octet()-48;                                 
                  digit_u = lire_octet()-48;                             
                  value=lire_octet()-48;                                                                            
                  slot_p=digit_d*10+digit_u;                                  
                  if (!time_out) {
                     EEPROM.write(run+slot_p,value);                                                     
                     Serial.print("DONE G");
                     Serial.print(digit_d,DEC);
                     Serial.print(digit_u,DEC);
                     Serial.println(value,DEC);
                   }  
                 }  
                
                if (incomingByte==98) {
                                                                                   
                                                                                      
                                 
                                  
                                  
                                  
                                  
                                  
                                   
                                   
                                   
                                   
                                    
                                     
                  digit_d=lire_octet()-48;                                 
                  digit_u = lire_octet()-48;                             
                  value=digit_d*10+digit_u;                                 
                  if (value>11) {value=7;}
                  if (!time_out) {
                     EEPROM.write(baud_rate,value);                                                    
                     Serial.print("DONE b");
                     Serial.print(digit_d,DEC);
                     Serial.println(digit_u,DEC);
                     fixbaudrate();                                               
                   }  
                 }
                 
                if (incomingByte==72) {
                                                                                      
                                        
                  digit_d=lire_octet()-48;                                 
                  digit_u = lire_octet()-48;                             
                  n1=lire_octet();                                                
                  n2=lire_octet();                                                
                  n3=lire_octet();                                                
                  n4=lire_octet();                                                
                  n5=lire_octet();                                                
                  n6=lire_octet();                                                
                  n7=lire_octet();                                                
                  n8=lire_octet();                                                
                  slot_p=digit_d*10+digit_u;                                  
                  if (!time_out) {
                     EEPROM.write(name_slot+slot_p*8,n1);                                            
                     EEPROM.write(name_slot+slot_p*8+1,n2);                                            
                     EEPROM.write(name_slot+slot_p*8+2,n3);                                            
                     EEPROM.write(name_slot+slot_p*8+3,n4);                                            
                     EEPROM.write(name_slot+slot_p*8+4,n5);                                            
                     EEPROM.write(name_slot+slot_p*8+5,n6);                                            
                     EEPROM.write(name_slot+slot_p*8+6,n7);                                            
                     EEPROM.write(name_slot+slot_p*8+7,n8);                                            
                     
                     Serial.print("DONE H");
                     Serial.print(digit_d,DEC);
                     Serial.print(digit_u,DEC);
                     Serial.write(n1);
                     Serial.write(n2);
                     Serial.write(n3);
                     Serial.write(n4);
                     Serial.write(n5);
                     Serial.write(n6);
                     Serial.write(n7);
                     Serial.write(n8);
                     Serial.println("");
                   }  
                 }
                #if defined(Activate_Buzzer)
                if (incomingByte==79) {
                                                                                        
                                        
                  digit_d=lire_octet()-48;                                 
                  digit_u = lire_octet()-48;                             
                  n1=lire_octet()-48;                                 
                  n2=lire_octet()-48;                                  
                  n3=lire_octet()-48;                                 
                  n4=lire_octet()-48;                                
                  n5=lire_octet()-48;                             
                  n6=lire_octet()-48;                              
                  n7=lire_octet()-48;                             
                  n8=lire_octet()-48;                           
                  slot_p=digit_d*10+digit_u;                                  
                  long freq=1000*n1+100*n2+10*n3+n4;
                  long dur=1000*n5+100*n6+10*n7+n8;
                  if (!time_out) {
                     buzz(slot_p,freq,dur);
                     Serial.print("DONE O");
                     Serial.print(digit_d,DEC);
                     Serial.print(digit_u,DEC);
                     Serial.print(n1,DEC);
                     Serial.print(n2,DEC);
                     Serial.print(n3,DEC);
                     Serial.print(n4,DEC);
                     Serial.print(n5,DEC);
                     Serial.print(n6,DEC);
                     Serial.print(n7,DEC);
                     Serial.print(n8,DEC);
                     Serial.println("");
                   }  
                 }
                 #endif
                if (incomingByte==75) {
                                                                                    
                                
                                             
                  digit_d=lire_octet()-48;                                 
                  digit_u = lire_octet()-48;                             
                  slot_p=digit_d*10+digit_u;                                  
                  if (!time_out) {
                     n1=EEPROM.read(name_slot+slot_p*8);                                           
                     n2=EEPROM.read(name_slot+slot_p*8+1);                                           
                     n3=EEPROM.read(name_slot+slot_p*8+2);                                           
                     n4=EEPROM.read(name_slot+slot_p*8+3);                                           
                     n5=EEPROM.read(name_slot+slot_p*8+4);                                           
                     n6=EEPROM.read(name_slot+slot_p*8+5);                                           
                     n7=EEPROM.read(name_slot+slot_p*8+6);                                           
                     n8=EEPROM.read(name_slot+slot_p*8+7);                                           
                     value=EEPROM.read(run+slot_p);                                              
                     Serial.print("VALUE=");
                     Serial.write(n1);
                     Serial.write(n2);
                     Serial.write(n3);
                     Serial.write(n4);
                     Serial.write(n5);
                     Serial.write(n6);
                     Serial.write(n7);
                     Serial.write(n8);
                     Serial.print(":");
                     Serial.println(value,DEC);
                   }  
                 }
                 #if defined(Activate_Servo_Motor)
                if (incomingByte==83) {
                                                                              
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                                                                                          
                  no=digit_d*10+digit_u;
                                            
                  digit_c = lire_octet()-48;
                                            
                  digit_d = lire_octet()-48;
                                            
                  digit_u = lire_octet()-48;
                  if (!time_out) {
                                                       
                  value=digit_c*100+digit_d*10+digit_u;
                  #if defined(__mega_ver)
                   switch(no) {
                    case 14 :
                            myservo14.write(value);
                            break;
                    case 15 :
                            myservo15.write(value);
                            break;
                    case 16 :
                            myservo16.write(value);
                            break;
                    case 17 :
                            myservo17.write(value);
                            break;
                    case 18 :
                            myservo18.write(value);
                            break;
                    case 19 :
                            myservo19.write(value);
                            break;
                    case 22 :
                            myservo22.write(value);
                            break;
                    case 23 :
                            myservo23.write(value);
                            break;
                    case 24 :
                            myservo24.write(value);
                            break;
                    case 25 :
                            myservo25.write(value);
                            break;
                    case 26 :
                            myservo26.write(value);
                            break;
                    case 27 :
                            myservo27.write(value);
                            break;        
                   }  
                   #else
                   switch(no) {
                    case 4 :
                            myservo4.write(value);
                            break;
                    case 5 :
                            myservo5.write(value);
                            break;
                    case 6 :
                            myservo6.write(value);
                            break;
                    case 7 :
                            myservo7.write(value);
                            break;
                    case 8 :
                            myservo8.write(value);
                            break;
                    case 9 :
                            myservo9.write(value);
                            break;
                    case 10 :
                            myservo10.write(value);
                            break;
                    case 11 :
                            myservo11.write(value);
                            break;
                    case 12 :
                            myservo12.write(value);
                            break;
                    case 13 :
                            myservo13.write(value);
                            break;
                    } 
                   #endif
                                                    
                   Serial.println("DONE S");
                                          
                                       
                                               
                  } 
                }
                #endif                
                if (incomingByte==86) {
                                                                              
                  identification();
                }
                #if defined(Activate_Compiler)
                if (incomingByte==88) {
                                                                                                  
                  Serial.println("PROGRAM RUNNING");
                  interpreteur();
                }
                #endif
                
                #if defined(Activate_Generic_I2c)
                if (incomingByte==105) {
                                                                      
                                               
                  n1 = lire_octet();
                  if (!time_out) {
                                                      
                    if (n1=='R') {
                                                                                                                    
                      n2 = lire_octet()-48;             
                      n3 = lire_octet()-48;             
                      n4 = lire_octet()-48;             
                      n5 = lire_octet()-48;             
                      n6 = lire_octet()-48;             
                      n7 = n2*100+n3*10+n4;                                       
                      n8 = n5*10+n6;                                          
                      if (!time_out) {
                                               
                       Wire.requestFrom(n7,n8);
                       Serial.print("VALUE=");
                       for (byte i=1;i<n8;i++) {
                         delay(1);
                         n9=Wire.read();
                         Serial.print(n9,DEC);
                         Serial.print(",");
                       }
                       delay(1);
                       n9=Wire.read();
                       Wire.endTransmission();
                       Serial.println(n9,DEC);
                       delay(1);
                      } 
                    }                      
                                                   
                    if (n1=='W') {
                                                                                                                                
                      n2 = lire_octet()-48;             
                      n3 = lire_octet()-48;             
                      n4 = lire_octet()-48;             
                      n5 = lire_octet()-48;             
                      n6 = lire_octet()-48;             
                      n7 = n2*100+n3*10+n4;                                       
                      n8 = n5*10+n6;                                          
                      if (!time_out) {
                                               
                       Wire.beginTransmission(n7);                                                  
                       for (byte i=1;i<=n8;i++){
                                                  
                         n2 = lire_octet()-48;             
                         n3 = lire_octet()-48;             
                         n4 = lire_octet()-48;             
                         n7 = n2*100+n3*10+n4;                                    
                         delay(1);
                         Wire.write(n7);
                       }
                       delay(1);
                       Wire.endTransmission();                                                     
                       Serial.println("DONE iW");
                      } 
                    }                         
                                                   
                    if (n1=='w') {
                                                                                                                                   
                                                      
                      n2 = lire_octet()-48;             
                      n3 = lire_octet()-48;             
                      n4 = lire_octet()-48;             
                      n5 = lire_octet()-48;             
                      n6 = lire_octet()-48;             
                      n7 = n2*100+n3*10+n4;                                       
                      n8 = n5*10+n6;                                          
                      if (!time_out) {
                                               
                       Wire.beginTransmission(n7);                                                  
                       for (byte i=1;i<=n8;i++){
                                                  
                         n2 = lire_octet()-48;             
                         n3 = lire_octet()-48;             
                         n4 = lire_octet()-48;             
                         n7 = n2*100+n3*10+n4;                                    
                         Wire.write(n7);
                       }
                       Wire.endTransmission(false);                                                                    
                       Serial.println("DONE iw");
                      } 
                    }                         
                    
                  }            
                }                     
                #endif

               #if defined(Activate_Real_Time_Clock)
                if (incomingByte==89) {
                                                                          
                                               
                  n1 = lire_octet();
                  if (!time_out) {
                                                    
                    if (n1=='C') {
                                                                                                                    
                      n2 = lire_octet()-48;             
                      n3 = lire_octet()-48;             
                      n4 = lire_octet()-48;             
                      if (!time_out) {
                       duree(n2,n3,n4); 
                       Serial.println("DONE YC");
                      } 
                    }
                    if (n1==83) {
                                                                                          
                      n2 = lire_octet()-48;            
                      n3 = lire_octet()-48;            
                      n4 = lire_octet()-48;            
                      n5 = lire_octet()-48;            
                      n6 = lire_octet()-48;            
                      n7 = lire_octet()-48;            
                      n8 = lire_octet()-48;            
                      n9 = lire_octet()-48;            
                      n10 = lire_octet()-48;            
                      n11 = lire_octet()-48;            
                      n12 = lire_octet()-48;            
                      n13 = lire_octet()-48;            
                      if (!time_out) {
                       RTC.stopClock();
                       RTC.fillByYMD(2000+n6*10+n7,n4*10+n5,n2*10+n3);
                       RTC.fillByHMS(n8*10+n9,n10*10+n11,n12*10+n13);
                       RTC.setTime();
                       RTC.startClock();
                       Serial.println("DONE YS");
                      } 
                    }
                    if (n1==86) {
                                                                                       
                      n2 = lire_octet()-48;            
                      n4 = lire_octet();               
                      if (!time_out) 
                      {
                                              
                        n5=6;                                                
                        switch (n4) {
                          case 'J' : 
                             n5=0;
                             break;
                          case 'M' :
                             n5=1;
                             break;
                          case 'A' :
                             n5=2; 
                             break;
                          case 'h' :
                             n5=3;
                             break;
                          case 'm' :
                             n5=4;
                             break;
                          case 's' :
                             n5=5;
                             break;
                          case 'o' :
                             n5=6;
                             break;   
                        }
                        RTC.getRAM(n2*7+n5,&hor,1);
                        Serial.print("VALUE=");
                        Serial.println(hor,DEC);
                      }
                    }
                    if (n1==82) {
                                                                                                                    
                      n2 = lire_octet()-48;            
                      if (!time_out) {
                      RTC.getTime();
                      hor=RTC.day;
                      RTC.setRAM(n2*7,&hor,1);
                      hor=RTC.month;
                      RTC.setRAM(n2*7+1,&hor,1);
                      hor=RTC.year-2000;
                      RTC.setRAM(n2*7+2,&hor,1);
                      hor=RTC.hour;
                      RTC.setRAM(n2*7+3,&hor,1);
                      hor=RTC.minute;
                      RTC.setRAM(n2*7+4,&hor,1);
                      hor=RTC.second;
                      RTC.setRAM(n2*7+5,&hor,1);
                      hor=RTC.dow;
                      RTC.setRAM(n2*7+6,&hor,1);
                      Serial.print("VALUE=");
                      if (RTC.day < 10)                                                
                      {
                        Serial.print("0");
                        Serial.print(RTC.day, DEC);
                      }else
                      {
                        Serial.print(RTC.day, DEC);
                      }
                      Serial.print("/");
                      if (RTC.month < 10)                                                
                      {
                        Serial.print("0");
                        Serial.print(RTC.month, DEC);
                      }
                      else
                      {
                        Serial.print(RTC.month, DEC);
                      }
                      Serial.print("/");
                      Serial.print(RTC.year-2000, DEC);                                        
                      Serial.print(" ");
                      if (RTC.hour < 10)                                                
                      {
                        Serial.print("0");
                        Serial.print(RTC.hour, DEC);
                      } 
                      else
                      {
                        Serial.print(RTC.hour, DEC);
                      }
                      Serial.print(":");
                      if (RTC.minute < 10)                                                
                      {
                        Serial.print("0");
                        Serial.print(RTC.minute, DEC);
                      }
                      else
                      {
                        Serial.print(RTC.minute, DEC);
                      }
                      Serial.print(":");
                      if (RTC.second < 10)                                                
                      {
                        Serial.print("0");
                        Serial.print(RTC.second, DEC);
                      }
                      else
                      {
                        Serial.print(RTC.second, DEC);
                      }
                      Serial.print(" ");
                      Serial.println(RTC.dow,DEC);
                      }         
                    }                
                }                         
                
            }  
            #endif            
	}
}

