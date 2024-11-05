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
    appLinker.onWrite(INPUT,[&](uint8_t* data){
        
    });
    appLinker.onWrite(INPUT_PULLUP,[&](uint8_t* data){
        
    });
    appLinker.onWrite(INPUT_PULLDOWN,[&](uint8_t* data){
        
    });
    appLinker.onWrite(OUTPUT,[&](uint8_t* data){
        
    });
    appLinker.onWrite(DW1,[&](uint8_t* data){
        
    });
    appLinker.onWrite(DW0,[&](uint8_t* data){
        
    });
    appLinker.onWrite(DR,[&](uint8_t* data){
        
    });
    



    appLinker.onWrite(TBOT_MOVE,[&](uint8_t* data){
        Serial.print("[TBOT] : ");
        Serial.println((char*)data);

        tbot.move(
            atoi((char*)json("d1",data)),
            atoi((char*)json("d2",data)),
            atoi((char*)json("feedRate",data))
        );
    });
    appLinker.onWrite(TBOT_SETUP,[&](uint8_t* data){
        //{"266":{"output_set":1073809424,"output_reset":1073809428,"en":0,"dir1":1,"dir2":2,"clk1":3,"clk2":4,"stepsPerUnit":80}}
        Serial.print("[TBOT_SETUP] : ");
        Serial.println((char*)data);

        tbot.setup(
            (uint32_t*)atoi((char*)json("output_set",data)),
            (uint32_t*)atoi((char*)json("output_reset",data)),
            atoi((char*)json("en",data)),
            atoi((char*)json("dir1",data)),
            atoi((char*)json("dir2",data)),
            atoi((char*)json("clk1",data)),
            atoi((char*)json("clk2",data)),
            atoi((char*)json("stepsPerUnit",data))
        );
    });
    return;
}

void registerSet(uint32_t address,char *data){
    // if(appLinker.read(address)!=Memory::undefined)
        appLinker.write(address,(uint8_t*)data);
    return;
}