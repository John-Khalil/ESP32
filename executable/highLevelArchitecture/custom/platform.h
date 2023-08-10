#include <Arduino.h>
#include <functional>
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef ESP32
    #include "esp_task_wdt.h"

    #include <LITTLEFS.h>
    #include <SPIFFS.h>
    
    #define fileSystem LITTLEFS
    // #define fileSystem SPIFFS
    
    volatile uint32_t *_outputRegisterLow=((volatile uint32_t*)0X3FF44004UL);
    volatile uint32_t *_outputRegisterHigh=((volatile uint32_t*)0X3FF44010UL);

    volatile uint32_t *_outputRegisterLowSet=((volatile uint32_t*)0X3FF44008UL);
    volatile uint32_t *_outputRegisterHighSet=((volatile uint32_t*)0X3FF44014UL);

    volatile uint32_t *_outputRegisterLowClear=((volatile uint32_t*)0X3FF4400CUL);
    volatile uint32_t *_outputRegisterHighClear=((volatile uint32_t*)0X3FF44018UL);




    #define outputRegisterLow (*_outputRegisterLow)
    #define outputRegisterHigh (*_outputRegisterHigh)

    #define outputRegisterLowSet (*_outputRegisterLowSet)
    #define outputRegisterHighSet (*_outputRegisterHighSet)

    #define outputRegisterLowClear (*_outputRegisterLowClear)
    #define outputRegisterHighClear (*_outputRegisterHighClear)


    #define inputRegisterLow (*((volatile uint32_t*)0x3FF4403CUL))
    #define inputRegisterHigh (*((volatile uint32_t*)0x3FF44040UL))



    volatile uint32_t *_outputEnableRegisterLow=((volatile uint32_t*)0X3FF44020UL);
    volatile uint32_t *_outputEnableRegisterHigh=((volatile uint32_t*)0X3FF44024UL);

    volatile uint32_t *_outputEnableRegisterLowSet=((volatile uint32_t*)0X3FF44028UL);
    volatile uint32_t *_outputEnableRegisterHighSet=((volatile uint32_t*)0X3FF4402CUL);

    volatile uint32_t *_outputEnableRegisterLowClear=((volatile uint32_t*)0X3FF44030UL);
    volatile uint32_t *_outputEnableRegisterHighClear=((volatile uint32_t*)0X3FF44034UL);




    #define outputEnableRegisterLow (*_outputEnableRegisterLow)
    #define outputEnableRegisterHigh (*_outputEnableRegisterHigh)

    #define outputEnableRegisterLowSet (*_outputEnableRegisterLowSet)
    #define outputEnableRegisterHighSet (*_outputEnableRegisterHighSet)

    #define outputEnableRegisterLowClear (*_outputEnableRegisterLowClear)
    #define outputEnableRegisterHighClear (*_outputEnableRegisterHighClear)

    
    unsigned long _CURRENT_TIME_;
    #define microSec(_DELAY_IN_US_) _CURRENT_TIME_=micros()+_DELAY_IN_US_;  while(micros()<_CURRENT_TIME_);


    unsigned long _CALIBRATED_DELAY_=0;
    float _DELAY_CALIBRATING_FACTOR_=1.0;
    #define _US(_US_DELAY_) _CALIBRATED_DELAY_=_US_DELAY_*_DELAY_CALIBRATING_FACTOR_; while(_CALIBRATED_DELAY_--)NOP();

    void delayAutoCalibrate(void){
        unsigned long lastKnownTime=micros();
        _US(1000000);
        _DELAY_CALIBRATING_FACTOR_=1000000.0f/(float)(micros()-lastKnownTime);
    }



    #define _delay_us _US
    // #define RTosDelay_us(_TIME_IN_US_) vTaskDelay(_TIME_IN_US_ / ( ( TickType_t ) 1 / configTICK_RATE_HZ )) //this doess not really work
    #define _delay_ms(_TIME_IN_MS_) vTaskDelay(_TIME_IN_MS_ / portTICK_PERIOD_MS)





    void microSecDelay(unsigned long timeInMicroSec){
        _delay_us(timeInMicroSec);
    }



    #define endTask() vTaskDelete(NULL)

    
    #define async(asyncThread)  xTaskCreate((void (*)(void*))[&](void *arg){asyncThread;vTaskDelete(NULL);},"async-task",30000,NULL,0,NULL);





#endif
#ifdef ESP8266
    // #include "osapi.h"
    #include <osapi.h>

    #include <LittleFS.h>

    #define fileSystem LittleFS 

    #define _delay_ms delay

    #define delayAutoCalibrate()

    void esp8266StartTaskHandler(void){
        static uint8_t firstRun;
        if(!firstRun){
            firstRun=1;
            // system_os_start();
        }
        return;
    }

    typedef void (*func)(void*);


    // #define async(asyncThread)  system_os_task((func)([&](void *arg){asyncThread;esp8266StartTaskHandler();}),0,NULL);
    #define async(asyncThread)  

#endif

#endif
