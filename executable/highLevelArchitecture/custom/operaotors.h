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


// static utils::highLevelMemory operatorsMemory(20000);
static utils::highLevelMemory BUFFER(5000);
static utils::highLevelMemory RETURN_BUFFER(5000);

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

// void operatorsSetup(void){

//     operatorsMemory["readTest"]<<[&](void){
//         operatorsMemory["readTest"]="readTest !!!";
//         return;
//     };

//     operatorsMemory[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                            //& CONSOLE_LOGGER
//         console.log("->",operatorData);

//         return;
//     };
//     return;
// }

uint8_t* threadRunner(uint8_t *operatorObject){
    // console.log("threadRunner >> ",operatorObject);

    utils::highLevelMemory operatorsMemory(5000);

    ([&](utils::highLevelMemory &operatorsMemoryCallbacks){
        
        operatorsMemoryCallbacks["readTest"]<<[&](void){
            operatorsMemoryCallbacks["readTest"]="readTest !!!";
            return;
        };

        operatorsMemoryCallbacks[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                            //& CONSOLE_LOGGER
            console.log("->",operatorData);

            return;
        };

        return;
    })(operatorsMemory);

    static uint8_t recursionDepth;
    // static uint8_t firstRun;
    // if(!firstRun){
    //     operatorsSetup();
    //     firstRun=1;
    // }

    if(json(OPERATOR,operatorObject)==UNDEFINED)
        return operatorObject;
    recursionDepth++;

    BUFFER[recursionDepth]=json(DATA,operatorObject);

    uint8_t *bufferPtr=threadRunner(BUFFER[recursionDepth]);
    BUFFER[recursionDepth]=bufferPtr;

    // BUFFER[recursionDepth]=threadRunner(BUFFER[recursionDepth]);
    operatorsMemory[json(OPERATOR,operatorObject)]=BUFFER[recursionDepth];
    RETURN_BUFFER[recursionDepth]=operatorsMemory[json(OPERATOR,operatorObject)];

    // console.log("threadRunner >> ",(uint8_t*)RETURN_BUFFER[recursionDepth]);

    // recursionDepth--;
    // return operatorsMemory[json(OPERATOR,operatorObject)];
    return RETURN_BUFFER[recursionDepth--];
}


