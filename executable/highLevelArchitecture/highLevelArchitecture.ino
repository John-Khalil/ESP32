#include "custom/consoleLogger.h"
#include "custom/consoleLogger.cpp"
#include "custom/highLevelMemory.cpp"
#include "custom/pointerTool.h"
#include "custom/constDefinitions.h"
#include "custom/eepromBasicConfig.h"
#include "custom/networkUtils.h"
#include "custom/platform.h"
#include "custom/http.h"
#include "custom/webService.h"
#include "custom/fetch.h"

#include "custom/jsonParser.h"

#include "custom/localStorage.h"

#include "custom/operaotors.h"

#include "custom/mqttClient.h"

#include "custom/cyclicBinary.cpp"

// #include "custom/ledMatrix.h"

#include <iostream>
#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>





// using namespace std;

utils::highLevelMemory MEMORY(5000);

cyclicBinaryDecompression outputPort([&](uint32_t sysTick){});


mqttClient mqttServer;
web::service webServer;

void setup(){
	disableAutoReset();
    delayAutoCalibrate();
    Serial.begin(115200);
	EEPROM_UTILS::eepromInit();
    console.addConsole([&](unsigned char *cosnoleData,unsigned char autoNLCR){
      if(autoNLCR)
        Serial.println((char*)cosnoleData);
      else
        Serial.print((char*)cosnoleData);
    });

    console.log("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    console.log("code just started");

	MEMORY[WIFI_SETTINGS]>>[&](unsigned char* eventData){
		if(json(NETWORK_SSID,eventData)==UNDEFINED){
			console.log("WiFi Disconnect");
			WIFI_UTILS::accessPoint();
			return;
		}
		MEMORY[NETWORK_SSID]=constJson(NETWORK_SSID,eventData);
		MEMORY[NETWORK_PASSWORD]=constJson(NETWORK_PASSWORD,eventData);
		EEPROM_UTILS::ssidSave(MEMORY[NETWORK_SSID]);
		EEPROM_UTILS::wifiPasswordSave(MEMORY[NETWORK_PASSWORD]);
		console.log("connecting to -> ",(char*)MEMORY[NETWORK_SSID]," <",(char*)MEMORY[NETWORK_PASSWORD],"> ...");
		WIFI_UTILS::networkConnect();
		WIFI_UTILS::connected()?([&](void){
			console.log("CONNECTED @ <",WIFI_UTILS::currentIP(),">");
			appLinker["networkConnect"]=$("CONNECTED @ <",WIFI_UTILS::currentIP(),">");
		})():console.log("FAILED TO CONNECT -> ACCESS POINT ACTIVE <",WIFI_UTILS::currentIP(),"> same password");
		WIFI_UTILS::accessPoint();
	};

	
	EEPROM_UTILS::ssidSave((uint8_t*)"RISC-V");					//^ for testing only
	EEPROM_UTILS::wifiPasswordSave((uint8_t*)"threadripper");	//^ for testing only


	// console.log(" -->> ",localStorage.getItem(556).c_str());
	// while(1);


	// const char* testInstruction0="{\"OPERATOR\":\"LOOP\",\"DATA\":{\"LOOP_COUNTER\":1,\"LOOP_ELEMENENTS\":[{\"OPERATOR\":\"ADD_THREAD\",\"DATA\":{\"THREAD_ID\":\"task1\",\"THREAD_EXECUTABLE\":{\"THREAD_PRIORITY\":50,\"THREAD_EXECUTABLE\":{\"OPERATOR\":\"CONSOLE_LOGGER\",\"DATA\":\"this is test from a running ............task1 \"}}}},{\"OPERATOR\":\"ADD_THREAD\",\"DATA\":{\"THREAD_ID\":\"task0\",\"THREAD_EXECUTABLE\":{\"THREAD_PRIORITY\":1,\"THREAD_EXECUTABLE\":{\"OPERATOR\":\"CONSOLE_LOGGER\",\"DATA\":\"this is test from a running task0 \"}}}}]}}";
	// threadRunner((uint8_t*)testInstruction0);
	// console.log("threadRunner return");
	// while(1)
	// 	runThreads();

	appLinker["networkConnect"]>>[&](uint8_t *eventData){

		mqttServer.setup("SnNvbi1NYWNoaW5l","SnNvbi1NYWNoaW5l",(char*)MQTT_TOPIC,"mqtt-dashboard.com");
		// mqttServer.setup("U25OdmJpMU5ZV05vYVc1bA==","U25OdmJpMU5ZV05vYVc1bA==","U25OdmJpMU5ZV05vYVc1bA==","mqtt-dashboard.com");
		mqttServer.onData([&](uint8_t *data){
			// appLinker[MQTT_RX]=data;
			// console.log(data);
			appLinker[GENERIC_RX]=data;
		});

		webServer.onData([&](uint8_t *data){
			// appLinker[WEB_SERVER_RX]=data;
			appLinker[GENERIC_RX]=data;
		});

		console.log("starting service");
		webServer.setup(80,"/");

		async({
			for(;;){
				mqttServer.loop();
			}
		});

		loadStartupScripts();

		// console.log("api call >> ",fetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json"));

		return;
	};

	appLinker[GENERIC_RX]>>[&](uint8_t *eventData){
		console.log("data >> ",eventData);
		threadRunner(eventData);
	};

	// appLinker[MQTT_RX]>>[&](uint8_t *eventData){
	// 	console.log("data >> ",eventData);
	// 	threadRunner(eventData);
	// };

	appLinker[MQTT_TX]>>[&](uint8_t *eventData){
		mqttServer.send(eventData);
	};

	// appLinker[WEB_SERVER_RX]>>[&](uint8_t *eventData){
	// 	console.log("data >> ",eventData);
	// 	threadRunner(eventData);
	// };

	appLinker[WEB_SERVER_TX]>>[&](uint8_t *eventData){
		webServer.send(eventData);
		webServer.httpSetResponse(eventData);
	};
	
	outputPort.onData([&](uint16_t portValue){
		console.log(intToHexaDecimal(portValue));
	});

	appLinker["base64decode"]>>[&](uint8_t *eventData){
		// union{
		// 	uint8_t* base64;
		// 	uint32_t* rawData;
		// }signalBuffer;

		// uint32_t rawDataLength=(stringCounter(eventData)*0.75)/4;
		// signalBuffer.base64=base64Decode(eventData);

		// uint32_t outputCounter=0;
		// while(rawDataLength--){
		// 	console.log(intToHexaDecimal(signalBuffer.rawData[outputCounter++]));
		// }

		outputPort.decode(eventData);

		

		return;
	};




	MEMORY[WIFI_SETTINGS]=JSON_OBJECT(JSON_KEYS(NETWORK_SSID,NETWORK_PASSWORD),JSON_VALUES(EEPROM_UTILS::userSSID(),EEPROM_UTILS::userPassword()));

	

	// async({
	// 	for(;;){
	// 		while(!Serial.available())_delay_ms(1);
	// 		mqttServer.send((uint8_t*)Serial.readStringUntil('\n').c_str());
	// 	}
	// });

	
	// uint8_t *testPtr=(uint8_t*)calloc(5,sizeof(uint8_t));
	
	// testPtr[0]=255;
	// testPtr[1]=0;
	// testPtr[2]=255;
	// testPtr[3]=255;

	// uint32_t* testPtr16=(uint32_t*)testPtr;

	// union{
	// 	uint8_t* testPtr=(uint8_t*)calloc(50,sizeof(uint8_t));
	// 	uint32_t* testPtr32;
	// }testPtr;

	// uint16_t testCounter=0;

	// testPtr.testPtr[testCounter++]=0x11;
	// testPtr.testPtr[testCounter++]=0x22;
	// testPtr.testPtr[testCounter++]=0x33;
	// testPtr.testPtr[testCounter++]=0x44;

	// testPtr.testPtr[testCounter++]=0x55;
	// testPtr.testPtr[testCounter++]=0x66;
	// testPtr.testPtr[testCounter++]=0x77;
	// testPtr.testPtr[testCounter++]=0x88;

	// testPtr.testPtr[testCounter++]=0x99;
	// testPtr.testPtr[testCounter++]=0xAA;
	// testPtr.testPtr[testCounter++]=0xBB;
	// testPtr.testPtr[testCounter++]=0xCC;



	// console.log(intToHexaDecimal(testPtr.testPtr32[0]));
	// console.log(intToHexaDecimal(testPtr.testPtr32[1]));
	// console.log(intToHexaDecimal(testPtr.testPtr32[2]));









}

void loop(){
	// ws.cleanupClients();
	static uint32_t loopCounter;
	// mqttServer.loop();

	// if(!(loopCounter++%99999))
	// 	mqttServer.send("this is test");
	// if(Serial.available())
	// 	mqttServer.send((uint8_t*)Serial.readStringUntil('\n').c_str());

	runThreads();

}

