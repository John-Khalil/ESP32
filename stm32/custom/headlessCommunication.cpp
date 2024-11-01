#pragma once

#include "pointerTool.h"
#include "smartMemory.cpp"

#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

#include <ArduinoJson.h>


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
    Serial.print("\"}\n");

    isBusy=0;
    return 1;
}


template <typename T1, typename T2>
void headlessAttribute(T1 key,T2 value){
    while(!_headlessAttribute(key,value));
    return;
}

// intended to run as a task
void headlessEndpoint(void){
    while(1){
        if(Serial.available()){
            StaticJsonDocument<1024> doc;
            String receivedObject=Serial.readStringUntil('\n');
            Serial.println(receivedObject);
            if(!(DeserializationError)deserializeJson(doc, receivedObject)){
                JsonObject obj = doc.as<JsonObject>();
                for(JsonPair pair : obj){
                    const char* key = pair.key().c_str();
                    String value;
                    if(pair.value().is<JsonObject>())
                        serializeJson(pair.value(),value);
                    else
                        value = pair.value().as<String>();
                    
                    Serial.print("Key: ");
                    Serial.print(key);
                    Serial.print(" Value: ");
                    Serial.println(value);
                }
            }
        }
    }
    return;
}