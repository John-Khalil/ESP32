#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "pointerTool.h"
#include "eepromBasicConfig.h"
#include "platform.h"


#ifdef ESP32
    #include <WiFi.h>
#endif

#ifdef ESP8266
    #include <ESP8266WiFi.h>
#endif



namespace WIFI_UTILS{

    using namespace EEPROM_UTILS;


    unsigned char GLOBAL_IPADDRESS_VARIABLE_NAME[16]="";
    unsigned char * ipAddressString(IPAddress deviceIP){
            unsigned char *ipString=GLOBAL_IPADDRESS_VARIABLE_NAME;
        while(*ipString){
            *ipString=0;
            ipString++;
        }
        ipString=GLOBAL_IPADDRESS_VARIABLE_NAME;
        unsigned char byteCounter=0;
        byteCounterEqual4:
        unsigned char *byteCounterString=inttostring((unsigned long)deviceIP[byteCounter]);
        while(*byteCounterString){
            *ipString=*byteCounterString;
            ipString++;
            byteCounterString++;
        }
        if(byteCounter!=3){
            byteCounter++;
            *ipString=0x2E;
            ipString++;
            goto byteCounterEqual4;
        }
        return GLOBAL_IPADDRESS_VARIABLE_NAME;  
    }

    void accessPoint(void){
        WiFi.softAP("",(char*)userPassword());
        WiFi.softAP((char*)ipAddressString((WiFi.status()==WL_CONNECTED)?WiFi.localIP():WiFi.softAPIP()),(char*)userPassword());
        return;
    }

    unsigned short networkConnect(void){
        unsigned char *ssid=userSSID();
        unsigned char *password=userPassword();
        unsigned short exitStatus;
        WiFi.mode(WIFI_STA);
        WiFi.softAP("",(char*)password);
        WiFi.softAP((char*)ipAddressString(WiFi.softAPIP()),(char*)password);
        if(stringCounter(ssid)){
            WiFi.begin((char*)ssid,(char*)password);
            unsigned short connectTimeout=0x8000;
            while ((exitStatus=WiFi.status()) != WL_CONNECTED){
                //ESP.wdtFeed();
                if(!connectTimeout){
                    // WiFi.disconnect();
                    return exitStatus;
                }
                _delay_ms(1);
                connectTimeout++;
            }
            WiFi.setAutoReconnect(true);
            WiFi.persistent(true);
            WiFi.softAP((char*)ipAddressString(WiFi.localIP()),(char*)password);
        }
        return exitStatus;
    }

}




#endif
