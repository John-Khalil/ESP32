#pragma once

#include "platformUtils.cpp"
#include "pointerTool.h"
#include "smartMemory.cpp"
#include "tBot.cpp"
#include "taskLauncher.cpp"
#include "headlessCommunication.cpp"
#include "registersAddress.h"


// #include <iostream>
#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>



void registersSetup(void){
    appLinker.onWrite(TBOT,[&](uint8_t* data){
        Serial.println((char*)data);
    });
    appLinker.onWrite(TBOT_SETUP,[&](uint8_t* data){
        Serial.println((char*)data);
        tbot;
    });
    return;
}

void registerSet(uint32_t address,char *data){
    // if(appLinker.read(address)!=Memory::undefined)
        appLinker.write(address,(uint8_t*)data);
    return;
}