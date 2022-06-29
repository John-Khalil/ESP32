#include <stdint.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include "SHA1_HASH.h"
#include "EEPROM.h"
#include "spiConsoleSync.cpp"


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
