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
        std::size_t size = sizeof(myMap); // Size of the map object itself

        // Add the size of the elements (keys and values) stored in the map
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
        localStorage::localStorageMap.localStorageTable[key]=value;
        
    }

};