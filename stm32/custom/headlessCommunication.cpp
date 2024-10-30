#pragma once
#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>


template <typename T1, typename T2>
uint8_t _headlessAttribute(T1 key,T2 value){
    static uint8_t isBusy;
    if(isBusy)
        return 0;
    isBusy=1;

    Serial.print("{\"");
    Serial.print(key);
    Serial.print("\":\"");
    Serial.print(value);
    Serial.print("\"}\r\n");

    isBusy=0;
    return 1;
}


template <typename T1, typename T2>
void headlessAttribute(T1 key,T2 value){
    while(!_headlessAttribute(key,value));
    return;
}