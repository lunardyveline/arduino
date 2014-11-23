
/*
 * Firmata is a generic protocol for communicating with microcontrollers
 * from software on a host computer. It is intended to work with
 * any host computer software package.
 *
 * To download a host software package, please clink on the following link
 * to open the download page in your default browser.ser
 *
 * http://firmata.org/wiki/Download
 */

/*
 Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
 Copyright (C) 2010-2011 Paul Stoffregen.  All rights reserved.
 Copyright (C) 2009 Shigeru Kobayashi.  All rights reserved.
 Copyright (C) 2009-2011 Jeff Hoefs.  All rights reserved.
 Copyright (C) 2013-2014 Alan Yorinks. All rights reserved.


 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 See file LICENSE.txt for further informations on licensing terms.

 formatted using the GNU C formatting and indenting
 */
 
 /* This sketch provides additional functionality to StandardFirmata. 
    It provides support for:
    Tone
    Stepper
    NewPing
    Encoder - Note that the encoder works for an Arduino UNO and does not function
              with a Leonardo. It may not work for others boards as well.

/*
 * TODO: use Program Control to load stored profiles from EEPROM
 */

#include <Servo.h>
#include <Wire.h>
#include <FirmataPlusLBT.h>
#include <NewPing.h>
#include <Stepper.h>
#include <ooPinChangeInt.h> https://code.google.com/p/arduino-pinchangeint/downloads/detail?name=pinchangeint-v2.19beta.zip
#include <AdaEncoder.h> https://code.google.com/p/adaencoder/downloads/detail?name=adaencoder-v0.7beta.zip


// move the following defines to Firmata.h?
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

#define INTER_PING_INTERVAL 40 // 40 ms.

// SYSEX command sub specifiers
#define TONE_TONE 0
#define TONE_NO_TONE 1

#define STEPPER_CONFIGURE 0
#define STEPPER_STEP 1
#define STEPPER_LIBRARY_VERSION 2


/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/* analog inputs */
int analogInputsToReport = 0; // bitwise array to store pin reporting

/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent

/* pins configuration */
byte pinConfig[TOTAL_PINS];         // configuration of every pin
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int pinState[TOTAL_PINS];           // any value that has been written

/* timer variables */
unsigned long currentMillis;        // store the current value from millis()
unsigned long previousMillis;       // for comparison with currentMillis
int samplingInterval = 19;          // how often to run the main loop (in ms)

/* i2c data */
struct i2c_device_info {
  byte addr;
  byte reg;
  byte bytes;
};

/* for i2c read continuous more */
i2c_device_info query[MAX_QUERIES];

byte i2cRxData[32];
boolean isI2CEnabled = false;
signed char queryIndex = -1;
unsigned int i2cReadDelayTime = 0;  // default delay time between i2c read request and Wire.requestFrom()


// Servos
Servo servos[MAX_SERVOS];

/* Rotary Encoder Support */
byte  encoderMSB, encoderLSB ;     // sysex data registers
uint8_t encoderPin1, encoderPin2 ; // user specified encoder pins
int encoderPostion = 0;            // current position of encoder
int8_t clicks = 0 ;                // encoder click counter
boolean encoderPresent = false ;   // encoder installed flag

// Ping variables

int numLoops = 0 ;
int pingLoopCounter = 0 ;

int numActiveSonars = 0 ; // number of sonars attached
uint8_t sonarPinNumbers[MAX_SONARS] ;
int nextSonar = 0 ; // index into sonars[] for next device

// array to hold up to 6 instances of sonar devices
NewPing *sonars[MAX_SONARS] ;

uint8_t sonarTriggerPin;
uint8_t sonarEchoPin ;
uint8_t currentSonar = 0;            // Keeps track of which sensor is active.

uint8_t pingInterval = 33 ;  // Milliseconds between sensor pings (29ms is about the min to avoid
// cross- sensor echo).
byte sonarMSB, sonarLSB ;


// Stepper Motor

Stepper *stepper = NULL;


/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

void readAndReportData(byte address, int theRegister, byte numBytes) {
  // allow I2C requests that don't require a register read
  // for example, some devices using an interrupt pin to signify new data available
  // do not always require the register read so upon interrupt you call Wire.requestFrom()
  if (theRegister != REGISTER_NOT_SPECIFIED) {
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write((byte)theRegister);
#else
    Wire.send((byte)theRegister);
#endif
    Wire.endTransmission();
    // do not set a value of 0
    if (i2cReadDelayTime > 0) {
      // delay is necessary for some devices such as WiiNunchuck
      delayMicroseconds(i2cReadDelayTime);
    }
  }
  else {
    theRegister = 0;  // fill the register with a dummy value
  }

  Wire.requestFrom(address, numBytes);  // all bytes are returned in requestFrom

  // check to be sure correct number of bytes were returned by slave
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

  // send slave address, register and received bytes
  Firmata.sendSysex(SYSEX_I2C_REPLY, numBytes + 2, i2cRxData);
}

void outputPort(byte portNumber, byte portValue, byte forceSend)
{
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if (forceSend || previousPINs[portNumber] != portValue) {
    Firmata.sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void checkDigitalInputs(void)
{
  /* Using non-looping code allows constants to be given to readPort().
   * The compiler will apply substantial optimizations if the inputs
   * to readPort() are compile-time constants. */
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

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(byte pin, int mode)
{
  if (pinConfig[pin] == I2C && isI2CEnabled && mode != I2C) {
    // disable i2c so pins can be used for other functions
    // the following if statements should reconfigure the pins properly
    disableI2CPins();
  }
  if (IS_PIN_SERVO(pin) && mode != SERVO && servos[PIN_TO_SERVO(pin)].attached()) {
    servos[PIN_TO_SERVO(pin)].detach();
  }
  if (IS_PIN_ANALOG(pin)) {
    reportAnalogCallback(PIN_TO_ANALOG(pin), mode == ANALOG ? 1 : 0); // turn on/off reporting
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
          pinMode(PIN_TO_DIGITAL(pin), INPUT); // disable output driver
          digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable internal pull-ups
        }
        pinConfig[pin] = ANALOG;
      }
      break;
    case INPUT:
      if (IS_PIN_DIGITAL(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), INPUT); // disable output driver
        digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable internal pull-ups
        pinConfig[pin] = INPUT;
      }
      break;
    case OUTPUT:
      if (IS_PIN_DIGITAL(pin)) {
        digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable PWM
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
      // enable the pullups for an encoder pin
      pinMode(pin, INPUT);
      digitalWrite(pin, HIGH);

      // used as part of encoder sysex message
      pinConfig[pin] = ENCODER ;
      break ;

    case I2C:
      if (IS_PIN_I2C(pin)) {
        // mark the pin as i2c
        // the user must call I2C_CONFIG to enable I2C for a device
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
      Firmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
      break ;
  }
  // TODO: save status to EEPROM here, if changed
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
    // create a mask of the pins on this port that are writable.
    lastPin = port * 8 + 8;
    if (lastPin > TOTAL_PINS) lastPin = TOTAL_PINS;
    for (pin = port * 8; pin < lastPin; pin++) {
      // do not disturb non-digital pins (eg, Rx & Tx)
      if (IS_PIN_DIGITAL(pin)) {
        // only write to OUTPUT and INPUT (enables pullup)
        // do not touch pins in PWM, ANALOG, SERVO or other modes
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


// -----------------------------------------------------------------------------
/* sets bits in a bit array (int) to toggle the reporting of the analogIns
 */
//void FirmataClass::setAnalogPinReporting(byte pin, byte state) {
//}
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
  // TODO: save status to EEPROM here, if changed
}

void reportDigitalCallback(byte port, int value)
{
  if (port < TOTAL_PORTS) {
    reportPINs[port] = (byte)value;
  }
  // do not disable analog reporting on these 8 pins, to allow some
  // pins used for digital, others analog.  Instead, allow both types
  // of reporting to be enabled, but check if the pin is configured
  // as analog when sampling the analog inputs.  Likewise, while
  // scanning digital pins, portConfigInputs will mask off values from any
  // pins configured as analog
}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv)
{
  byte mode;
  byte slaveAddress;
  byte slaveRegister;
  byte data;
  unsigned int delayTime;
  byte pin ;// used for tone
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
            // a slave register is specified
            slaveRegister = argv[2] + (argv[3] << 7);
            data = argv[4] + (argv[5] << 7);  // bytes to read
            readAndReportData(slaveAddress, (int)slaveRegister, data);
          }
          else {
            // a slave register is NOT specified
            data = argv[2] + (argv[3] << 7);  // bytes to read
            readAndReportData(slaveAddress, (int)REGISTER_NOT_SPECIFIED, data);
          }
          break;
        case I2C_READ_CONTINUOUSLY:
          if ((queryIndex + 1) >= MAX_QUERIES) {
            // too many queries, just ignore
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
          // if read continuous mode is enabled for only 1 i2c device, disable
          // read continuous reporting for that device
          if (queryIndex <= 0) {
            queryIndex = -1;
          }
          else {
            // if read continuous mode is enabled for multiple devices,
            // determine which device to stop reading and remove it's data from
            // the array, shifiting other array data to fill the space
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
        // these vars are here for clarity, they'll optimized away by the compiler
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
        /* calculate number of loops per ping */
        numLoops = INTER_PING_INTERVAL / samplingInterval ;
        //numLoops = 1 ;
      }
      else {
        //Firmata.sendString("Not enough data");
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
          Firmata.write(1);  // to do: determine appropriate value
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
    // instantiate an encoder object with user's
    // requested pin designators
    encoderPin1 = argv[0] ;
    encoderPin2 = argv[1] ;
    static AdaEncoder encoder = 
      AdaEncoder('a', encoderPin1, encoderPin2) ;
    //thisEncoder = &encoder ;
    encoderPresent = true ;
    break ; 


    case TONE_DATA:
      byte toneCommand, pin;
      int frequency, duration;

      toneCommand = argv[0];
      pin = argv[1];

      if (toneCommand == TONE_TONE) {
        frequency = argv[2] + (argv[3] << 7);
        // duration is currently limited to 16,383 ms
        duration = argv[4] + (argv[5] << 7);
        tone(pin, frequency, duration);
      }
      else if (toneCommand == TONE_NO_TONE) {
        noTone(pin);
      }
      break ;

      // arg0 = trigger pin
      // arg1 = echo pin
      // arg2 = ping interval in milliseconds if 0, then set to 33
      // arg3 = maxdistance lsb
      // arg4 = maxdistance msb
    case SONAR_CONFIG :
      int max_distance ;
      if ( numActiveSonars < MAX_SONARS)
      {
        sonarTriggerPin = argv[0] ;
        sonarEchoPin = argv[1] ;
        // set interval to a minium of 33 ms.
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
      // determine if this a STEPPER_CONFIGURE command or STEPPER_OPERATE command
      if (argv[0] == STEPPER_CONFIGURE)
      {
        int numSteps = argv[1] + (argv[2] << 7);
        int pin1 = argv[3] ;
        int pin2 = argv[4] ;
        if ( argc == 5 )
        {
          // two pin motor
          stepper = new Stepper(numSteps, pin1, pin2) ;
        }
        else if (argc == 7 ) // 4 wire motor
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
          // did not find a configured stepper
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
  // is there a faster way to do this? would probaby require importing
  // Arduino.h to get SCL and SDA pins
  for (i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_I2C(i)) {
      // mark pins as i2c so they are ignore in non i2c data requests
      setPinModeCallback(i, I2C);
    }
  }

  isI2CEnabled = true;

  // is there enough time before the first I2C request to call this here?
  Wire.begin();
}

/* disable the i2c pins so they can be used for other functions */
void disableI2CPins() {
  isI2CEnabled = false;
  // disable read continuous mode for all devices
  queryIndex = -1;
  // uncomment the following if or when the end() method is added to Wire library
  // Wire.end();
}

/*==============================================================================
 * SETUP()
 *============================================================================*/

void systemResetCallback()
{
  // initialize a defalt state
  // TODO: option to load config from EEPROM instead of default
  if (isI2CEnabled) {
    disableI2CPins();
  }
  for (byte i = 0; i < TOTAL_PORTS; i++) {
    reportPINs[i] = false;      // by default, reporting off
    portConfigInputs[i] = 0;	// until activated
    previousPINs[i] = 0;
  }
  // pins with analog capability default to analog input
  // otherwise, pins default to digital output
  for (byte i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
      // turns off pullup, configures everything
      setPinModeCallback(i, ANALOG);
    }
    else if ( IS_PIN_TONE(i)) {
      noTone(i) ;
    }
    else {
      // sets the output to 0, configures portConfigInputs
      setPinModeCallback(i, OUTPUT);
    }
  }
  // stop pinging
  numActiveSonars = 0 ;
  for (int i = 0; i < MAX_SONARS; i++) {
    sonarPinNumbers[i] = IGNORE ;
    if ( sonars[i] ) {
      sonars[i] = NULL ;
    }
  }
  numActiveSonars = 0 ;

  // by default, do not report any analog inputs
  analogInputsToReport = 0;

  // clear stepper pointer
  stepper = NULL ;

  /* send digital inputs to set the initial state on the host computer,
   * since once in the loop(), this firmware will only send on change */
  /*
  TODO: this can never execute, since no pins default to digital input
   but it will be needed when/if we support EEPROM stored config
   for (byte i=0; i < TOTAL_PORTS; i++) {
   outputPort(i, readPort(i, portConfigInputs[i]), true);
   }
   */
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
  for (uint8_t i = 1; i < MAX_SONARS; i++) // Set the starting time for each sensor.
  {
    sonarPinNumbers[i] = IGNORE ;
    if ( sonars[i] ) {
      sonars[i] = NULL ;
    }
  }

  /* calculate number of loops per ping */
  numLoops = INTER_PING_INTERVAL / samplingInterval ;

  Firmata.begin(57600);
  systemResetCallback();  // reset to default config

}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop()
{
  byte pin, analogPin;
  int pingResult = 0;

  /* DIGITALREAD - as fast as possible, check for changes and output them to the
   * FTDI buffer using Serial.print()  */
  checkDigitalInputs();

  /* SERIALREAD - processing incoming messagse as soon as possible, while still
   * checking digital inputs.  */
  while (Firmata.available())
    Firmata.processInput();

  /* SEND FTDI WRITE BUFFER - make sure that the FTDI buffer doesn't go over
   * 60 bytes. use a timer to sending an event character every 4 ms to
   * trigger the buffer to dump. */

  currentMillis = millis();
  if (currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;

    if ( pingLoopCounter++ > numLoops)
    {
      pingLoopCounter = 0 ;
      if (numActiveSonars)
      {
        unsigned int uS = sonars[nextSonar]->ping();
        // Convert ping time to distance in cm and print
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


    /* ANALOGREAD - do all analogReads() at the configured sampling interval */
    for (pin = 0; pin < TOTAL_PINS; pin++) {
      if (IS_PIN_ANALOG(pin) && pinConfig[pin] == ANALOG) {
        analogPin = PIN_TO_ANALOG(pin);
        if (analogInputsToReport & (1 << analogPin)) {
          Firmata.sendAnalog(analogPin, analogRead(analogPin));
        }
      }
    }
    // report i2c data for all device with read continuous mode enabled
    if (queryIndex > -1) {
      for (byte i = 0; i < queryIndex + 1; i++) {
        readAndReportData(query[i].addr, query[i].reg, query[i].bytes);
      }
    }
    // if encoder was installed, return its data
    if ( encoderPresent == true)
    {
      // read encoder data and return it
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



