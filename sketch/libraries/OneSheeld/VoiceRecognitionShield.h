/*

  Project:       1Sheeld Library 
  File:          VoiceRecognitionShield.h
                 
  Version:       1.2

  Compiler:      Arduino avr-gcc 4.3.2

  Author:        Integreight
                 
  Date:          2014.9

*/
#ifndef	VoiceRecognitionShield_h
#define VoiceRecognitionShield_h

//Input Function ID's 
#define VOICE_GET 				0x01
#define VOICE_START_LISTENING	0x01
#define VOICE_GET_ERROR		    0x02
//Errors messages 
#define NETWORK_TIMEOUT_ERROR	0x01
#define NETWORK_ERROR 			0x02
#define AUDIO_ERROR 			0x03
#define SERVER_ERROR 			0x04
#define SPEECH_TIMEOUT_ERROR	0x06
#define NO_MATCH_ERROR			0x07
#define RECOGNIZER_BUSY_ERROR	0x08


class VoiceRecognitionShield
{
public:
	//Constructor 
	VoiceRecognitionShield();
	//Listener
	void start();
	//Getter
	char * getLastCommand();
	//Getter
	int getLastCommandLength();
	//Checker 
	bool isNewCommandReceived();
	//Setter
	void setOnNewCommand(void (*)(char*));
	//Setter
	void setOnError(void (*)(byte));
private:
	//Pointer to voice data in memory 
	char * voice;
	//Voice length
	int voicetextLength;
	//Boolean for Users function
	bool isCallBackAssigned;
	//Boolean for Users function
	bool errorAssigned;
	//Error number for user function 
	byte errorNumber;
	//Boolean for newCommand function
	bool newCommand; 
	//Process Input Data
	void processData();
	//Setter 
	void (*changeCallBack)(char*);
	//Setter
	void (*errorCallBack)(byte);
	//Friend Class to OneSheeld Class
	friend class OneSheeldClass;
};
//Extern Object
extern VoiceRecognitionShield VoiceRecognition;
#endif