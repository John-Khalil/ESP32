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


const char *GENERIC_RX=      "WEB_SERVER_RX";

const char *MQTT_TX=            "MQTT_TX";
const char *MQTT_RX=            "MQTT_RX";
const char *WEB_SERVER_TX=      "WEB_SERVER_TX";
const char *WEB_SERVER_RX=      "WEB_SERVER_RX";




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
const char *REGISTER_WRITE=     "REGISTER_WRITE";
const char *REGISTER_READ=      "REGISTER_READ";
const char *REGISTER_ADDRESS=   "REGISTER_ADDRESS";
const char *REGISTER_DATA=      "REGISTER_DATA";

const char *LOOP_COUNTER=       "LOOP_COUNTER";
const char *LOOP_ELEMENENTS=    "LOOP_ELEMENENTS";

const char *THREAD_ADD=         "ADD_THREAD";
const char *THREAD_ID=          "THREAD_ID";
const char *THREAD_EXECUTABLE=  "THREAD_EXECUTABLE";
const char *THREAD_PRIORITY=    "THREAD_PRIORITY";

const char *STARTUP_ADD=         "STARTUP_ADD";
const char *STARTUP_ID=          "STARTUP_ID";
const char *STARTUP_SCRIPT=      "STARTUP_SCRIPT";

const char *ALOP=                "ALOP";
const char *FIRST_OPERAND=       "FIRST_OPERAND";
const char *SECOND_OPERAND=      "SECOND_OPERAND";

const char *SET_OBJECT=          "SET_OBJECT";
const char *USER_OBJECT=         "USER_OBJECT";
const char *USER_KEY=            "USER_KEY";
const char *NEW_VALUE=           "NEW_VALUE";


const char *MATH_OPERATORS[]={
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "_2sComp",
    "XOR",
    "OR",
    "AND",
    "COMP",
    "SHIFT_RIGHT",
    "SHIFT_LEFT",
    "EQUAL",
    "NOT_EQUAL",
    "LOGIC_AND",
    "LOGIC_OR",
    "NOT",
    "SMALLER",
    "GREATER",
    "SMALLER_EQUAL",
    "GREATER_EQUAL"
};

const std::function<unsigned long(unsigned long,unsigned long)>MATH_OPERATIONS[]={			
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ ADD 				-ARITHMETIC
        return firstOperand+secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ SUB 				-ARITHMETIC
        return firstOperand-secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ MUL 				-ARITHMETIC
        return firstOperand*secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ DIV 				-ARITHMETIC
        return firstOperand/secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ MOD 				-ARITHMETIC
        return firstOperand%secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ 2's COMP 			-ARITHMETIC
        return (~firstOperand)+1;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ XOR 				-BITWISE
        return firstOperand^secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ OR 					-BITWISE
        return firstOperand|secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ AND 				-BITWISE
        return firstOperand&secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ COMP 				-BITWISE
        return ~firstOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ SHIFT RIGHT 		-BITWISE
        return firstOperand>>secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ SHIFT LEFT 			-BITWISE
        return firstOperand<<secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ EQUAL 				-LOGIC
        return (firstOperand==secondOperand);
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ NOT EQUAL 			-LOGIC
        return (firstOperand!=secondOperand);
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ AND 				-LOGIC
        return firstOperand&&secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ OR 					-LOGIC
        return firstOperand||secondOperand;
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ NOT 				-LOGIC
        return (!firstOperand);
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ SMALLER 			-LOGIC
        return (firstOperand<secondOperand);
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ GREATER 			-LOGIC
        return (firstOperand>secondOperand);
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ SMALLER OR EQUAL	-LOGIC
        return (firstOperand<=secondOperand);
    },
    [&](unsigned long firstOperand,unsigned long secondOperand){	//^ GREATER OR EQUAL	-LOGIC
        return (firstOperand>=secondOperand);
    }
    // all the basic mathematical operations like Power, logarithmic , exponential , root , trignometric should be added further
};





utils::highLevelMemory& instruction(utils::highLevelMemory& operatorObject){

    // console.log("start >> ",(uint8_t*)operatorObject[OPERATOR]);
    uint8_t *userOperator;
    if((userOperator=json(OPERATOR,operatorObject[OPERATOR]))==UNDEFINED)
        return operatorObject[OPERATOR];

    static uint16_t nestedCounter;

    utils::highLevelMemory operatorObjectMemory(BUFFER_SIZE_1);

    ([&](utils::highLevelMemory &operatorsMemoryCallbacks){

        #define SET_OPERATOR(USER_OPERATOR) if(equalStrings((uint8_t*)USER_OPERATOR,userOperator))operatorsMemoryCallbacks[USER_OPERATOR]

        SET_OPERATOR("readTest")<<[&](void){
            operatorsMemoryCallbacks["readTest"]="readTest !!!";
            return;
        };

        SET_OPERATOR(CONSOLE_LOGGER)>>[&](uint8_t *operatorData){                           //^ CONSOLE_LOGGER
            static uint16_t logCounter;

            console.log("[",++logCounter,"]-> ",operatorData);

            return;
        };

        SET_OPERATOR(LOOP)>>[&](uint8_t *operatorData){                                     //^ LOOP
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            // console.log("LOOP --->> ",operatorData);

            localBuffer[OPERATOR]=json(LOOP_COUNTER,operatorData);
            uint32_t loopLimit=getInt32_t(instruction(localBuffer[OPERATOR]));
            while(loopLimit--){
                uint16_t loopCounter=0;
                while((localBuffer[OPERATOR]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED)
                    instruction(localBuffer[OPERATOR]);
            }
            return;

        };

        SET_OPERATOR(MEMORY_WRITE)<<[&](void){                                              //^ MEMORY_WRITE
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

        SET_OPERATOR(MEMORY_READ)<<[&](void){                                               //^ MEMORY_READ
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
            localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorsMemoryCallbacks[MEMORY_READ]);
            instruction(localBuffer[OPERATOR]);

            operatorsMemoryCallbacks[MEMORY_READ]=operatorsMemory[(uint8_t*)localBuffer[OPERATOR]];
            return;
        };

        SET_OPERATOR(STORAGE_WRITE)<<[&](void){                                             //^ STORAGE_WRITE
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

        SET_OPERATOR(STORAGE_READ)<<[&](void){                                              //^ STORAGE_READ
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
            localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorsMemoryCallbacks[STORAGE_READ]);
            instruction(localBuffer[OPERATOR]);

            operatorsMemoryCallbacks[STORAGE_READ]=localStorage.getItem((char*)localBuffer[OPERATOR]).c_str();
            return;
        };

        SET_OPERATOR(THREAD_ADD)>>[&](uint8_t *threadData){                                 //^ THREAD_ADD
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

        SET_OPERATOR(STARTUP_ADD)>>[&](uint8_t *startupData){                               //^ STARTUP_ADD
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            localBuffer[OPERATOR]=json(STARTUP_ID,startupData);
            instruction(localBuffer[OPERATOR]);
            localBuffer[STARTUP_ID]=localBuffer[OPERATOR];

            localBuffer[STARTUP_SCRIPT]=json(STARTUP_SCRIPT,startupData);

            //! localBuffer[OPERATOR]=json(STARTUP_SCRIPT,startupData);
            //! instruction(localBuffer[OPERATOR]);
            //! localBuffer[STARTUP_SCRIPT]=localBuffer[OPERATOR];

            localStorage.setItem(([&](uint8_t* scriptID){
                uint32_t loopCounter=0;
                while((!equalStrings((uint8_t*)localStorage.getItem(loopCounter).c_str(),UNDEFINED))&&(!equalStrings((uint8_t*)localStorage.getItem(loopCounter).c_str(),scriptID)))
                    loopCounter++;
                return loopCounter;
            })(localBuffer[STARTUP_ID]),localBuffer[STARTUP_ID]);

            localStorage.setItem((char*)localBuffer[STARTUP_ID],(char*)localBuffer[STARTUP_SCRIPT]);

            return;
        };
        
        SET_OPERATOR(ALOP)<<[&](void){                                                      //^ Arithmetic Logic operators
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            localBuffer[OPERATOR]=json(ALOP,operatorsMemoryCallbacks[ALOP]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[ALOP]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(FIRST_OPERAND,operatorsMemoryCallbacks[ALOP]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[FIRST_OPERAND]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(SECOND_OPERAND,operatorsMemoryCallbacks[ALOP]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[SECOND_OPERAND]=localBuffer[OPERATOR];

            operatorsMemoryCallbacks[ALOP]=inttostring(MATH_OPERATIONS[([&](void){
                uint16_t loopCounter=0;
                while(!equalStrings(localBuffer[ALOP],(uint8_t*)MATH_OPERATORS[loopCounter++]));
                return loopCounter-1;
            })()](strint(localBuffer[FIRST_OPERAND]),strint(localBuffer[SECOND_OPERAND])));
            
            return;
        };

        SET_OPERATOR(REGISTER_WRITE)<<[&](void){                                            //^ REGISTER_WRITE
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

            localBuffer[OPERATOR]=json(REGISTER_ADDRESS,operatorsMemoryCallbacks[REGISTER_WRITE]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[REGISTER_ADDRESS]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(REGISTER_DATA,operatorsMemoryCallbacks[REGISTER_WRITE]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[REGISTER_DATA]=localBuffer[OPERATOR];


            appLinker[(uint8_t*)localBuffer[REGISTER_ADDRESS]]=localBuffer[REGISTER_DATA];
            operatorsMemoryCallbacks[REGISTER_WRITE]=appLinker[(uint8_t*)localBuffer[REGISTER_ADDRESS]];
            return;
        };

        SET_OPERATOR(REGISTER_READ)<<[&](void){                                             //^ REGISTER_READ
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
            localBuffer[OPERATOR]=json(REGISTER_ADDRESS,operatorsMemoryCallbacks[REGISTER_READ]);
            instruction(localBuffer[OPERATOR]);

            operatorsMemoryCallbacks[REGISTER_READ]=appLinker[(uint8_t*)localBuffer[OPERATOR]];
            return;
        };

        SET_OPERATOR(SET_OBJECT)<<[&](void){                                                //^ SET_OBJECT
            utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
            localBuffer[OPERATOR]=(json(USER_OBJECT,operatorsMemoryCallbacks[SET_OBJECT])==UNDEFINED)?(uint8_t*)"{}":json(USER_OBJECT,operatorsMemoryCallbacks[SET_OBJECT]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[USER_OBJECT]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(USER_KEY,operatorsMemoryCallbacks[SET_OBJECT]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[USER_KEY]=localBuffer[OPERATOR];

            localBuffer[OPERATOR]=json(NEW_VALUE,operatorsMemoryCallbacks[SET_OBJECT]);
            instruction(localBuffer[OPERATOR]);
            localBuffer[NEW_VALUE]=localBuffer[OPERATOR];
            
            operatorsMemoryCallbacks[SET_OBJECT]=editJson((char*)localBuffer[USER_OBJECT],(char*)localBuffer[USER_KEY],(char*)localBuffer[NEW_VALUE]).c_str();
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

    --nestedCounter;

    return operatorObject[OPERATOR];
}







// utils::highLevelMemory& instruction(utils::highLevelMemory& operatorObject){

//     // console.log("start >> ",(uint8_t*)operatorObject[OPERATOR]);

//     if(json(OPERATOR,operatorObject[OPERATOR])==UNDEFINED)
//         return operatorObject[OPERATOR];

//     static uint16_t nestedCounter;


//     // #define NESTING_LIMIT 5
//     // static uint8_t staticBuffer[NESTING_LIMIT][BUFFER_SIZE_1*2]={{}};
//     // uint8_t* allocatedMemoryPointer=staticBuffer[nestedCounter++];
//     // utils::highLevelMemory operatorObjectMemory(CLR(allocatedMemoryPointer,BUFFER_SIZE_1),BUFFER_SIZE_1);
//     // utils::highLevelMemory operatorLocalBuffer(CLR(allocatedMemoryPointer+BUFFER_SIZE_1,BUFFER_SIZE_1),BUFFER_SIZE_1);


//     static std::vector<utils::highLevelMemory> operatorObjectMemoryVector;
//     // static std::vector<utils::highLevelMemory> operatorLocalBufferVector;

//     if(nestedCounter>=operatorObjectMemoryVector.size()){

//         console.log("see if this ever triggers");

//         // operatorObjectMemoryVector.emplace_back(BUFFER_SIZE_1);
//         // operatorLocalBufferVector.emplace_back(BUFFER_SIZE_1);

//         operatorObjectMemoryVector.emplace_back((uint8_t*)calloc(BUFFER_SIZE_1+1,sizeof(uint8_t)),BUFFER_SIZE_1);
        
//         ([&](utils::highLevelMemory &operatorsMemoryCallbacks){

//     /*


//             static auto CONSOLE_LOGGER_FUNCTION=[&](uint8_t *operatorData){                           //^ CONSOLE_LOGGER
//                 static uint16_t logCounter;

//                 console.log("[",++logCounter,"]-> ",operatorData);

//                 return;
//             };

//             operatorsMemoryCallbacks[CONSOLE_LOGGER]>>CONSOLE_LOGGER_FUNCTION;

            
//             static auto LOOP_FUNCTION=[&](uint8_t *operatorData){                                     //^ LOOP
//                 // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//                 // console.log("LOOP --->> ",operatorData);

//                 localBuffer[OPERATOR]=json(LOOP_COUNTER,operatorData);
//                 uint32_t loopLimit=getInt32_t(instruction(localBuffer[OPERATOR]));
//                 while(loopLimit--){
//                     uint16_t loopCounter=0;
//                     while((localBuffer[OPERATOR]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED)
//                         instruction(localBuffer[OPERATOR]);
//                 }

//                 return;

//             };

//             operatorsMemoryCallbacks[LOOP]>>LOOP_FUNCTION;


//             static auto THREAD_ADD_FUNCTION=[&](uint8_t *threadData){                                 //^ THREAD_ADD
//                 // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//                 localBuffer[OPERATOR]=json(THREAD_ID,threadData);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[THREAD_ID]=localBuffer[OPERATOR];

//                 localBuffer[OPERATOR]=json(THREAD_EXECUTABLE,threadData);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[THREAD_EXECUTABLE]=localBuffer[OPERATOR];

//                 operatorsMemory[([&](uint8_t *threadID){
//                     uint32_t loopCounter=0;
//                     while(stringCounter(operatorsMemory[loopCounter])&&!equalStrings(operatorsMemory[loopCounter],threadID))
//                         loopCounter++;
//                     return loopCounter;
//                 })(localBuffer[THREAD_ID])]=localBuffer[THREAD_ID];

//                 operatorsMemory[(uint8_t*)localBuffer[THREAD_ID]]=localBuffer[THREAD_EXECUTABLE];

                

//                 return;
//             };

//             operatorsMemoryCallbacks[THREAD_ADD]>>THREAD_ADD_FUNCTION;


//     */

//             operatorsMemoryCallbacks["readTest"]<<[&](void){
//                 operatorsMemoryCallbacks["readTest"]="readTest !!!";
//                 return;
//             };

//             operatorsMemoryCallbacks[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                           //^ CONSOLE_LOGGER
//                 static uint16_t logCounter;

//                 console.log("[",++logCounter,"]-> ",operatorData);

//                 return;
//             };

//             operatorsMemoryCallbacks[LOOP]>>[&](uint8_t *operatorData){                                     //^ LOOP
//                 utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//                 // console.log("LOOP --->> ",operatorData);

//                 localBuffer[OPERATOR]=json(LOOP_COUNTER,operatorData);
//                 uint32_t loopLimit=getInt32_t(instruction(localBuffer[OPERATOR]));
//                 while(loopLimit--){
//                     uint16_t loopCounter=0;
//                     while((localBuffer[OPERATOR]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED){
//                         // console.log(" -------------> ",json($(LOOP_ELEMENENTS,"[",loopCounter+1,"]"),operatorData));
//                         instruction(localBuffer[OPERATOR]);
//                         // console.log(" =============> ",operatorData);
//                     }
//                 }

//                 // console.log("LOOP --->> ",operatorData);

//                 return;

//             };

//             operatorsMemoryCallbacks[MEMORY_WRITE]<<[&](void){                                              //^ MEMORY_WRITE
//                 utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//                 localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorsMemoryCallbacks[MEMORY_WRITE]);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[MEMORY_ADDRESS]=localBuffer[OPERATOR];

//                 localBuffer[OPERATOR]=json(MEMORY_DATA,operatorsMemoryCallbacks[MEMORY_WRITE]);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[MEMORY_DATA]=localBuffer[OPERATOR];


//                 operatorsMemory[(uint8_t*)localBuffer[MEMORY_ADDRESS]]=localBuffer[MEMORY_DATA];
//                 operatorsMemoryCallbacks[MEMORY_WRITE]=operatorsMemory[(uint8_t*)localBuffer[MEMORY_ADDRESS]];
//                 return;
//             };

//             operatorsMemoryCallbacks[MEMORY_READ]<<[&](void){                                               //^ MEMORY_READ
//                 utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
                
//                 localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorsMemoryCallbacks[MEMORY_READ]);
//                 instruction(localBuffer[OPERATOR]);

//                 operatorsMemoryCallbacks[MEMORY_READ]=operatorsMemory[(uint8_t*)localBuffer[OPERATOR]];
//                 return;
//             };

//             operatorsMemoryCallbacks[STORAGE_WRITE]<<[&](void){                                              //^ STORAGE_WRITE
//                 utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//                 localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorsMemoryCallbacks[STORAGE_WRITE]);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[STORAGE_ADDRESS]=localBuffer[OPERATOR];

//                 localBuffer[OPERATOR]=json(STORAGE_DATA,operatorsMemoryCallbacks[STORAGE_WRITE]);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[STORAGE_DATA]=localBuffer[OPERATOR];

//                 operatorsMemoryCallbacks[STORAGE_WRITE]=localStorage.setItem((char*)localBuffer[STORAGE_ADDRESS],localBuffer[STORAGE_DATA]).c_str();
//                 return;
//             };

//             operatorsMemoryCallbacks[STORAGE_READ]<<[&](void){                                               //^ STORAGE_READ
//                 utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
                
//                 localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorsMemoryCallbacks[STORAGE_READ]);
//                 instruction(localBuffer[OPERATOR]);

//                 operatorsMemoryCallbacks[STORAGE_READ]=localStorage.getItem((char*)localBuffer[OPERATOR]).c_str();
//                 return;
//             };

//             operatorsMemoryCallbacks[THREAD_ADD]>>[&](uint8_t *threadData){                                 //^ THREAD_ADD
//                 utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//                 localBuffer[OPERATOR]=json(THREAD_ID,threadData);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[THREAD_ID]=localBuffer[OPERATOR];

//                 localBuffer[OPERATOR]=json(THREAD_EXECUTABLE,threadData);
//                 instruction(localBuffer[OPERATOR]);
//                 localBuffer[THREAD_EXECUTABLE]=localBuffer[OPERATOR];

//                 operatorsMemory[([&](uint8_t *threadID){
//                     uint32_t loopCounter=0;
//                     while(stringCounter(operatorsMemory[loopCounter])&&!equalStrings(operatorsMemory[loopCounter],threadID))
//                         loopCounter++;
//                     return loopCounter;
//                 })(localBuffer[THREAD_ID])]=localBuffer[THREAD_ID];

//                 operatorsMemory[(uint8_t*)localBuffer[THREAD_ID]]=localBuffer[THREAD_EXECUTABLE];

//                 console.log("thread load return");

//                 return;
//             };



//             return;
//         })(operatorObjectMemoryVector[operatorObjectMemoryVector.size()-1]);
//     }


//     // utils::highLevelMemory operatorObjectMemory(BUFFER_SIZE_1);
//     // utils::highLevelMemory operatorLocalBuffer(BUFFER_SIZE_1);

//     // console.log(inttostring(operatorObjectMemoryVector.size())," - - ",nestedCounter);

//     // operatorObjectMemoryVector[operatorObjectMemoryVector.size()-1][CONSOLE_LOGGER]=inttostring(operatorObjectMemoryVector.size());
    
//     utils::highLevelMemory &operatorObjectMemory=operatorObjectMemoryVector[nestedCounter++];

//     // operatorObjectMemory[CONSOLE_LOGGER]=inttostring(operatorObjectMemoryVector.size());

//     /*

//     // ([&](utils::highLevelMemory &operatorsMemoryCallbacks){

//         // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
//         // operatorObjectMemory["readTest"]<<[&](void){
//         //     operatorObjectMemory["readTest"]="readTest !!!";
//         //     return;
//         // };

//         // operatorObjectMemory[CONSOLE_LOGGER]>>[&](uint8_t *operatorData){                           //^ CONSOLE_LOGGER
//         //     static uint16_t logCounter;

//         //     console.log("[",++logCounter,"]-> ",operatorData);

//         //     return;
//         // };

//         // operatorObjectMemory[LOOP]>>[&](uint8_t *operatorData){                                     //^ LOOP
//         //     // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//         //     // console.log("LOOP --->> ",operatorData);

//         //     localBuffer[OPERATOR]=json(LOOP_COUNTER,operatorData);
//         //     uint32_t loopLimit=getInt32_t(instruction(localBuffer[OPERATOR]));
//         //     while(loopLimit--){
//         //         uint16_t loopCounter=0;
//         //         while((localBuffer[OPERATOR]=json($(LOOP_ELEMENENTS,"[",loopCounter++,"]"),operatorData))!=UNDEFINED)
//         //             instruction(localBuffer[OPERATOR]);
//         //     }

//         //     return;

//         // };

//         // operatorObjectMemory[MEMORY_WRITE]<<[&](void){                                              //^ MEMORY_WRITE
//         //     // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//         //     localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorObjectMemory[MEMORY_WRITE]);
//         //     instruction(localBuffer[OPERATOR]);
//         //     localBuffer[MEMORY_ADDRESS]=localBuffer[OPERATOR];

//         //     localBuffer[OPERATOR]=json(MEMORY_DATA,operatorObjectMemory[MEMORY_WRITE]);
//         //     instruction(localBuffer[OPERATOR]);
//         //     localBuffer[MEMORY_DATA]=localBuffer[OPERATOR];


//         //     operatorsMemory[(uint8_t*)localBuffer[MEMORY_ADDRESS]]=localBuffer[MEMORY_DATA];
//         //     operatorObjectMemory[MEMORY_WRITE]=operatorsMemory[(uint8_t*)localBuffer[MEMORY_ADDRESS]];
//         //     return;
//         // };

//         // operatorObjectMemory[MEMORY_READ]<<[&](void){                                               //^ MEMORY_READ
//         //     // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
//         //     localBuffer[OPERATOR]=json(MEMORY_ADDRESS,operatorObjectMemory[MEMORY_READ]);
//         //     instruction(localBuffer[OPERATOR]);

//         //     operatorObjectMemory[MEMORY_READ]=operatorsMemory[(uint8_t*)localBuffer[OPERATOR]];
//         //     return;
//         // };

//         // operatorObjectMemory[STORAGE_WRITE]<<[&](void){                                              //^ STORAGE_WRITE
//         //     // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//         //     localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorObjectMemory[STORAGE_WRITE]);
//         //     instruction(localBuffer[OPERATOR]);
//         //     localBuffer[STORAGE_ADDRESS]=localBuffer[OPERATOR];

//         //     localBuffer[OPERATOR]=json(STORAGE_DATA,operatorObjectMemory[STORAGE_WRITE]);
//         //     instruction(localBuffer[OPERATOR]);
//         //     localBuffer[STORAGE_DATA]=localBuffer[OPERATOR];

//         //     operatorObjectMemory[STORAGE_WRITE]=localStorage.setItem((char*)localBuffer[STORAGE_ADDRESS],localBuffer[STORAGE_DATA]).c_str();
//         //     return;
//         // };

//         // operatorObjectMemory[STORAGE_READ]<<[&](void){                                               //^ STORAGE_READ
//         //     // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
            
//         //     localBuffer[OPERATOR]=json(STORAGE_ADDRESS,operatorObjectMemory[STORAGE_READ]);
//         //     instruction(localBuffer[OPERATOR]);

//         //     operatorObjectMemory[STORAGE_READ]=localStorage.getItem((char*)localBuffer[OPERATOR]).c_str();
//         //     return;
//         // };

//         // operatorObjectMemory[THREAD_ADD]>>[&](uint8_t *threadData){                                 //^ THREAD_ADD
//         //     // utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

//         //     localBuffer[OPERATOR]=json(THREAD_ID,threadData);
//         //     instruction(localBuffer[OPERATOR]);
//         //     localBuffer[THREAD_ID]=localBuffer[OPERATOR];

//         //     localBuffer[OPERATOR]=json(THREAD_EXECUTABLE,threadData);
//         //     instruction(localBuffer[OPERATOR]);
//         //     localBuffer[THREAD_EXECUTABLE]=localBuffer[OPERATOR];

//         //     operatorsMemory[([&](uint8_t *threadID){
//         //         uint32_t loopCounter=0;
//         //         while(stringCounter(operatorsMemory[loopCounter])&&!equalStrings(operatorsMemory[loopCounter],threadID))
//         //             loopCounter++;
//         //         return loopCounter;
//         //     })(localBuffer[THREAD_ID])]=localBuffer[THREAD_ID];

//         //     operatorsMemory[(uint8_t*)localBuffer[THREAD_ID]]=localBuffer[THREAD_EXECUTABLE];

            

//         //     return;
//         // };



//     //     return;
//     // })(operatorObjectMemory);
    
//     */
    

//     operatorObjectMemory[OPERATOR]=json(DATA,operatorObject[OPERATOR]);

//     instruction(operatorObjectMemory[OPERATOR]);

//         console.log("nestedCounter => ",nestedCounter);

//     // uint8_t* operatorData=operatorObjectMemory[OPERATOR];

//     operatorObjectMemory[json(OPERATOR,operatorObject[OPERATOR])]=operatorObjectMemory[OPERATOR];

//     // uint8_t* returnData=operatorObjectMemory[json(OPERATOR,operatorObject)];

//     operatorObject[OPERATOR]=operatorObjectMemory[json(OPERATOR,operatorObject[OPERATOR])];

//     nestedCounter--;

//     console.log("nestedCounter >> ",nestedCounter);

//     return operatorObject[OPERATOR];
// }


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
        loopCounter++;
        return;
    }
    static utils::highLevelMemory localBuffer(BUFFER_SIZE_1);
    
    localBuffer[THREAD_ID]=operatorsMemory[threadCounter++];

    uint8_t *threadPriority=json(THREAD_PRIORITY,operatorsMemory[(char*)localBuffer[THREAD_ID]]);

    localBuffer[OPERATOR]=threadPriority;
    instruction(localBuffer[OPERATOR]);
    
    if(!(loopCounter%getInt(localBuffer[OPERATOR]))){
        uint8_t* threadExecutable=json(THREAD_EXECUTABLE,operatorsMemory[(char*)localBuffer[THREAD_ID]]);
        localBuffer[OPERATOR]=threadExecutable;
        instruction(localBuffer[OPERATOR]);
    }
    else
        _delay_ms(1);
    return;
}

void loadStartupScripts(void){
    uint32_t scriptCounter=0;
    utils::highLevelMemory localBuffer(BUFFER_SIZE_1);

    // console.log("----> ",localStorage.getItem("script1").c_str());

    while(!equalStrings((localBuffer[STARTUP_ID]=(uint8_t*)localStorage.getItem(scriptCounter++).c_str()),UNDEFINED)){
        console.log(" [STARTUP_SCRIPT] ",(char*)localBuffer[STARTUP_ID]);
        uint8_t *startupScript=(uint8_t*)localStorage.getItem((char*)localBuffer[STARTUP_ID]).c_str();
        localBuffer[OPERATOR]=startupScript;
        instruction(localBuffer[OPERATOR]);
    }

    return;
}

















