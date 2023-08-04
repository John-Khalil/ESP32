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
#include <map>

#define FILE_PATH "/localStorage.txt"
#define LOCAL_STORAGE_BUFFER 5000

class localStorage{

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

    static union{
        uint8_t *mapFootprint;
        std::map<std::string,std::string>localStorageTable;
    }localStorageMap;

    static void set(std::string key,std::string value){
        localStorage::localStorageInit();
        auto localStorageFile=fileSystem.open(FILE_PATH,"w");
        uint8_t localStorageBuffer[LOCAL_STORAGE_BUFFER]={};
        localStorage::localStorageMap.mapFootprint=(!localStorageFile)?localStorage::localStorageMap.mapFootprint:([&](void){
            for(auto &localStorageElement:localStorageBuffer){
                if(!localStorageFile.available())
                    break;
                localStorageElement=localStorageFile.read();
            }
            return localStorageBuffer;
        })();

        localStorage::localStorageMap.localStorageTable[key]=value;

        size_t localStorageTableSize=localStorage::getMapMemoryFootprint(localStorage::localStorageMap.localStorageTable);
        uint32_t footPrintCounter=0;
        while(localStorageTableSize--){
            localStorageFile.write(localStorage::localStorageMap.mapFootprint[footPrintCounter++]);
        }
        
        localStorageFile.close();
        return;
    }

};