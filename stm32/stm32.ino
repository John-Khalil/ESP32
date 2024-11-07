#include "custom/platformUtils.cpp"
#include "custom/pointerTool.h"
#include "custom/smartMemory.cpp"
#include "custom/tBot.cpp"
#include "custom/taskLauncher.cpp"
#include "custom/headlessCommunication.cpp"
#include "custom/registersSetup.cpp"
#include "custom/startup.cpp"



// #include <iostream>
#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

void setup(void){
    Serial.begin(921600);
    setupTimer();
    tbotSetup();
    registersSetup();

    headlessEndpoint();
    // taskLauncher();
    return;
}

void loop(){

}