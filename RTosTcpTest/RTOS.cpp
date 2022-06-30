#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include "SHA1_HASH.h"
#include "EEPROM.h"
#include "spiConsoleSync.cpp"



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









unsigned char *sha1Hash(unsigned char *rawData){
	Sha1.init();
	Sha1.print((char*)rawData);
	return Sha1.result();
}


void setup(){
    // Serial.begin(9600);
    _PM(13,OUTPUT);
    within(-1,{
        outputRegisterLow^=(1<<13);
        delay(250);
       
    });
}


void loop(){

}
