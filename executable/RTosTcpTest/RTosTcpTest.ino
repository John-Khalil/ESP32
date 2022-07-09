#include <stdint.h>
#include <algorithm>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include "SHA1_HASH.h"
#include "EEPROM.h"
#include "spiConsoleSync.cpp"

#include "consoleLogger.h"

typedef unsigned long loopVar;

loopVar _LOOP_COUNTER_ = 0;
loopVar *_NESTED_LOOPS_= (loopVar*)malloc(1*sizeof(loopVar));
unsigned char _NESTED_LOOPS_COUNTER_ = 0;
#define within(_LOOP_ITERATIONS_,_LOOP_BODY_) _NESTED_LOOPS_[_NESTED_LOOPS_COUNTER_++]=_LOOP_COUNTER_; _NESTED_LOOPS_=(loopVar *)realloc(_NESTED_LOOPS_,(_NESTED_LOOPS_COUNTER_+1) *sizeof(loopVar));_LOOP_COUNTER_=_LOOP_ITERATIONS_;while(_LOOP_COUNTER_--) _LOOP_BODY_; _LOOP_COUNTER_=_NESTED_LOOPS_[--_NESTED_LOOPS_COUNTER_];  _NESTED_LOOPS_=(loopVar *)realloc(_NESTED_LOOPS_, (_NESTED_LOOPS_COUNTER_+1) * sizeof(loopVar));

#define lengthInBytes(object) (unsigned char *)(&object+1)-(unsigned char *)(&object)


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

#define _DW digitalWrite
#define _DR digitalRead
#define _PM pinMode

spiConsole console;




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

#define consoleClkPin 13
#define consoleDataPin 12 

const uint32_t consoleClkPinValue=(1<<consoleClkPin);
const uint32_t consoleDataPinValue=(1<<consoleDataPin);


void microSecDelay(unsigned long timeInMicroSec){
    _delay_us(timeInMicroSec);
}

// void consoleClk(unsigned char pinState){
//     outputRegisterLowSet|=consoleClkPinValue*(pinState!=0);
//     outputRegisterLowClear|=consoleClkPinValue*(!pinState);
// }

// void consoleData(unsigned char pinState){
//     outputRegisterLowSet|=consoleDataPinValue*(pinState!=0);
//     outputRegisterLowClear|=consoleDataPinValue*(!pinState);
// }

// unsigned char consoleSync(void){
//     return 1;
// }

void consoleSetup(void){
    _PM(consoleClkPin,OUTPUT);
    _PM(consoleDataPin,OUTPUT);
    // console.setup(consoleClk,consoleData,consoleSync,microSecDelay,200000);
    console.setup(
        [&](unsigned char pinState){
            outputRegisterLowSet|=consoleClkPinValue*(pinState!=0);
            outputRegisterLowClear|=consoleClkPinValue*(!pinState);
        },
        [&](unsigned char pinState){
            outputRegisterLowSet|=consoleDataPinValue*(pinState!=0);
            outputRegisterLowClear|=consoleDataPinValue*(!pinState);
        },
        [&](void){
            return 1;
        },
        [&](unsigned long timeInMicroSec){
            _delay_us(timeInMicroSec);
        },
        200000
    );
}




unsigned char *sha1Hash(unsigned char *rawData){
	Sha1.init();
	Sha1.print((char*)rawData);
	return Sha1.result();
}


void setup(){
    delayAutoCalibrate();
    consoleSetup();
    // Serial.begin(9600);
    _PM(13,OUTPUT);
    console.log("\n\n----------------------------------------------------------------------------------------------------\n");
    within(20,{
        
        console.log("first time from ESP32 >> ",_LOOP_COUNTER_);
        // _delay_ms(500);
       
    });
}


void loop(){

}
