#pragma once


#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

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
#include <map>

#define FILE_PATH "/localStorage.txt"
#define LOCAL_STORAGE_BUFFER 5000



class LOCAL_STORAGE{
    public:

        static void localStorageInit(void){
            static uint8_t firstRun;
            if(!firstRun){
                firstRun=1;
                fileSystem.begin();
            }
            return;
        }

        template <typename Key, typename Value>
        static std::size_t getMapMemoryFootprint(const std::map<Key, Value>& myMap) {
            std::size_t size = sizeof(myMap);
            for (const auto& pair : myMap) {
                size += sizeof(pair.first) + sizeof(pair.second);
            }
            return size;
        }

        // union localStorageMap_t{
        //     uint8_t *mapFootprint;
        //     std::map<std::string,std::string>localStorageTable;
        // };

        // LOCAL_STORAGE::localStorageMap_t localStorageMap;

        
        // union{
        //     uint8_t *mapFootprint;
        //     std::map<std::string,std::string>localStorageTable;
        //     (){

        //     }
        // }localStorageMap;

        // void set(std::string key,std::string value){

        //     LOCAL_STORAGE::localStorageInit();
        //     auto localStorageFile=fileSystem.open(FILE_PATH,"w");
        //     uint8_t localStorageBuffer[LOCAL_STORAGE_BUFFER]={};

        //     localStorageMap.mapFootprint=(!localStorageFile)?localStorageMap.mapFootprint:([&](void){
        //         for(auto &localStorageElement:localStorageBuffer){
        //             if(!localStorageFile.available())
        //                 break;
        //             localStorageElement=localStorageFile.read();
        //         }
        //         return localStorageBuffer;
        //     })();

        //     localStorageMap.localStorageTable[key]=value;

        //     size_t localStorageTableSize=getMapMemoryFootprint(localStorageMap.localStorageTable);
        //     uint32_t footPrintCounter=0;
        //     while(localStorageTableSize--){
        //         localStorageFile.write(localStorageMap.mapFootprint[footPrintCounter++]);
        //     }
            
        //     localStorageFile.close();
        //     return;
        // }

        // std::string get(std::string key){

        //     localStorageInit();
        //     auto localStorageFile=fileSystem.open(FILE_PATH,"w");
        //     uint8_t localStorageBuffer[LOCAL_STORAGE_BUFFER]={};
            
        //     // localStorageMap.mapFootprint=(!localStorageFile)?localStorageMap.mapFootprint:([&](void){
        //     //     for(auto &localStorageElement:localStorageBuffer){
        //     //         if(!localStorageFile.available())
        //     //             break;
        //     //         localStorageElement=localStorageFile.read();
        //     //     }
        //     //     return localStorageBuffer;
        //     // })();

        //     return localStorageMap.localStorageTable[key];
        // }



        LOCAL_STORAGE(){

        }
};

extern LOCAL_STORAGE localStorage();

#endif