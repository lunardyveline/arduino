#line 1 "FirmataPlus.ino"

  
                                                                        
                                                                
                                      
  
                                                                          
                                                        
  
                                   
   

  
                                                                      
                                                               
                                                            
                                                          
                                                           


                                                              
                                                           
                                                             
                                                                   

                                                                  

                                                   
   
 
                                                                      
                            
        
           
           
                                                                                  
                                                                         

  
                                                                
   

#include <Servo.h>
#include <Wire.h>
#include <FirmataPlus.h>
#include <NewPing.h>
#include <Stepper.h>
#include <ooPinChangeInt.h> https:                                                                                         
#include <AdaEncoder.h> https:                                                                            


                                           
#define I2C_WRITE B00000000
#define I2C_READ B00001000
#define I2C_READ_CONTINUOUSLY B00010000
#define I2C_STOP_READING B00011000
#define I2C_READ_WRITE_MODE_MASK B00011000
#define I2C_10BIT_ADDRESS_MODE_MASK B00100000

#define MAX_QUERIES 8
#define MINIMUM_SAMPLING_INTERVAL 10

#define REGISTER_NOT_SPECIFIED -1

#define ENCODER_NOT_PRESENT 0
#define ENCODER_IS_PRESENT  1

#define INTER_PING_INTERVAL 40          

                               
#define TONE_TONE 0
#define TONE_NO_TONE 1

#define STEPPER_CONFIGURE 0
#define STEPPER_STEP 1
#define STEPPER_LIBRARY_VERSION 2


                                                                                
                   
                                                                                

                   
#include "Arduino.h"
void readAndReportData(byte address, int theRegister, byte numBytes);
void outputPort(byte portNumber, byte portValue, byte forceSend);
void checkDigitalInputs(void);
void setPinModeCallback(byte pin, int mode);
void analogWriteCallback(byte pin, int value);
void digitalWriteCallback(byte port, int value);
void reportAnalogCallback(byte analogPin, int value);
void reportDigitalCallback(byte port, int value);
void sysexCallback(byte command, byte argc, byte *argv);
void enableI2CPins();
void disableI2CPins();
void systemResetCallback();
void setup();
void loop();
void printData(char * id,  long data);
#line 84
int analogInputsToReport = 0;                                        

                         
byte reportPINs[TOTAL_PORTS];                                           
byte previousPINs[TOTAL_PORTS];                            

                        
byte pinConfig[TOTAL_PINS];                                      
byte portConfigInputs[TOTAL_PORTS];                                                 
int pinState[TOTAL_PINS];                                             

                     
unsigned long currentMillis;                                                
unsigned long previousMillis;                                           
int samplingInterval = 19;                                                   

              
struct i2c_device_info {
  byte addr;
  byte reg;
  byte bytes;
};

                                  
i2c_device_info query[MAX_QUERIES];

byte i2cRxData[32];
boolean isI2CEnabled = false;
signed char queryIndex = -1;
unsigned int i2cReadDelayTime = 0;                                                                       


         
Servo servos[MAX_SERVOS];

                            
byte  encoderMSB, encoderLSB ;                            
uint8_t encoderPin1, encoderPin2 ;                               
int encoderPostion = 0;                                          
int8_t clicks = 0 ;                                        
boolean encoderPresent = false ;                            

                 

int numLoops = 0 ;
int pingLoopCounter = 0 ;

int numActiveSonars = 0 ;                             
uint8_t sonarPinNumbers[MAX_SONARS] ;
int nextSonar = 0 ;                                       

                                                   
NewPing *sonars[MAX_SONARS] ;

uint8_t sonarTriggerPin;
uint8_t sonarEchoPin ;
uint8_t currentSonar = 0;                                                     

uint8_t pingInterval = 33 ;                                                                      
                       
byte sonarMSB, sonarLSB ;


                

Stepper *stepper = NULL;


                                                                                
            
                                                                                

void readAndReportData(byte address, int theRegister, byte numBytes) {
                                                          
                                                                                   
                                                                                          
  if (theRegister != REGISTER_NOT_SPECIFIED) {
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write((byte)theRegister);
#else
    Wire.send((byte)theRegister);
#endif
    Wire.endTransmission();
                              
    if (i2cReadDelayTime > 0) {
                                                                
      delayMicroseconds(i2cReadDelayTime);
    }
  }
  else {
    theRegister = 0;                                         
  }

  Wire.requestFrom(address, numBytes);                                          

                                                                    
  if (numBytes == Wire.available()) {
    i2cRxData[0] = address;
    i2cRxData[1] = theRegister;
    for (int i = 0; i < numBytes; i++) {
#if ARDUINO >= 100
      i2cRxData[2 + i] = Wire.read();
#else
      i2cRxData[2 + i] = Wire.receive();
#endif
    }
  }
  else {
    if (numBytes > Wire.available()) {
      Firmata.sendString("I2C Read Error: Too many bytes received");
    }
    else {
      Firmata.sendString("I2C Read Error: Too few bytes received");
    }
  }

                                                    
  Firmata.sendSysex(SYSEX_I2C_REPLY, numBytes + 2, i2cRxData);
}

void outputPort(byte portNumber, byte portValue, byte forceSend)
{
                                                      
  portValue = portValue & portConfigInputs[portNumber];
                                                             
  if (forceSend || previousPINs[portNumber] != portValue) {
    Firmata.sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
}

                                                                                
                                                                               
                                                     
void checkDigitalInputs(void)
{
                                                                       
                                                                    
                                                 
  if (TOTAL_PORTS > 0 && reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (TOTAL_PORTS > 1 && reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (TOTAL_PORTS > 2 && reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
  if (TOTAL_PORTS > 3 && reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  if (TOTAL_PORTS > 4 && reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
  if (TOTAL_PORTS > 5 && reportPINs[5]) outputPort(5, readPort(5, portConfigInputs[5]), false);
  if (TOTAL_PORTS > 6 && reportPINs[6]) outputPort(6, readPort(6, portConfigInputs[6]), false);
  if (TOTAL_PORTS > 7 && reportPINs[7]) outputPort(7, readPort(7, portConfigInputs[7]), false);
  if (TOTAL_PORTS > 8 && reportPINs[8]) outputPort(8, readPort(8, portConfigInputs[8]), false);
  if (TOTAL_PORTS > 9 && reportPINs[9]) outputPort(9, readPort(9, portConfigInputs[9]), false);
  if (TOTAL_PORTS > 10 && reportPINs[10]) outputPort(10, readPort(10, portConfigInputs[10]), false);
  if (TOTAL_PORTS > 11 && reportPINs[11]) outputPort(11, readPort(11, portConfigInputs[11]), false);
  if (TOTAL_PORTS > 12 && reportPINs[12]) outputPort(12, readPort(12, portConfigInputs[12]), false);
  if (TOTAL_PORTS > 13 && reportPINs[13]) outputPort(13, readPort(13, portConfigInputs[13]), false);
  if (TOTAL_PORTS > 14 && reportPINs[14]) outputPort(14, readPort(14, portConfigInputs[14]), false);
  if (TOTAL_PORTS > 15 && reportPINs[15]) outputPort(15, readPort(15, portConfigInputs[15]), false);
}

                                                                                
                                                                           
                                                       
   
void setPinModeCallback(byte pin, int mode)
{
  if (pinConfig[pin] == I2C && isI2CEnabled && mode != I2C) {
                                                          
                                                                       
    disableI2CPins();
  }
  if (IS_PIN_SERVO(pin) && mode != SERVO && servos[PIN_TO_SERVO(pin)].attached()) {
    servos[PIN_TO_SERVO(pin)].detach();
  }
  if (IS_PIN_ANALOG(pin)) {
    reportAnalogCallback(PIN_TO_ANALOG(pin), mode == ANALOG ? 1 : 0);                         
  }
  if (IS_PIN_DIGITAL(pin)) {
    if (mode == INPUT) {
      portConfigInputs[pin / 8] |= (1 << (pin & 7));
    }
    else {
      portConfigInputs[pin / 8] &= ~(1 << (pin & 7));
    }
  }
  pinState[pin] = 0;
  switch (mode) {
    case ANALOG:
      if (IS_PIN_ANALOG(pin)) {
        if (IS_PIN_DIGITAL(pin)) {
          pinMode(PIN_TO_DIGITAL(pin), INPUT);                         
          digitalWrite(PIN_TO_DIGITAL(pin), LOW);                             
        }
        pinConfig[pin] = ANALOG;
      }
      break;
    case INPUT:
      if (IS_PIN_DIGITAL(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), INPUT);                         
        digitalWrite(PIN_TO_DIGITAL(pin), LOW);                             
        pinConfig[pin] = INPUT;
      }
      break;
    case OUTPUT:
      if (IS_PIN_DIGITAL(pin)) {
        digitalWrite(PIN_TO_DIGITAL(pin), LOW);               
        pinMode(PIN_TO_DIGITAL(pin), OUTPUT);
        pinConfig[pin] = OUTPUT;
      }
      break;
    case PWM:
      if (IS_PIN_PWM(pin)) {
        pinMode(PIN_TO_PWM(pin), OUTPUT);
        analogWrite(PIN_TO_PWM(pin), 0);
        pinConfig[pin] = PWM;
      }
      break;
    case SERVO:
      if (IS_PIN_SERVO(pin)) {
        pinConfig[pin] = SERVO;
        if (!servos[PIN_TO_SERVO(pin)].attached()) {
          servos[PIN_TO_SERVO(pin)].attach(PIN_TO_DIGITAL(pin));
        }
      }
      break;

    case ENCODER:
                                              
      pinMode(pin, INPUT);
      digitalWrite(pin, HIGH);

                                              
      pinConfig[pin] = ENCODER ;
      break ;

    case I2C:
      if (IS_PIN_I2C(pin)) {
                              
                                                                   
        pinConfig[pin] = I2C;
      }
      break;
    case TONE:
      pinConfig[pin] = TONE ;
      break ;
    case SONAR:
      pinConfig[pin] = SONAR ;
      break ;
    case STEPPER:
      pinConfig[pin] = STEPPER ;
      break ;
    default:
      Firmata.sendString("Unknown pin mode");                                  
      break ;
  }
                                                 
}

void analogWriteCallback(byte pin, int value)
{
  if (pin < TOTAL_PINS) {
    switch (pinConfig[pin]) {
      case SERVO:
        if (IS_PIN_SERVO(pin))
          servos[PIN_TO_SERVO(pin)].write(value);
        pinState[pin] = value;
        break;
      case PWM:
        if (IS_PIN_PWM(pin))
          analogWrite(PIN_TO_PWM(pin), value);
        pinState[pin] = value;
        break;
    }
  }
}

void digitalWriteCallback(byte port, int value)
{
  byte pin, lastPin, mask = 1, pinWriteMask = 0;

  if (port < TOTAL_PORTS) {
                                                                
    lastPin = port * 8 + 8;
    if (lastPin > TOTAL_PINS) lastPin = TOTAL_PINS;
    for (pin = port * 8; pin < lastPin; pin++) {
                                                      
      if (IS_PIN_DIGITAL(pin)) {
                                                          
                                                                 
        if (pinConfig[pin] == OUTPUT || pinConfig[pin] == INPUT) {
          pinWriteMask |= mask;
          pinState[pin] = ((byte)value & mask) ? 1 : 0;
        }
      }
      mask = mask << 1;
    }
    writePort(port, (byte)value, pinWriteMask);
  }
}


                                                                                
                                                                          
   
                                                                  
   
void reportAnalogCallback(byte analogPin, int value)
{
  if (analogPin < TOTAL_ANALOG_PINS) {
    if (value == 0) {
      analogInputsToReport = analogInputsToReport &~ (1 << analogPin);
    }
    else {
      analogInputsToReport = analogInputsToReport | (1 << analogPin);
    }
  }
                                                 
}

void reportDigitalCallback(byte port, int value)
{
  if (port < TOTAL_PORTS) {
    reportPINs[port] = (byte)value;
  }
                                                                   
                                                                     
                                                                   
                                                                
                                                                          
                              
}

                                                                                
                       
                                                                                

void sysexCallback(byte command, byte argc, byte *argv)
{
  byte mode;
  byte slaveAddress;
  byte slaveRegister;
  byte data;
  unsigned int delayTime;
  byte pin ;                
  int frequency ;
  int duration ;

  switch (command) {
    case I2C_REQUEST:
      mode = argv[1] & I2C_READ_WRITE_MODE_MASK;
      if (argv[1] & I2C_10BIT_ADDRESS_MODE_MASK) {
        Firmata.sendString("10-bit addressing mode is not yet supported");
        return;
      }
      else {
        slaveAddress = argv[0];
      }

      switch (mode) {
        case I2C_WRITE:
          Wire.beginTransmission(slaveAddress);
          for (byte i = 2; i < argc; i += 2) {
            data = argv[i] + (argv[i + 1] << 7);
#if ARDUINO >= 100
            Wire.write(data);
#else
            Wire.send(data);
#endif
          }
          Wire.endTransmission();
          delayMicroseconds(70);
          break;
        case I2C_READ:
          if (argc == 6) {
                                            
            slaveRegister = argv[2] + (argv[3] << 7);
            data = argv[4] + (argv[5] << 7);                  
            readAndReportData(slaveAddress, (int)slaveRegister, data);
          }
          else {
                                                
            data = argv[2] + (argv[3] << 7);                  
            readAndReportData(slaveAddress, (int)REGISTER_NOT_SPECIFIED, data);
          }
          break;
        case I2C_READ_CONTINUOUSLY:
          if ((queryIndex + 1) >= MAX_QUERIES) {
                                            
            Firmata.sendString("too many queries");
            break;
          }
          queryIndex++;
          query[queryIndex].addr = slaveAddress;
          query[queryIndex].reg = argv[2] + (argv[3] << 7);
          query[queryIndex].bytes = argv[4] + (argv[5] << 7);
          break;
        case I2C_STOP_READING:
          byte queryIndexToSkip;
                                                                              
                                                      
          if (queryIndex <= 0) {
            queryIndex = -1;
          }
          else {
                                                                       
                                                                               
                                                                      
            for (byte i = 0; i < queryIndex + 1; i++) {
              if (query[i].addr = slaveAddress) {
                queryIndexToSkip = i;
                break;
              }
            }

            for (byte i = queryIndexToSkip; i < queryIndex + 1; i++) {
              if (i < MAX_QUERIES) {
                query[i].addr = query[i + 1].addr;
                query[i].reg = query[i + 1].addr;
                query[i].bytes = query[i + 1].bytes;
              }
            }
            queryIndex--;
          }
          break;
        default:
          break;
      }
      break;
    case I2C_CONFIG:
      delayTime = (argv[0] + (argv[1] << 7));

      if (delayTime > 0) {
        i2cReadDelayTime = delayTime;
      }

      if (!isI2CEnabled) {
        enableI2CPins();
      }

      break;
    case SERVO_CONFIG:
      if (argc > 4) {
                                                                                  
        byte pin = argv[0];
        int minPulse = argv[1] + (argv[2] << 7);
        int maxPulse = argv[3] + (argv[4] << 7);

        if (IS_PIN_SERVO(pin)) {
          if (servos[PIN_TO_SERVO(pin)].attached())
            servos[PIN_TO_SERVO(pin)].detach();
          servos[PIN_TO_SERVO(pin)].attach(PIN_TO_DIGITAL(pin), minPulse, maxPulse);
          setPinModeCallback(pin, SERVO);
        }
      }
      break;
    case SAMPLING_INTERVAL:
      if (argc > 1) {
        samplingInterval = argv[0] + (argv[1] << 7);
        if (samplingInterval < MINIMUM_SAMPLING_INTERVAL) {
          samplingInterval = MINIMUM_SAMPLING_INTERVAL;
        }
                                                
        numLoops = INTER_PING_INTERVAL / samplingInterval ;
                        
      }
      else {
                                                
      }
      break;
    case EXTENDED_ANALOG:
      if (argc > 1) {
        int val = argv[1];
        if (argc > 2) val |= (argv[2] << 7);
        if (argc > 3) val |= (argv[3] << 14);
        analogWriteCallback(argv[0], val);
      }
      break;
    case CAPABILITY_QUERY:
      Firmata.write(START_SYSEX);
      Firmata.write(CAPABILITY_RESPONSE);
      for (byte pin = 0; pin < TOTAL_PINS; pin++) {
        if (IS_PIN_DIGITAL(pin)) {
          Firmata.write((byte)INPUT);
          Firmata.write(1);
          Firmata.write((byte)OUTPUT);
          Firmata.write(1);
        }
        if (IS_PIN_ANALOG(pin)) {
          Firmata.write(ANALOG);
          Firmata.write(10);
        }
        if (IS_PIN_PWM(pin)) {
          Firmata.write(PWM);
          Firmata.write(8);
        }
        if (IS_PIN_SERVO(pin)) {
          Firmata.write(SERVO);
          Firmata.write(14);
        }
        if (IS_PIN_I2C(pin)) {
          Firmata.write(I2C);
          Firmata.write(1);                                       
        }
        Firmata.write(127);
      }
      Firmata.write(END_SYSEX);
      break;
    case PIN_STATE_QUERY:
      if (argc > 0) {
        byte pin = argv[0];
        Firmata.write(START_SYSEX);
        Firmata.write(PIN_STATE_RESPONSE);
        Firmata.write(pin);
        if (pin < TOTAL_PINS) {
          Firmata.write((byte)pinConfig[pin]);
          Firmata.write((byte)pinState[pin] & 0x7F);
          if (pinState[pin] & 0xFF80) Firmata.write((byte)(pinState[pin] >> 7) & 0x7F);
          if (pinState[pin] & 0xC000) Firmata.write((byte)(pinState[pin] >> 14) & 0x7F);
        }
        Firmata.write(END_SYSEX);
      }
      break;
    case ANALOG_MAPPING_QUERY:
      Firmata.write(START_SYSEX);
      Firmata.write(ANALOG_MAPPING_RESPONSE);
      for (byte pin = 0; pin < TOTAL_PINS; pin++) {
        Firmata.write(IS_PIN_ANALOG(pin) ? PIN_TO_ANALOG(pin) : 127);
      }
      Firmata.write(END_SYSEX);
      break;
  case ENCODER_CONFIG:
                                                
                                
    encoderPin1 = argv[0] ;
    encoderPin2 = argv[1] ;
    static AdaEncoder encoder = 
      AdaEncoder('a', encoderPin1, encoderPin2) ;
                              
    encoderPresent = true ;
    break ; 


    case TONE_DATA:
      byte toneCommand, pin;
      int frequency, duration;

      toneCommand = argv[0];
      pin = argv[1];

      if (toneCommand == TONE_TONE) {
        frequency = argv[2] + (argv[3] << 7);
                                                     
        duration = argv[4] + (argv[5] << 7);
        tone(pin, frequency, duration);
      }
      else if (toneCommand == TONE_NO_TONE) {
        noTone(pin);
      }
      break ;

                           
                        
                                                                  
                               
                               
    case SONAR_CONFIG :
      int max_distance ;
      if ( numActiveSonars < MAX_SONARS)
      {
        sonarTriggerPin = argv[0] ;
        sonarEchoPin = argv[1] ;
                                             
        if ( argv[2] >= 33 ) {
          pingInterval = argv[2] ;
        }
        else {
          pingInterval = 33 ;
        }
        max_distance = argv[3] + (argv[4] << 7 ) ;
        sonarPinNumbers[numActiveSonars] = sonarTriggerPin ;

        setPinModeCallback(sonarTriggerPin, SONAR);
        setPinModeCallback(sonarEchoPin, SONAR);
        sonars[numActiveSonars] = new NewPing(sonarTriggerPin, sonarEchoPin, max_distance) ;
        numActiveSonars++ ;
      }
      else {
        Firmata.sendString("PING_CONFIG Error: Exceeded number of supported ping devices");
      }
      break ;

    case STEPPER_DATA:
                                                                                 
      if (argv[0] == STEPPER_CONFIGURE)
      {
        int numSteps = argv[1] + (argv[2] << 7);
        int pin1 = argv[3] ;
        int pin2 = argv[4] ;
        if ( argc == 5 )
        {
                          
          stepper = new Stepper(numSteps, pin1, pin2) ;
        }
        else if (argc == 7 )                
        {
          int pin3 = argv[5] ;
          int pin4 = argv[6] ;
          stepper =  new Stepper(numSteps, pin1, pin2, pin3, pin4) ;
        }
        else
        {
          Firmata.sendString("STEPPER CONFIG Error: Wrong Number of arguments");
          printData("argc = ", argc) ;
        }
      }
      else if ( argv[0] == STEPPER_STEP )
      {
        long speed = (long)argv[1] | ((long)argv[2] << 7) | ((long)argv[3] << 14);
        int numSteps = argv[4] + (argv[5] << 7);
        int direction = argv[6] ;
        if (stepper != NULL )
        {
          stepper->setSpeed(speed) ;
          if (direction == 0 )
          {
            numSteps *= -1 ;
          }
          stepper->step(numSteps) ;
        }
        else
        {
          Firmata.sendString("STEPPER OPERATE Error: MOTOR NOT CONFIGURED");
        }
      }
      else if ( argv[0] == STEPPER_LIBRARY_VERSION )
      {
        if ( stepper != NULL )
        {
          int version = stepper->version() ;
          Firmata.write(START_SYSEX);
          Firmata.write(STEPPER_DATA);
          Firmata.write(version & 0x7F);
          Firmata.write(version >> 7);
          Firmata.write(END_SYSEX);
        }
        else
        {
                                              
          Firmata.sendString("STEPPER FIRMWARE VERSION Error: NO MOTORS CONFIGURED");
        }
        break ;
      }
      else
      {
        Firmata.sendString("STEPPER CONFIG Error: UNKNOWN STEPPER COMMAND");
      }
      break ;
  }
}

void enableI2CPins()
{
  byte i;
                                                                      
                                      
  for (i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_I2C(i)) {
                                                                     
      setPinModeCallback(i, I2C);
    }
  }

  isI2CEnabled = true;

                                                                         
  Wire.begin();
}

                                                                  
void disableI2CPins() {
  isI2CEnabled = false;
                                                 
  queryIndex = -1;
                                                                                 
                
}

                                                                                
          
                                                                                

void systemResetCallback()
{
                              
                                                               
  if (isI2CEnabled) {
    disableI2CPins();
  }
  for (byte i = 0; i < TOTAL_PORTS; i++) {
    reportPINs[i] = false;                                  
    portConfigInputs[i] = 0;	                  
    previousPINs[i] = 0;
  }
                                                        
                                              
  for (byte i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
                                                
      setPinModeCallback(i, ANALOG);
    }
    else if ( IS_PIN_TONE(i)) {
      noTone(i) ;
    }
    else {
                                                          
      setPinModeCallback(i, OUTPUT);
    }
  }
                 
  numActiveSonars = 0 ;
  for (int i = 0; i < MAX_SONARS; i++) {
    sonarPinNumbers[i] = IGNORE ;
    if ( sonars[i] ) {
      sonars[i] = NULL ;
    }
  }
  numActiveSonars = 0 ;

                                                
  analogInputsToReport = 0;

                          
  stepper = NULL ;

                                                                       
                                                                        
    
                                                                      
                                                                
                                         
                                                         
    
     
}

void setup()
{


  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);

  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
  Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
  Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);
  Firmata.attach(SET_PIN_MODE, setPinModeCallback);
  Firmata.attach(START_SYSEX, sysexCallback);
  Firmata.attach(SYSTEM_RESET, systemResetCallback);

  numActiveSonars = 0 ;
  pingLoopCounter = 0 ;
  for (uint8_t i = 1; i < MAX_SONARS; i++)                                          
  {
    sonarPinNumbers[i] = IGNORE ;
    if ( sonars[i] ) {
      sonars[i] = NULL ;
    }
  }

                                          
  numLoops = INTER_PING_INTERVAL / samplingInterval ;

  Firmata.begin(57600);
  systemResetCallback();                            

}

                                                                                
         
                                                                                
void loop()
{
  byte pin, analogPin;
  int pingResult = 0;

                                                                                
                                         
  checkDigitalInputs();

                                                                               
                                 
  while (Firmata.available())
    Firmata.processInput();

                                                                            
                                                                      
                                   

  currentMillis = millis();
  if (currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;

    if ( pingLoopCounter++ > numLoops)
    {
      pingLoopCounter = 0 ;
      if (numActiveSonars)
      {
        unsigned int uS = sonars[nextSonar]->ping();
                                                        
        pingResult = uS / US_ROUNDTRIP_CM ;
        currentSonar = nextSonar ;
        if ( nextSonar++ >= numActiveSonars - 1)
        {
          nextSonar = 0 ;
        }
        sonarLSB = pingResult & 0x7f ;
        sonarMSB = pingResult >> 7 & 0x7f ;

        Firmata.write(START_SYSEX);
        Firmata.write(SONAR_DATA) ;
        Firmata.write(sonarPinNumbers[currentSonar]) ;
        Firmata.write(sonarLSB) ;
        Firmata.write(sonarMSB) ;
        Firmata.write(END_SYSEX);
      }
    }


                                                                               
    for (pin = 0; pin < TOTAL_PINS; pin++) {
      if (IS_PIN_ANALOG(pin) && pinConfig[pin] == ANALOG) {
        analogPin = PIN_TO_ANALOG(pin);
        if (analogInputsToReport & (1 << analogPin)) {
          Firmata.sendAnalog(analogPin, analogRead(analogPin));
        }
      }
    }
                                                                       
    if (queryIndex > -1) {
      for (byte i = 0; i < queryIndex + 1; i++) {
        readAndReportData(query[i].addr, query[i].reg, query[i].bytes);
      }
    }
                                                
    if ( encoderPresent == true)
    {
                                        
      encoderMSB = 0 ;
      encoderLSB = 0 ;
      clicks = 0 ;

      AdaEncoder *encoder = NULL;
      encoder = AdaEncoder::genie() ;
      if ( encoder != NULL) {
        clicks = encoder->query() ;
        if (clicks > 0) {
          encoderPostion += clicks ;
        }
        if (clicks < 0) {
          encoderPostion += clicks ;
        }
      }

      encoderLSB = encoderPostion & 0x7f ;
      encoderMSB = (encoderPostion >> 7) & 0x7f ;

      Firmata.write(START_SYSEX);
      Firmata.write(ENCODER_DATA) ;
      Firmata.write(encoderPin1) ;
      Firmata.write(encoderLSB) ;
      Firmata.write(encoderMSB) ;
      Firmata.write(END_SYSEX);
    }
  }
}



void printData(char * id,  long data)
{
  char myArray[64] ;

  String myString = String(data);
  myString.toCharArray(myArray, 64) ;
  Firmata.sendString(id) ;
  Firmata.sendString(myArray);
}




