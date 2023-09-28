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





static utils::highLevelMemory operatorsMemory(BUFFER_SIZE_5);
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
const char *MEMORY_ADDRESS=     "MEMORY_ADDRESS";
const char *MEMORY_DATA=        "MEMORY_DATA";
const char *TIMER=              "TIMER";
const char *CLOCK_OUTPUT=       "CLOCK_OUTPUT";
const char *STORAGE_WRITE=      "STORAGE_WRITE";
const char *STORAGE_READ=       "STORAGE_READ";
const char *STORAGE_ADDRESS=    "STORAGE_ADDRESS";
const char *STORAGE_DATA=       "STORAGE_DATA";

const char *LOOP_COUNTER=       "LOOP_COUNTER";
const char *LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";

const char *THREAD_ADD=         "ADD_THREAD";
const char *THREAD_ID=          "THREAD_ID";
const char *THREAD_EXECUTABLE=  "THREAD_EXECUTABLE";
const char *THREAD_PRIORITY=    "THREAD_PRIORITY";










utils::highLevelMemory& instruction(utils::highLevelMemory& operatorObject){

    // console.log("start >> ",(uint8_t*)operatorObject[OPERATOR]);

    if(json(OPERATOR,operatorObject[OPERATOR])==UNDEFINED)
        return operatorObject[OPERATOR];

    utils::highLevelMemory operatorObjectMemory(BUFFER_SIZE_1);


    ([&](utils::highLevelMemory &operatorsMemoryCallbacks){
            
        operatorsMemoryCallbacks["readTest"]<<[&](void){
            operatorsMemoryCallbacks["readTest"]="readTest !!!";
            return;
        };

        operatorsMemoryCallbacks[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                           //^ CONSOLE_LOGGER
            static uint16_t logCounter;

            console.log("[",++logCounter,"]-> ",operatorData);

            return;
        };

        operatorsMemoryCallbacks[LOOP]>>[&](uint8_t *operatorData){                                     //^ LOOP
            utils::highLevelMemory loopMemory(BUFFER_SIZE_1);

            // console.log("LOOP --->> ",operatorData);

            loopMemory[OPERATOR]=json(LOOP_COUNTER,operatorData);
            uint32_t loopLimit=getInt32_t(instruction(loopMemory[OPERATOR]));
            while(loopLimit--){
                uint16_t loopCounter=0;
                while((loopMemory[OPERATOR]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED)
                    instruction(loopMemory[OPERATOR]);
            }

            return;

        };

        operatorsMemoryCallbacks[MEMORY_WRITE]<<[&](void){                                              //^ MEMORY_WRITE
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorsMemoryCallbacks[MEMORY_WRITE]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[MEMORY_ADDRESS]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(MEMORY_DATA,operatorsMemoryCallbacks[MEMORY_WRITE]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[MEMORY_DATA]=localBuffer[OPERATOR];


            operatorsMemory[(uint8_t*)localBuffer[MEMORY_ADDRESS]]=localBuffer[MEMORY_DATA];
            operatorsMemoryCallbacks[MEMORY_WRITE]=operatorsMemory[(uint8_t*)localBuffer[MEMORY_ADDRESS]];
            return;
        };

        operatorsMemoryCallbacks[MEMORY_READ]<<[&](void){                                               //^ MEMORY_READ
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
            localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorsMemoryCallbacks[MEMORY_READ]);
            instruction(localBuffer[OPERATOR]);

            operatorsMemoryCallbacks[MEMORY_READ]=operatorsMemory[(uint8_t*)localBuffer[OPERATOR]];
            return;
        };

        operatorsMemoryCallbacks[STORAGE_WRITE]<<[&](void){                                              //^ STORAGE_WRITE
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorsMemoryCallbacks[STORAGE_WRITE]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[STORAGE_ADDRESS]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(STORAGE_DATA,operatorsMemoryCallbacks[STORAGE_WRITE]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[STORAGE_DATA]=localBuffer[OPERATOR];

            operatorsMemoryCallbacks[STORAGE_WRITE]=localStorage.setItem((char*)localBuffer[STORAGE_ADDRESS],localBuffer[STORAGE_DATA]).c_str();
            return;
        };

        operatorsMemoryCallbacks[STORAGE_READ]<<[&](void){                                               //^ STORAGE_READ
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
            localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorsMemoryCallbacks[STORAGE_READ]);
            instruction(localBuffer[OPERATOR]);

            operatorsMemoryCallbacks[STORAGE_READ]=localStorage.getItem((char*)localBuffer[OPERATOR]).c_str();
            return;
        };

        operatorsMemoryCallbacks[THREAD_ADD]>>[&](uint8_t *threadData){                                 //^ THREAD_ADD
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            localBuffer[OPERATOR]=json(THREAD_ID,threadData);
            instruction(localBuffer[OPERATOR]);
            localBuffer[THREAD_ID]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(THREAD_EXECUTABLE,threadData);
            instruction(localBuffer[OPERATOR]);
            localBuffer[THREAD_EXECUTABLE]=localBuffer[OPERATOR];

            operatorsMemory[([&](uint8_t *threadID){
                uint32_t loopCounter=0;
                while(stringCounter(operatorsMemory[loopCounter])&&!equalStrings(operatorsMemory[loopCounter],threadID))
                    loopCounter++;
                return loopCounter;
            })(localBuffer[THREAD_ID])]=localBuffer[THREAD_ID];

            operatorsMemory[(uint8_t*)localBuffer[THREAD_ID]]=localBuffer[THREAD_EXECUTABLE];

            

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
    utils::highLevelMemory operatorObjectMemory(BUFFER_SIZE_1);
    operatorObjectMemory[OPERATOR]=operatorObject;
    instruction(operatorObjectMemory[OPERATOR]);
    return;
}

void runThreads(void){
    static uint64_t loopCounter;
    static uint32_t threadCounter;

    if(!stringCounter(operatorsMemory[threadCounter])){
        threadCounter=0;
        return;
    }
    static utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
    
    localBuffer[THREAD_ID]=operatorsMemory[threadCounter++];

    uint8_t *threadPriority=json(THREAD_PRIORITY,operatorsMemory[(char*)localBuffer[THREAD_ID]]);

    localBuffer[OPERATOR]=threadPriority;
    instruction(localBuffer[OPERATOR]);
    
    if(!(loopCounter++%getInt(localBuffer[OPERATOR]))){
        uint8_t* threadExecutable=json(THREAD_EXECUTABLE,operatorsMemory[(char*)localBuffer[THREAD_ID]]);
        localBuffer[OPERATOR]=threadExecutable;
        instruction(localBuffer[OPERATOR]);
    }
    else
        _delay_ms(1);
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
