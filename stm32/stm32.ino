#include "custom/platformUtils.cpp"
#include "custom/pointerTool.h"
#include "custom/smartMemory.cpp"
#include "custom/tBot.cpp"



// #include <iostream>
#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

void setup(void){
    Serial.begin(250000);
    setupTimer();

    uint64_t test[2]={};
    uint32_t loopCounter=0;
    while(1){
        test[(loopCounter++)%2]=GET_TIMER();
        Serial.println((test[0]>test[1])?(test[0]-test[1]):(test[1]-test[0]));
        delay(1000);
    }
    return;
}

void loop(){

}