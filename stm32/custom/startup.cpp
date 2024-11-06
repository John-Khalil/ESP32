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

void tbotSetup(void){
    PM(PA0,OUTPUT);
    PM(PA1,OUTPUT);
    PM(PA2,OUTPUT);
    PM(PA3,OUTPUT);
    PM(PA4,OUTPUT);
    tbot.setup(GPIOA_BSRR,GPIOA_BRR,0,1,2,3,4,80);
}

void startup(void){

    return;
}