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



class $json{
    private:
        uint8_t objectBuffer[1024]={};
        uint8_t *mainJsonObject=nullptr;
        uint8_t objectNotFound=1;

       

    public:

        unsigned char *jsonParse(unsigned char *userObject,unsigned char *jsonObject){
            if(EXTERNJSONFUNCTIONCALL(userObject,jsonObject,objectBuffer)){
                if(*objectBuffer==0x20){
                    unsigned short charSpaceCounter=1;
                    while(objectBuffer[charSpaceCounter++]==0x22);
                    charSpaceCounter--;
                    unsigned char *charSpacePointer=objectBuffer;
                    unsigned char *realDataPointer=charSpacePointer+charSpaceCounter;
                    while(*realDataPointer){
                        *charSpacePointer=*realDataPointer;
                        charSpacePointer++;
                        realDataPointer++;
                    }
                    CLR(charSpacePointer);
                }
                if((*objectBuffer)==0x22){
                    unsigned char* makeStrJSON=objectBuffer;
                    unsigned short jsonObjectLength=stringCounter(makeStrJSON)-2;
                    unsigned char *clearString=makeStrJSON+1;
                    while(jsonObjectLength){
                        (*makeStrJSON)=(*clearString);
                        *clearString=0;
                        clearString++;
                        makeStrJSON++;
                        jsonObjectLength--;
                    }
                    *clearString=0;
                }
                objectNotFound=0;
                return objectBuffer;
            }
            objectNotFound=1;
            return UNDEFINED;
        }

        operator uint8_t*(){
            return objectBuffer;
        }

        $json &operator[](uint8_t* jsonKey){
            if(mainJsonObject!=nullptr)
                jsonParse(jsonKey,mainJsonObject);
            return (*this);
        }


        $json &operator[](char* jsonKey){
            if(mainJsonObject!=nullptr)
                jsonParse((uint8_t*)jsonKey,mainJsonObject);
            return (*this);
        }

        $json &operator[](std::string jsonKey){
            if(mainJsonObject!=nullptr)
                jsonParse((uint8_t*)jsonKey.c_str(),mainJsonObject);
            return (*this);
        }

        uint8_t operator!(void){
            return objectNotFound;
        }

        

        $json(uint8_t *jsonObject){
            mainJsonObject=jsonObject;
        }

        $json(char *jsonObject){
            mainJsonObject=(uint8_t *)jsonObject;
        }

        $json(std::string jsonObject){
            mainJsonObject=(uint8_t *)jsonObject.c_str();
        }
};