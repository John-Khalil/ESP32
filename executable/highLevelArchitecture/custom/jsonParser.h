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



class jsonParser{
    private:
        uint8_t objectBuffer[300]={};
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

        operator uint8_t (){
            return getInt32_t(objectBuffer);
        }

        operator int8_t (){
            return getInt32_t(objectBuffer);
        }
        
        operator uint16_t (){
            return getInt32_t(objectBuffer);
        }

        operator int16_t (){
            return getInt32_t(objectBuffer);
        }

        operator uint32_t (){
            return getInt32_t(objectBuffer);
        }

        operator int32_t (){
            return getInt32_t(objectBuffer);
        }





        jsonParser &operator[](uint8_t* jsonKey){
            if(mainJsonObject!=nullptr)
                jsonParse(jsonKey,mainJsonObject);
            return (*this);
        }


        jsonParser &operator[](char* jsonKey){
            if(mainJsonObject!=nullptr)
                jsonParse((uint8_t*)jsonKey,mainJsonObject);
            return (*this);
        }

        jsonParser &operator[](std::string jsonKey){
            if(mainJsonObject!=nullptr)
                jsonParse((uint8_t*)jsonKey.c_str(),mainJsonObject);
            return (*this);
        }

        uint8_t operator!(void){
            return objectNotFound;
        }

        

        jsonParser(uint8_t *jsonObject){
            mainJsonObject=jsonObject;
        }

        jsonParser(char *jsonObject){
            mainJsonObject=(uint8_t *)jsonObject;
        }

        jsonParser(std::string jsonObject){
            mainJsonObject=(uint8_t *)jsonObject.c_str();
        }
};