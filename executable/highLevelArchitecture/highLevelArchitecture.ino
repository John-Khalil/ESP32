#include "custom/consoleLogger.h"
#include "custom/consoleLogger.cpp"
#include "custom/highLevelMemory.cpp"
#include "custom/pointerTool.h"

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

	(test[101]>>([&](unsigned char* newData) {
		console.log("101 >> ",newData);
	}))="super random test";

	test[202]>>([&](unsigned char* newData) {
		console.log("202 >> ",newData);
	});

	test["indicator"]>>([&](unsigned char* newData) {
		console.log("indicator >> ",newData);
	});

	// test[101]<<([&]() {
	// 	test.write(101, (uint8_t*)"this is extreme test!!!!!!!!!!!");
	// });
	


	test[101]="test this 101";

	test["indicator"]="random bs";

	test[202]=test[101];
	test["indicator"]=test[202];
	test[101]=test[202];
	// test[202]="this text should appear !!";

	console.log(" ----------->> ",test.read(202));

    test2.write(101, (uint8_t*)"same text");
    test2.write(102, (uint8_t*)"same text");
	test2.write(202, (uint8_t*)"not same text");

    console.log("test for op == >> ",(uint16_t)(test2[101]==test2[202]));
    console.log("test for op == >> ",(uint16_t)(test2[101]==test2[102]));

	console.log("interesting test >> ",(char*)(test["898"]=test[101]));

	console.log("super interesting test >> ",(char*)test["898"]);

	test["indicator"]="123";

	test["525"]|=test["indicator"];
	test["525"]|="this is some super random default text";

	console.log("super interesting test >> ",(char*)test["525"]);

}

void loop(){

}