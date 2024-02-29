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


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

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


void regsitersSetup(void){
    appLinker[PIN_MODE]>>[&](uint8_t *eventData){
        int modes[]={OUTPUT,INPUT,INPUT_PULLUP,INPUT_PULLDOWN};
		_PM(strint(json(PIN_NUMBER,eventData)),modes[strint(json(PIN_MODE,eventData))]);
	};

    appLinker[DIGITAL_WRITE]>>[&](uint8_t *eventData){
		_DW(strint(json(PIN_NUMBER,eventData)),strint(json(PIN_STATE,eventData)));
	};

    appLinker[DIGITAL_READ]<<[&](void){
		appLinker[DIGITAL_READ]=_DR(strint(json(PIN_NUMBER,eventData)));
	};
    
    appLinker[ANALOG_READ]<<[&](void){
		appLinker[ANALOG_READ]=analogRead(strint(json(PIN_NUMBER,eventData)));
	};

    return;
}