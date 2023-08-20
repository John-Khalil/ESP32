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

#include "jsonParser.h"
#include "localStorage.h"


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>


// static utils::highLevelMemory operatorsMemory(20000);
// static utils::highLevelMemory BUFFER(5000);
// static utils::highLevelMemory RETURN_BUFFER(5000);

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

const char *LOOP_COUNTER=       "LOOP_COUNTER";
const char *LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";


;







utils::highLevelMemory& instruction(utils::highLevelMemory& operatorObject){

    // console.log("start >> ",(uint8_t*)operatorObject[OPERATOR]);

    if(json(OPERATOR,operatorObject[OPERATOR])==UNDEFINED)
        return operatorObject[OPERATOR];

    utils::highLevelMemory operatorObjectMemory(5000);


    ([&](utils::highLevelMemory &operatorsMemoryCallbacks){
            
        operatorsMemoryCallbacks["readTest"]<<[&](void){
            operatorsMemoryCallbacks["readTest"]="readTest !!!";
            return;
        };

        operatorsMemoryCallbacks[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                           //& CONSOLE_LOGGER
            console.log("-> ",operatorData);

            return;
        };

        operatorsMemoryCallbacks[LOOP]>>[&](uint8_t *operatorData){                                     //& LOOP
            utils::highLevelMemory loopMemory(600);

            // console.log("LOOP --->> ",operatorData);

            loopMemory[OPERATOR]=json(LOOP_COUNTER,operatorData);
            uint32_t loopLimit=getInt32_t(instruction(loopMemory[OPERATOR]));
            while(loopLimit--){
                uint16_t loopCounter=0;
                while((loopMemory[OPERATOR]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED)
                    instruction(loopMemory[OPERATOR]);
            }

            return;


            return;
        };

        return;
    })(operatorObjectMemory);

    operatorObjectMemory[OPERATOR]=json(DATA,operatorObject[OPERATOR]);

    instruction(operatorObjectMemory[OPERATOR]);

    // uint8_t* operatorData=operatorObjectMemory[OPERATOR];

    operatorObjectMemory[json(OPERATOR,operatorObject[OPERATOR])]=operatorObjectMemory[OPERATOR];

    // uint8_t* returnData=operatorObjectMemory[json(OPERATOR,operatorObject)];

    operatorObject[OPERATOR]=operatorObjectMemory[json(OPERATOR,operatorObject[OPERATOR])];

    return operatorObject[OPERATOR];
}

void threadRunner(uint8_t *operatorObject){
    // console.log("operatorObject >> ",operatorObject);
    utils::highLevelMemory operatorObjectMemory(5000);
    operatorObjectMemory[OPERATOR]=operatorObject;
    instruction(operatorObjectMemory[OPERATOR]);
    return;
}

















// utils::highLevelMemory operatorsMemory(5000)





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



// uint8_t *threadRunner(uint8_t * operatorObject){
//     static uint32_t firstRun;
//     if(!firstRun--)
//         ([&](utils::highLevelMemory &operatorsMemoryCallbacks){
            
//             operatorsMemoryCallbacks["readTest"]<<[&](void){
//                 operatorsMemoryCallbacks["readTest"]="readTest !!!";
//                 return;
//             };

//             operatorsMemoryCallbacks[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                           //& CONSOLE_LOGGER
//                 console.log("-> ",operatorData);

//                 return;
//             };

//             operatorsMemoryCallbacks[LOOP]>>[&](uint8_t *operatorData){                                     //& LOOP
//                 std::string operatorObject=(char*)operatorData;
//                 jsonParser operatorJsonObject(operatorObject);

//                 operatorJsonObject[LOOP_COUNTER];
//                 uint32_t loopCounter=getInt32_t(threadRunner(operatorJsonObject));

//                 while(loopCounter--){
//                     uint16_t loopIterator=0;
//                     while(operatorJsonObject[$(LOOP_ELEMENENTS,"[",loopIterator++,"]")]!=UNDEFINED){
//                         console.log((uint8_t*)operatorJsonObject);
//                         // threadRunner(operatorJsonObject);
//                     }
//                         // console.log(operatorJsonObject);
//                 }


//                 return;
//             };

//             return;
//         })(operatorsMemory);


    

//     // auto operatorIdentifier=jsonParser(operatorObject)[OPERATOR];
//     jsonParser operatorIdentifier(operatorObject);
//     operatorIdentifier[OPERATOR];

//     if(((uint8_t*)operatorIdentifier)==UNDEFINED)
//         return operatorObject;

//     // auto data=jsonParser(operatorObject)[DATA];
//     jsonParser data(operatorObject);
//     data[DATA];
//     std::string operatorReturn=(char*)threadRunner(data);
//     operatorsMemory[operatorIdentifier]=operatorReturn;
//     // operatorsMemory[operatorIdentifier]=threadRunner(data);      // this doesnt work
//     return operatorsMemory[operatorIdentifier];

// }






// uint8_t* threadRunner(uint8_t *operatorObject){
//     // console.log("threadRunner >> ",operatorObject);

//     utils::highLevelMemory operatorsMemory(5000);

//     ([&](utils::highLevelMemory &operatorsMemoryCallbacks){
        
//         operatorsMemoryCallbacks["readTest"]<<[&](void){
//             operatorsMemoryCallbacks["readTest"]="readTest !!!";
//             return;
//         };

//         operatorsMemoryCallbacks[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                           //& CONSOLE_LOGGER
//             console.log("-> ",operatorData);

//             return;
//         };

//         operatorsMemoryCallbacks[LOOP]>>[&](uint8_t *operatorData){                                     //& LOOP
//             utils::highLevelMemory loopMemory(600);

//             // console.log("LOOP --->> ",operatorData);

//             loopMemory[LOOP_COUNTER]=json(LOOP_COUNTER,operatorData);
//             uint32_t loopLimit=getInt32_t(threadRunner(loopMemory[LOOP_COUNTER]));
//             while(loopLimit--){
//                 uint16_t loopCounter=0;
//                 while((loopMemory[LOOP_ELEMENENTS]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED)
//                     threadRunner(loopMemory[LOOP_ELEMENENTS]);
//             }

//             return;
//         };

//         return;
//     })(operatorsMemory);

//     static uint8_t recursionDepth;
//     // static uint8_t firstRun;
//     // if(!firstRun){
//     //     operatorsSetup();
//     //     firstRun=1;
//     // }

//     if(json(OPERATOR,operatorObject)==UNDEFINED)
//         return operatorObject;

//     recursionDepth++;

//     BUFFER[recursionDepth]=json(DATA,operatorObject);

//     uint8_t *bufferPtr=threadRunner(BUFFER[recursionDepth]);
//     // if(BUFFER[recursionDepth]!=bufferPtr)
//         BUFFER[recursionDepth]=bufferPtr;

//     // BUFFER[recursionDepth]=threadRunner(BUFFER[recursionDepth]);
//     operatorsMemory[json(OPERATOR,operatorObject)]=BUFFER[recursionDepth];
//     RETURN_BUFFER[recursionDepth]=operatorsMemory[json(OPERATOR,operatorObject)];

//     // console.log("threadRunner >> ",(uint8_t*)RETURN_BUFFER[recursionDepth]);

//     // recursionDepth--;
//     // return operatorsMemory[json(OPERATOR,operatorObject)];

//     return RETURN_BUFFER[recursionDepth--];
// }


