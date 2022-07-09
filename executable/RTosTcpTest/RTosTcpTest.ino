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




//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    LEGACY-CODE    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


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




#define MAX_EEPROM_SIZE 512U
#define SSID_MAX_LENGTH 32
#define WIFI_PASSWORD_MAX_LENGTH 63
#define WIFI_CONFIG 0

#define EEPROM_BUFFER_SIZE 200
unsigned char EEPROM_BUFFER[EEPROM_BUFFER_SIZE]="";



unsigned char ssidSave(unsigned char *networkSSID){
	unsigned short startAddress=WIFI_CONFIG;
	while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH)){
		EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),*networkSSID);
		startAddress++;
		networkSSID+=(*networkSSID!=0);
	}
	return (unsigned char)EEPROM.commit();
}


unsigned char wifiPasswordSave(unsigned char *networkPassword){
	unsigned short startAddress=WIFI_CONFIG+SSID_MAX_LENGTH;
	while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH)){
		EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),*networkPassword);
		startAddress++;
		networkPassword+=(*networkPassword!=0);
	}
	return (unsigned char)EEPROM.commit();
}



unsigned char NETWORK_SSID[SSID_MAX_LENGTH+1]="";
unsigned char NETWORK_PASSWORD[WIFI_PASSWORD_MAX_LENGTH+1]="";

unsigned char *userSSID(void){
	unsigned char *getSSID=NETWORK_SSID;
	unsigned short startAddress=WIFI_CONFIG;
	while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH)){
		*getSSID=EEPROM.read(startAddress);
		getSSID++;
		startAddress++;
	}
	return NETWORK_SSID;
}


unsigned char *userPassword(void){
	unsigned char *getPassword=NETWORK_PASSWORD;
	unsigned short startAddress=WIFI_CONFIG+SSID_MAX_LENGTH;
	while(startAddress<(WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH)){
		*getPassword=EEPROM.read(startAddress);
		getPassword++;
		startAddress++;
	}
	return NETWORK_PASSWORD;
}

unsigned char FIRST_RUN=0;
void eepromInit(void){
	EEPROM.begin(MAX_EEPROM_SIZE);
	if(EEPROM.read((MAX_EEPROM_SIZE-1))==0xFF){
		for(unsigned short clearCounter=0;clearCounter<MAX_EEPROM_SIZE;clearCounter++){
			EEPROM.write(clearCounter,0);
		}
		EEPROM.commit();
		FIRST_RUN=1;
		wifiPasswordSave((unsigned char*)"12345678");
	}
	return;
}


unsigned char *scanForWifi(unsigned char *networksObject){
	unsigned char *orgLocation=networksObject;
	int n = WiFi.scanNetworks();
	const unsigned char baseStr[14]="{\"network\":[\"";
	unsigned char *makeBaseStr=(unsigned char *)baseStr;
	while(*makeBaseStr){
		*networksObject=(*makeBaseStr);
		makeBaseStr++;
		networksObject++;
	}
	for (unsigned char i = 0; i < n; ++i) {
		
		unsigned char scannedSSIDLength=WiFi.SSID(i).length();
		for(unsigned char makeObject=0;makeObject<scannedSSIDLength;makeObject++){
			*networksObject=WiFi.SSID(i)[makeObject];
			networksObject++;
		}

		*networksObject=0x22;
		networksObject++;
		*networksObject=0x2C;
		networksObject++;

		unsigned char *scannedRSSI=longToString(WiFi.RSSI(i));
		unsigned char avoidErrorCounter=0;
		while(*scannedRSSI&&avoidErrorCounter<3){
			*networksObject=(*scannedRSSI);
			scannedRSSI++;
			networksObject++;
			avoidErrorCounter++;
		}

		if(i!=(n-1)){
			*networksObject=0x2C;
			networksObject++;
			*networksObject=0x22;
			networksObject++;
		}
	}
	*networksObject=0x5D;
	networksObject++;
	*networksObject=0x7D;
	networksObject++;

	while(*networksObject){
		*networksObject=0;
		networksObject++;
	}


	
	return orgLocation;
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


unsigned char *tcpGetString(WiFiClient &client,unsigned char *memoryBuffer){
	unsigned char *tcpBuffer=memoryBuffer;
	while(client.available()){
		if(!(*memoryBuffer)){
			*memoryBuffer=client.read();
			memoryBuffer++;
		}	
		else{
			client.read();
		}
	}
	*memoryBuffer=0;
	return tcpBuffer;
}

#define USER_REQUEST 0x01
#define WEB_SOCKET 0x02
#define UPLOADED_DATA 0x04
#define WEB_SERVER 0x08
unsigned char *EVENT_DATA;



unsigned char eventIdentifier(unsigned char *userRequest){
	unsigned char str1[5]="def:";
	unsigned char str2[20]="Sec-WebSocket-Key: ";
	unsigned char str3[6]="data:";
	unsigned char str4[10]="GET /home";
	#define cl1 4
	#define cl2 19
	#define cl3 5
	#define cl4 9
	unsigned char c1=0;
	unsigned char c2=0;
	unsigned char c3=0;
	unsigned char c4=0;
	while((cl1-c1)&&(cl2-c2)&&(cl3-c3)&&(cl4-c4)&&(*userRequest)){
		c1*=((*userRequest)==str1[c1++]);
		c2*=((*userRequest)==str2[c2++]);
		c3*=((*userRequest)==str3[c3++]);
		c4*=((*userRequest)==str4[c4++]);
		userRequest++;
	}
	EVENT_DATA=userRequest;
	return((USER_REQUEST*(c1==cl1))|(WEB_SOCKET*(c2==cl2))|(UPLOADED_DATA*(c3==cl3))|(WEB_SERVER*(c4==cl4)));
}



unsigned char *eventData(unsigned char* startLocation,unsigned char* originalString,unsigned char *endString){
	unsigned char *endLocation=startLocation;
	unsigned short endStringLength=stringCounter(endString);
	unsigned short valueMatchCounter=0;
	while((endStringLength-valueMatchCounter)&&(*endLocation)){
		valueMatchCounter*=((*endLocation)==endString[valueMatchCounter++]);
		endLocation++;
	}
	if(endStringLength==valueMatchCounter){
		endLocation-=endStringLength;
		unsigned char *makeStr=originalString;
		while(startLocation<endLocation){
			*makeStr=(*startLocation);
			makeStr++;
			startLocation++;
		}
		while(*makeStr){
			*makeStr=0;
			makeStr++;
		}
	}
	return originalString;
}








//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    LEGACY-CODE    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


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
