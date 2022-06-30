#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include "SHA1_HASH.h"
#include "EEPROM.h"
#include "spiConsoleSync.cpp"


volatile uint32_t *_outputRegisterLow=((volatile uint32_t*)0X3FF44004UL);
volatile uint32_t *_outputRegisterHigh=((volatile uint32_t*)0X3FF44010UL);

volatile uint32_t *_outputRegisterLowSet=((volatile uint32_t*)0X3FF44008UL);
volatile uint32_t *_outputRegisterHighSet=((volatile uint32_t*)0X3FF44014UL);

volatile uint32_t *_outputRegisterLowClear=((volatile uint32_t*)0X3FF4400CUL);
volatile uint32_t *_outputRegisterHighClear=((volatile uint32_t*)0X3FF44018UL);


#define outputRegisterLow *_outputRegisterLow
#define outputRegisterHigh *_outputRegisterHigh
#define outputRegisterLowSet *_outputRegisterLowSet
#define outputRegisterHighSet *_outputRegisterHighSet
#define outputRegisterLowClear *_outputRegisterLowClear
#define outputRegisterHighClear *_outputRegisterHighClear

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
    Serial.begin(9600);
    while(1){
        Sha1.init();
        Sha1.print("abc");
        Serial.print("\n\n\n\n\n\n\nresult >> ");
        Serial.println((char*)Sha1.result());
        Serial.println("\n\n----------------------------\n");
        // Serial.println("test");
        while(1);
    }

}


void loop(){

}
