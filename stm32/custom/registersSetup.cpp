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

#define PRINT_JSON(key, value)         \
            Serial.print("{\"");       \
            Serial.print(key);         \
            Serial.print("\":\"");     \
            Serial.print(value);       \
            Serial.print("\"}\n");


void registersSetup(void){
    appLinker.onWrite(DIGITAL_INPUT,[&](uint8_t* data){
        PM(atoi((char*)data),INPUT);
    });
    appLinker.onWrite(DIGITAL_INPUT_PULLUP,[&](uint8_t* data){
        PM(atoi((char*)data),INPUT_PULLUP);
    });
    appLinker.onWrite(DIGITAL_INPUT_PULLDOWN,[&](uint8_t* data){
        PM(atoi((char*)data),INPUT_PULLDOWN);
    });
    appLinker.onWrite(DIGITAL_OUTPUT,[&](uint8_t* data){
        PM(atoi((char*)data),OUTPUT);
    });
    appLinker.onWrite(DW1,[&](uint8_t* data){
        DW(atoi((char*)data),1);
    });
    appLinker.onWrite(DW0,[&](uint8_t* data){
        DW(atoi((char*)data),0);
    });
    appLinker.onWrite(DR,[&](uint8_t* data){
        PRINT_JSON((char*)data,(int)digitalRead(atoi((char*)data)&~INPUT_REGISTERS_BASE));
    });
    

    appLinker.onWrite(TBOT_MOVE,[&](uint8_t* data){
        // Serial.print("[TBOT] : ");
        // Serial.println((char*)data);

        uint32_t d1=tbot.refPoint.d1;
        uint32_t d2=tbot.refPoint.d2;

        uint8_t *_d1=json("d1",data);
        d1=(_d1!=UNDEFINED)?atoi((char*)_d1):d1;
        
        uint8_t *_d2=json("d2",data);
        d2=(_d2!=UNDEFINED)?atoi((char*)_d2):d2;
        
        tbot.move(d1,d2,atoi((char*)json("feedRate",data)));
    });

    appLinker.onWrite(CLOCK,[&](uint8_t* data){
        uint32_t reg=atoi((char*)json("reg",data));
        uint32_t mask=atoi((char*)json("mask",data));
        uint32_t delay=atoi((char*)json("delay",data));
        uint32_t clks=atoi((char*)json("clks",data));

        uint32_t _reg=reg;

        reg=(reg==PORTA)?(uint32_t)GPIOA_BSRR:reg;
        reg=(reg==PORTB)?(uint32_t)GPIOB_BSRR:reg;
        reg=(reg==PORTC)?(uint32_t)GPIOC_BSRR:reg;

        volatile uint32_t *REG=(volatile uint32_t *)reg;

        while(clks--){
            *REG|=mask;
            HW_DELAY(delay);

            if(_reg==reg)
                *REG&=~mask;
            else
                *REG|=mask<<16;
            HW_DELAY(delay);
        }

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