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
static const char *PIN_OUTPUT=          "PIN_MODE";
static const char *PIN_INPUT=           "PIN_MODE";
static const char *PIN_INPUT_PULLUP=    "PIN_MODE";
static const char *PIN_INPUT_PULLDOWN=  "PIN_MODE";


void regsitersSetup(void){
    appLinker[PIN_MODE]>>[&](uint8_t *eventData){
        int modes[]={OUTPUT,INPUT,INPUT_PULLUP,INPUT_PULLDOWN};
		_PM(strint(json(PIN_NUMBER,eventData)),modes[strint(json(PIN_MODE,eventData))]);
	};

    appLinker[DIGITAL_WRITE]>>[&](uint8_t *eventData){
		
	};

    appLinker[DIGITAL_READ]<<[&](void){
		
	};

    return;
}