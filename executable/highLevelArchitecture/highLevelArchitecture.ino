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
#include"custom/fetch.h"

#include "custom/jsonParser.h"

#include "custom/localStorage.h"

#include "custom/operaotors.h"

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



web::service webServer;

void setup(){
	// esp_task_wdt_init(-1,false);		//this is the most important line of this entire code esp_task_wdt_init(uint32_t timeInSec,bool panic);
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
		WIFI_UTILS::connected()?console.log("CONNECTED @ <",WIFI_UTILS::currentIP(),">"):console.log("FAILED TO CONNECT -> ACCESS POINT ACTIVE <",WIFI_UTILS::currentIP(),"> same password");
		WIFI_UTILS::accessPoint();
	};

	
	EEPROM_UTILS::ssidSave((uint8_t*)"RISC-V");					//^ for testing only
	EEPROM_UTILS::wifiPasswordSave((uint8_t*)"threadripper");	//^ for testing only

	MEMORY[WIFI_SETTINGS]=JSON_OBJECT(JSON_KEYS(NETWORK_SSID,NETWORK_PASSWORD),JSON_VALUES(EEPROM_UTILS::userSSID(),EEPROM_UTILS::userPassword()));

	webServer.onData([&](uint8_t *data){
		console.log("data >> ",data);
		// webServer.send(data);
		// webServer.httpSetResponse(data);
	});

	async({
		_delay_ms(10000);
		console.log("starting service");
		webServer.setup(80,"/");

	});

	MEMORY["BUFFER"]=JSON_OBJECT(JSON_KEYS(OPERATOR,DATA),JSON_VALUES((uint8_t*)"readTest",(uint8_t*)"CONSOLE_LOGGER"));
	
	uint8_t *testPtr=JSON_OBJECT(JSON_KEYS(OPERATOR,DATA),JSON_VALUES((uint8_t*)CONSOLE_LOGGER,(uint8_t*)MEMORY["BUFFER"]));
	MEMORY["BUFFER"]=testPtr;

	std::string testInstruction=std::string("{\"LOOP_COUNTER\":5,\"LOOP_ELEMENENTS\":[")+std::string((char*)MEMORY["BUFFER"])+std::string("]}");
	// (char*)JSON_OBJECT(JSON_KEYS(OPERATOR,DATA),JSON_VALUES((uint8_t*)LOOP,(uint8_t*)testInstruction.c_str()))
	std::string testInstruction2=std::string("{\"LOOP_COUNTER\":5,\"LOOP_ELEMENENTS\":[")+std::string((char*)JSON_OBJECT(JSON_KEYS(OPERATOR,DATA),JSON_VALUES((uint8_t*)LOOP,(uint8_t*)testInstruction.c_str())))+std::string("]}");;



	// std::make_shared<$json>(testInstruction2);
	// auto test0=jsonParser(testInstruction2)["LOOP_COUNTER"];
	// auto test2=jsonParser(testInstruction2)["LOOP_ELEMENENTS[0]"];
	// auto test1=jsonParser(testInstruction2)["LOOP_ELEMENENTS"];

	auto testCall=[&](uint8_t *param1,uint8_t *param2,uint8_t *param3){
		console.log("test0 >> ",param1);
		console.log("test0 >> ",param2);
		console.log("test0 >> ",param3);
		return;
	};
	

	testCall(jsonParser(testInstruction2)["LOOP_COUNTER"],jsonParser(testInstruction2)["LOOP_ELEMENENTS[0]"],jsonParser(testInstruction2)["LOOP_ELEMENENTS"]);
	

	// console.log(JSON_OBJECT(JSON_KEYS(OPERATOR,DATA),JSON_VALUES((uint8_t*)LOOP,(uint8_t*)testInstruction.c_str())));

	threadRunner(JSON_OBJECT(JSON_KEYS(OPERATOR,DATA),JSON_VALUES((uint8_t*)LOOP,(uint8_t*)testInstruction2.c_str())));
	// threadRunner(testPtr);

}

void loop(){
	// ws.cleanupClients();
}

