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
#include"fetch.h"

#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>


static utils::highLevelMemory operatorsMemory(20000);
static utils::highLevelMemory BUFFER(1000);

const char *OPERATOR="OPERATOR";
const char *DATA="DATA";

const char *LOOP=               "LOOP";
const char *CONSOLE_LOGGER=     "CONSOLE_LOGGER";
const char *DIGITAL_OUTPUT=     "DIGITAL_OUTPUT";
const char *DIGITAL_INPUT=      "DIGITAL_INPUT";
const char *ANALOG_INPUT=       "ANALOG_INPUT";
const char *PWM=                "PWM";
const char *DELAY=              "DELAY";
const char *SEND=               "SEND";
const char *MEMORY_WRITE=       "MEMORY_WRITE";
const char *MEMORY_READ=        "MEMORY_READ";
const char *TIMER=              "TIMER";
const char *CLOCK_OUTPUT=       "CLOCK_OUTPUT";
const char *STORAGE_WRITE=      "STORAGE_WRITE";
const char *STORAGE_READ=       "STORAGE_READ";

void operatorsSetup(void){
    
    return;
}

uint8_t *threadRunner(uint8_t *operatorObject){
    static uint8_t firstRun;
    if(!firstRun){
        operatorsSetup();
        firstRun=1;
    }
    
    if(json(OPERATOR,operatorObject)==UNDEFINED){
        console.log("test threadRunner >> ",operatorObject);
        return operatorObject;
    }

    // std::string operatorData=(char*)json(DATA,operatorObject);
    operatorsMemory[json(OPERATOR,operatorObject)]=threadRunner(json(DATA,operatorObject));


    return operatorsMemory[json(OPERATOR,operatorObject)];
}


