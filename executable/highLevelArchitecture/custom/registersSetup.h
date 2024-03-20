#pragma once

#include "consoleLogger.h"
#include "consoleLogger.cpp"
#include "highLevelMemory.cpp"
#include "pointerTool.h"
#include "constDefinitions.h"
#include "eepromBasicConfig.h"
#include "networkUtils.h"
#include "platform.h"
#include "http.h"
#include "webService.h"
#include "fetch.h"

#include "jsonParser.h"
#include "localStorage.h"
#include "cyclicBinary.cpp"
#include "operaotors.h"


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

static std::vector<std::function<void(uint32_t)>>registersThreads;

utils::highLevelMemory REGISTER_MATRIX(512);
static const char *MAIN_TX_REGISTER=	"MQTT_TX";

static const char *PIN_MODE=            "PIN_MODE";
static const char *PIN_NUMBER=          "PIN_NUMBER";
static const char *PIN_STATE=           "PIN_STATE";
static const char *DIGITAL_WRITE=       "DIGITAL_WRITE";
static const char *DIGITAL_READ=        "DIGITAL_READ";
static const char *PIN_OUTPUT=          "0";
static const char *PIN_INPUT=           "1";
static const char *PIN_INPUT_PULLUP=    "2";
static const char *PIN_INPUT_PULLDOWN=  "3";
static const char *PIN_HIGH=            "0";
static const char *PIN_LOW=             "1";
static const char *ANALOG_READ=         "ANALOG_READ";

static const char *SERIAL_SETUP=		"SERIAL_SETUP";
static const char *SERIAL_SEND=			"SERIAL_SEND";
static const char *SERIAL_RX_REGISTER=	"SERIAL_RX_REGISTER";
static const char *SERIAL_BUFFER=		"SERIAL_BUFFER";
static const char *SERIAL_BAUD=			"SERIAL_BAUD";
static const char *SERIAL_TX_PIN=		"SERIAL_TX_PIN";
static const char *SERIAL_RX_PIN=		"SERIAL_RX_PIN";



void regsitersSetup(void){

    appLinker["testRegister"]>>[&](uint8_t *eventData){
		console.log("eventData >> ",eventData);
		return;
	};

    appLinker["testRegister"]<<[&](void){
		appLinker["testRegister"]="this is a read test !!";
		return;
	};

    appLinker[PIN_MODE]>>[&](uint8_t *eventData){
        int modes[]={OUTPUT,INPUT,INPUT_PULLUP,INPUT_PULLDOWN};
		_PM(strint(json(PIN_NUMBER,eventData)),modes[strint(json(PIN_MODE,eventData))]);

		return;
	};

    appLinker[DIGITAL_WRITE]>>[&](uint8_t *eventData){
		_DW(strint(json(PIN_NUMBER,eventData)),strint(json(PIN_STATE,eventData)));

		return;
	};

    appLinker[DIGITAL_READ]<<[&](void){
        uint32_t digitalReadValue=_DR(strint(json(PIN_NUMBER,(uint8_t*)appLinker[DIGITAL_READ])));
		appLinker[DIGITAL_READ]=inttostring(digitalReadValue);

		return;
	};

    appLinker[ANALOG_READ]<<[&](void){
        uint32_t analogReadValue=analogRead(strint(json(PIN_NUMBER,(uint8_t*)appLinker[ANALOG_READ])));
		appLinker[ANALOG_READ]=inttostring(analogReadValue);

		return;
	};

	appLinker[SERIAL_SETUP]>>[&](uint8_t *eventData){
		uint32_t baudRate=strint(json(SERIAL_BAUD,eventData));
		uint32_t txPin=strint(json(SERIAL_TX_PIN,eventData));
		uint32_t rxPin=strint(json(SERIAL_RX_PIN,eventData));
		Serial1.begin(baudRate, SERIAL_8N1, rxPin, txPin);

		auto serialEvent=[&](uint32_t systick){
			#define TIME_OUT 20
			if(!systick%TIME_OUT){
				static uint32_t receivedBytes=-1UL;
				uint32_t currentBytes=Serial1.available();
				if(currentBytes==receivedBytes){
					operatorsMemory.write((uint8_t*)SERIAL_BUFFER,(uint8_t*)"",((currentBytes*1.334)+20));		//* dynamic memory allocation -- operatorsMemory[SERIAL_BUFFER] should be the new address
					uint8_t *serialBuffer=operatorsMemory[SERIAL_BUFFER];


					// base64Encode((uint8_t*)Serial1.readString().c_str(),,currentBytes);
					
					operatorsMemory[SERIAL_BUFFER]=(uint8_t*)"";
				}
				receivedBytes=currentBytes;
			}
		};

		static uint32_t threadAddress=-1UL;
		if(threadAddress==-1UL){
			threadAddress=registersThreads.size();
			registersThreads.push_back(serialEvent);
		}
		else
			registersThreads[threadAddress]=serialEvent;
		// Serial1.onReceive(NULL);
		// Serial1.onReceive([&](void){

		// },true);

		return;
	};

    return;
}

void runRegistersThreads(std::function<void(void)>systemDelay){
	static uint32_t sysTick;
	static uint64_t vectorIndexCounter;
	registersThreads[vectorIndexCounter++%registersThreads.size()](sysTick++);
	systemDelay();
	
	return;
}