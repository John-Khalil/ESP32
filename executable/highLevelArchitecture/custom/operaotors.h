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

const char *LOOP="loopOperator";
const char *CONSOLE_LOGGER="loopOperator";
const char *DIGITAL_OUTPUT="loopOperator";
const char *DIGITAL_INPUT="loopOperator";
const char *ANALOG_INPUT="loopOperator";
const char *PWM="loopOperator";
const char *DELAY="loopOperator";
const char *SEND="loopOperator";
const char *MEMORY_WRITE="loopOperator";
const char *MEMORY_READ="loopOperator";
const char *TIMER="loopOperator";
const char *CLOCK_OUTPUT="loopOperator";
const char *STORAGE_WRITE="loopOperator";
const char *STORAGE_READ="loopOperator";

void operatorsSetup(void){
    
    return;
}

uint8_t *threadRunner(uint8_t *operatorObject){
    static uint8_t firstRun;
    if(!firstRun){
        operatorsSetup();
        firstRun=1;
    }

    if(json())

    return operatorObject;
}


