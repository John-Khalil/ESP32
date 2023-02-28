#include "custom/consoleLogger.h"
#include "custom/consoleLogger.cpp"
#include "custom/highLevelMemory.cpp"
#include "custom/pointerTool.h"
#include "custom/constDefinitions.h"
#include "custom/eepromBasicConfig.h"

#include <iostream>

using namespace std;

utils::highLevelMemory MEMORY(20000);



void setup(){
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
		if(json("NETWORK_SSID",eventData)==UNDEFINED){
			console.log("WiFi Disconnect");

			return;
		}


		MEMORY[NETWORK_SSID]=constJson(NETWORK_SSID,eventData);
		MEMORY[NETWORK_PASSWORD]=constJson(NETWORK_PASSWORD,eventData);

		EEPROM_UTILS::ssidSave(MEMORY[NETWORK_SSID]);
		EEPROM_UTILS::wifiPasswordSave(MEMORY[NETWORK_PASSWORD]);

		console.log(" - connecting to -> ",(char*)MEMORY[NETWORK_SSID]," @ ",(char*)MEMORY[NETWORK_PASSWORD]);


	};

	EEPROM_UTILS::ssidSave((uint8_t*)"RISC-V");					//^ for testing only
	EEPROM_UTILS::wifiPasswordSave((uint8_t*)"threadripper");	//^ for testing only

	MEMORY[WIFI_SETTINGS]=JSON_OBJECT(JSON_KEYS(NETWORK_SSID,NETWORK_PASSWORD),JSON_VALUES(EEPROM_UTILS::userSSID(),EEPROM_UTILS::userPassword()));

}

void loop(){

}


    // utils::highLevelMemory test(2000);// = highLevelMemory(2000);

	// utils::highLevelMemory test2(2000);//= highLevelMemory(2000);

	// test.write(101, (uint8_t*)"test 101");
	// test.write(202, (uint8_t*)"test 202");

	// (test[101]>>([&](unsigned char* newData) {
	// 	console.log("101 >> ",newData);
	// }))="super random test";

	// test[202]>>([&](unsigned char* newData) {
	// 	console.log("202 >> ",newData);
	// });

	// test["indicator"]>>([&](unsigned char* newData) {
	// 	console.log("indicator >> ",newData);
	// });

	// test[101]<<([&]() {
	// 	test.write(101, (uint8_t*)"this is extreme test!!!!!!!!!!!");
	// });
	


	// test[101]="test this 101";

	// test["indicator"]="random bs";

	// test[202]=test[101];
	// test["indicator"]=test[202];
	// test[101]=test[202];
	// // test[202]="this text should appear !!";

	// console.log(" ----------->> ",test.read(202));

    // test2.write(101, (uint8_t*)"same text");
    // test2.write(102, (uint8_t*)"same text");
	// test2.write(202, (uint8_t*)"not same text");

    // console.log("test for op == >> ",(uint16_t)(test2[101]==test2[202]));
    // console.log("test for op == >> ",(uint16_t)(test2[101]==test2[102]));

	// console.log("interesting test >> ",(char*)(test["898"]=test[101]));

	// console.log("super interesting test >> ",(char*)test["898"]);

	// test["indicator"]="123";

	// test["525"]|=test["indicator"];
	// test["525"]|="this is some super random default text";

	// console.log("super interesting test >> ",(char*)test["525"]);

	// console.log("\n\n\nbind test----------------");

	// test[202].bind(101);

	// console.log(" ----------->> ",test.read(202));

	// test[202]="this should appear @ 101";

	// std::cout<<"did it just work here"<<std::endl;
