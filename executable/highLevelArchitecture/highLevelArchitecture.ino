#include "custom/consoleLogger.h"
#include "custom/consoleLogger.cpp"
#include "custom/highLevelMemory.cpp"

void setup(){
    Serial.begin(115200);
    console.addConsole([&](unsigned char *cosnoleData,unsigned char autoNLCR){
      if(autoNLCR)
        Serial.println((char*)cosnoleData);
      else
        Serial.print((char*)cosnoleData);
    });

    console.log("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    console.log("code just started");

    smart::highLevelMemory test(2000);// = highLevelMemory(2000);

	smart::highLevelMemory test2(2000);//= highLevelMemory(2000);

	test.write(101, (uint8_t*)"test 101");
	test.write(202, (uint8_t*)"test 202");

	test.get(101).onChange([&](unsigned char* newData) {
		console.log("101 >> ",newData);
	});

	test.get(202).onChange([&](unsigned char* newData) {
		console.log("202 >> ",newData);
	});

	test.get(101).onRead([&]() {
		test.write(101, (uint8_t*)"this is extreme test!!!!!!!!!!!");
	});

    test2.write(101, (uint8_t*)"same text");
    test2.write(102, (uint8_t*)"same text");
	test2.write(202, (uint8_t*)"not same text");

    console.log("test for op == >> ",(uint16_t)(test2[101]==test2[202]));
    console.log("test for op == >> ",(uint16_t)(test2[101]==test2[102]));

	(test[101]="test")="manga";
	(test[202]=test[101])=test2[101];
	test[101]=test2[202];
}

void loop(){

}