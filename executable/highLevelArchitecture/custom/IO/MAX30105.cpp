#pragma once

#include <bits/stdc++.h>

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

#define LOCAL_STARTUP(startupCode) {static std::function<void(void)>localStartup=[&](void){startupCode;localStartup=[](){};};localStartup();}
// #include "../consoleLogger.h"

namespace MAX30105{
    
    uint32_t getHeartRate(void){
        static MAX30105 particleSensor;

        #define RATE_AVG 4

        LOCAL_STARTUP({
            while(!particleSensor.begin(Wire, I2C_SPEED_FAST));

            particleSensor.setup();
            particleSensor.setPulseAmplitudeRed(0x1A);
            particleSensor.setPulseAmplitudeGreen(0);
        });

        uint16_t loopCounter=RATE_AVG;
        uint32_t beatAvg=0;
        uint32_t lastBeat=millis();
        while(loopCounter--){

            for(;;){
                uint32_t irValue = particleSensor.getIR();
                if(irValue<5000){
                    return -1;
                }
                if(checkForBeat(irValue))
                    break;
            };
            beatAvg +=  60 / ((millis() - lastBeat) / 1000.0);
           
            lastBeat = millis();

        }

        return (beatAvg/RATE_AVG);
    }

};

