#include <stdint.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include "spiConsole.cpp"
// #include "LittleFS.h"


void timeTicks(unsigned long timeInUs){
	while(timeInUs){ 
		_NOP();
		timeInUs--;
	}
	return;
}

#define calibratedFreq 100

#define devTick 15.2129f
#define devTickError 0.001f//0.000008f


#define argDataTypeMax 0xFFFFFFFF

#define delayUS(timeInUs) timeTicks((unsigned long)(((timeInUs*devTick)-((timeInUs*devTickError)*(timeInUs>calibratedFreq))+((timeInUs*devTickError)*(timeInUs<calibratedFreq)))))

#define _delay_us delayUS
#define _delay_ms delay
 
void microSec(unsigned long microSeconds){
	_delay_us(microSeconds);
}

#define WD_FEED //_delay_ms(0)

unsigned char WATCH_DOG_GLOBAL_TIMER=0;
#define _WDF //if(!(--WATCH_DOG_GLOBAL_TIMER))WD_FEED;

unsigned short WATCH_DOG_GLOBAL_TIMER_16BIT=0;
#define _WDSF //if(!(--WATCH_DOG_GLOBAL_TIMER_16BIT))WD_FEED;

#define _DW digitalWrite
#define _DR digitalRead
#define _PM pinMode




#define _TIMERINTERRUPT_LOGLEVEL_     1
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default
#include "ESP8266TimerInterrupt.h"
#define TIMER_INTERVAL_MS       2700

ESP8266Timer ITimer;



// unsigned char interruptEnable=1;

void  TimerHandler(){
	// static unsigned char cycleCounter=4;
	// if(!(cycleCounter-=(cycleCounter!=0)))
	// 	delay(0);
	// if(interruptEnable){
		ESP.wdtFeed();
    // 	delay(0);
	// }
	// _NOP();
}
#define disableAutoReset() ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler)
#define enableInt() //interruptEnable=1
#define disableInt() //interruptEnable=0




// #define fileSystem LittleFS

unsigned char UNDEFINED[10]="undefined";

#define lengthInBytes(object) (unsigned char *)(&object+1)-(unsigned char *)(&object)
unsigned long strToUint32(unsigned char *str) {
	unsigned char coun = 0;
	unsigned long num;
	while (*str) {
		if (((*str) > 0x2F) && ((*str) < 0x3A)) {
			if (!coun)
				num =(unsigned) str;
			coun++;
		}
		else if (coun)
			break;
		str++;
	}
	if (!coun)
		return 0;
	str = (unsigned char*)num;
	unsigned long ret = 0;
	unsigned x = 1;
	while (coun) { //while (coun > 0)
		ret += (str[coun - 1] - 0x30)*x;
		x *= 10;
		coun--;
	}
	return ret;
}

#define HEX_CHARS(FOUR_BIT) (((FOUR_BIT+0x30)*(FOUR_BIT<10))|((FOUR_BIT+0x37)*(FOUR_BIT>9)))
void intToHex(unsigned long decimalFrom,unsigned char *hexaDecimalForm){
	CLR(hexaDecimalForm);
	unsigned char numLength=8-((((decimalFrom&0xFF000000UL)!=0)+((decimalFrom&0x00FF0000UL)!=0)+((decimalFrom&0x0000FF00UL)!=0)+1)*2);
	for(unsigned char i=numLength;i<8;i++){
		unsigned char fourBits=(unsigned char)((decimalFrom&((unsigned long)(0xf0000000>>(4*i))))>>(4*(7-i)));
		*hexaDecimalForm=HEX_CHARS(fourBits);
		hexaDecimalForm++;
	}
	return;
}






unsigned char inttostr(unsigned long num, unsigned char *str) {
	static unsigned char ucoun;
	for (unsigned char clrCoun = 0; clrCoun < ucoun; clrCoun++) {
		str[clrCoun] = 0;
	}
	ucoun = 0;
	if (num) {
		unsigned long sum = 0;
		unsigned long r = 10000000;
		for (unsigned char i = 0; i < 8; i++) {
			unsigned char flg = 0;
			for (unsigned char j = 9; j > 0; j--) {
				if ((num - sum) >= r * j) {
					sum += r * j;
					*str = (j)+0x30;
					ucoun++;
					str++;
					flg = 1;
					break;
				}
			}
			if (flg == 0 && sum != 0) {
				*str = 0x30;
				ucoun++;
				str++;
			}
			r /= 10;
		}
	}
	else {
		*str = 0x30;
		ucoun = 1;
	}
	return ucoun;
}



unsigned char globalStringNameThatYouWillNeverUse[11]="";
unsigned char* inttostring(unsigned long num) {
	CLR(globalStringNameThatYouWillNeverUse);
	inttostr(num, globalStringNameThatYouWillNeverUse);
	return globalStringNameThatYouWillNeverUse;
}

unsigned char *intToHexaDecimal(unsigned long num){
	unsigned char *makeStr=globalStringNameThatYouWillNeverUse;
	*makeStr=0x30;
	makeStr++;
	*makeStr=0x78;
	makeStr++;
	intToHex(num,makeStr);
	return globalStringNameThatYouWillNeverUse;
}


#define C_HEX intToHexaDecimal
#define C_INT inttostring


unsigned char* longToString(long num){
	unsigned char *signedStr=globalStringNameThatYouWillNeverUse;
	if(num<0){
		*signedStr=0x2D;
		signedStr++;
		num*=(-1);
	}
	inttostr((unsigned long)num, signedStr);
	return globalStringNameThatYouWillNeverUse;
}


unsigned strint(unsigned char *str){
	unsigned ret=0;
	unsigned char coun=0;
	while((str[coun])&&(str[coun]>0x2F)&&(str[coun]<0x3A)){
		coun++;
	}
	unsigned x=1;
	while(coun>0){
		ret+=(str[coun-1]-0x30)*x;
		x*=10;
		coun--;
	}
	return ret;
}

uint64_t getInt(unsigned char *numStr){
	unsigned char numLength=stringCounter(numStr);
	uint64_t result=0;
	uint64_t factor=1;
	while(numLength--){
		result+=(numStr[numLength]-0x30)*factor;
		factor*=10;
	}
	return result;
}

unsigned char UNDEFINED_VALUE=0;
unsigned char *NO_DATA=&UNDEFINED_VALUE;


unsigned short stringCounter(unsigned char *counted){
	unsigned short counter=0;
	while(*counted){
		counter++;
		counted++;
	}
	return counter;
}

unsigned short CLR_LENGTH=0;									//this value will be reseted to zero after clearing the string/uint_8 pointer
unsigned char * CLR(unsigned char *deletedString){
	unsigned char *returnedString=deletedString;
	while(*deletedString||(CLR_LENGTH-=(CLR_LENGTH!=0))){
		*deletedString=0;
		deletedString++;	
	}
	return returnedString;
}

unsigned char equalStrings(unsigned char *stringPointer1,unsigned char *stringPointer2){
	unsigned short diffCounter;
	if((diffCounter=stringCounter(stringPointer1))!=stringCounter(stringPointer2))
		return 0;
	while((stringPointer1[--diffCounter]==stringPointer2[diffCounter])&&diffCounter)
	return !diffCounter;
}

unsigned char *_CS(unsigned char *bigString,unsigned char *smallString){
	unsigned char *smallStringLocation=bigString+stringCounter(bigString);		// lucky for us c/c++ support pointer arthematic
	while(*smallString){
		*smallStringLocation=*smallString;
		smallString++;
		smallStringLocation++;
	}
	return bigString;
}

// unsigned char ARRAY_OF_INDEX_GLOBAL[13]="";
// unsigned char *listIndex(unsigned long indexOFArray){
// 	CLR(ARRAY_OF_INDEX_GLOBAL);
// 	unsigned char mkStringCounter=0;
// 	ARRAY_OF_INDEX_GLOBAL[mkStringCounter]=0X5B; // square bracket openning
// 	unsigned char *indexOFArrayString=inttostring(indexOFArray);
// 	while(*indexOFArrayString){_WDF;
// 		ARRAY_OF_INDEX_GLOBAL[++mkStringCounter]=*indexOFArrayString;
// 		indexOFArrayString++;
// 	}
// 	ARRAY_OF_INDEX_GLOBAL[++mkStringCounter]=0x5D;	//square bracket closing
// 	return ARRAY_OF_INDEX_GLOBAL;
// }

unsigned char *skipStringJSON(unsigned char *jsonString){
	if(*jsonString==0x22){//stop searching inside a string
		unsigned char *backSlash=jsonString-1;
		if(*backSlash==0x3A){
			while((*jsonString!=0x22)&&(*backSlash!=0x5C)){
			backSlash=jsonString-1;
			jsonString++;
			}
		}
	}
	return jsonString;
}

unsigned char*skipArrayJSON(unsigned char *jsonString){
	if(*jsonString==0x5B){//skip an array
		jsonString++;
		unsigned short arrayStart=1;
		unsigned short arrayEnd=0;
		while(arrayStart!=arrayEnd){
			arrayStart+=(*jsonString==0x5B);
			arrayEnd+=(*jsonString==0x5D);
			jsonString=skipStringJSON(jsonString);
			jsonString++;
		}
	}
	return jsonString;
}

unsigned char *skipObjectJSON(unsigned char *jsonString){
	if(*jsonString==0x7B){//skip an object
		jsonString++;
		unsigned short objectStart=1;
		unsigned short objectEnd=0;
		while(objectStart!=objectEnd){
			objectStart+=(*jsonString==0x7B);
			objectEnd+=(*jsonString==0x7D);
			jsonString=skipStringJSON(jsonString);
			jsonString++;
		}
	}
	return jsonString;
}

unsigned char *JSON_OBJECT_FOUND_LOCATION=UNDEFINED;
unsigned char JSON(unsigned char *requestedJSON,unsigned char *jsonString,unsigned char *objectString){
	#define OBJECT_STRING_MAX_LENGTH 35
	unsigned char objectBuffer[OBJECT_STRING_MAX_LENGTH]="";
	unsigned short jsonArrayIndex=-1;
	unsigned char subObject=1;
	unsigned char *deadEndOfString=jsonString+stringCounter(jsonString);
	//jsonString++;
	unsigned char *objectScanner=requestedJSON;
	while(*objectScanner){
		subObject+=(*objectScanner==0x2E);
		objectScanner++;
	}
	for(unsigned char subObjectCounter=0;subObjectCounter<subObject;subObjectCounter++){	//start
		jsonString++;
		jsonArrayIndex=-1;
		unsigned char *jsonObject=objectBuffer;
		*jsonObject=0x22;
		jsonObject++;
		while(*requestedJSON&&(*requestedJSON!=0x2E)&&(*requestedJSON!=0x5B)){
			*jsonObject=*requestedJSON;
			requestedJSON++;
			jsonObject++;
		}
		*jsonObject=0x22;
		jsonObject++;
		while(jsonObject<(objectBuffer+OBJECT_STRING_MAX_LENGTH)){
			*jsonObject=0;
			jsonObject++;
		}
		jsonObject = objectBuffer;
		if(*requestedJSON==0x2E){
			requestedJSON++;
		}
		else if(*requestedJSON==0x5B){
			requestedJSON++;
			jsonArrayIndex=strint(requestedJSON);
			requestedJSON+=stringCounter(inttostring((unsigned long)jsonArrayIndex))+1;//very inefficiant but i dont care!! its a cleaner code
		}

		while(*jsonObject){WD_FEED; //start searching
			if(*jsonObject==*jsonString){//object to be found
				unsigned char *notString=jsonString-1;
				if(*notString!=0x3A){
					jsonObject++;
				}
			}
			else{
				jsonObject=objectBuffer;
			}
			jsonString=skipStringJSON(jsonString);
			jsonString=skipObjectJSON(jsonString);
			jsonString=skipArrayJSON(jsonString);
			if(jsonString==deadEndOfString){//object not found
				return 0;
			}
			jsonString++;
		}//object found

		jsonString++;//skip the ":"
		unsigned char objectTypeArray=0;
		searchInsideArray:
		unsigned short objectStartBracket=0;
		unsigned short objectEndBracket=0;
		unsigned short arrayStartBracket=0;
		unsigned short arrayEndBracket=0;
		unsigned short doubleQuoates=0;
		unsigned char *objectLocation=jsonString;

		while(1){
			objectStartBracket+=(*objectLocation==0x7B);
			objectEndBracket+=(*objectLocation==0x7D);
			arrayStartBracket+=(*objectLocation==0x5B);
			arrayEndBracket+=(*objectLocation==0x5D);
			unsigned char *validStringCheck=objectLocation-1;
			doubleQuoates+=((*validStringCheck!=0x5C)&&(*objectLocation==0x22));
			if(objectLocation==deadEndOfString){//object not found
				return 0;
			}
			while(doubleQuoates&0x01){
				objectLocation++;
				validStringCheck=objectLocation-1;
				doubleQuoates+=((*validStringCheck!=0x5C)&&(*objectLocation==0x22));
			}
			if((*objectLocation==0x2C) && (objectStartBracket == objectEndBracket) && (arrayStartBracket == arrayEndBracket))
				break;
			if(objectEndBracket>objectStartBracket)
				break;
			if(arrayEndBracket>arrayStartBracket)
				break;
			objectLocation++;
		}
		
		if(objectTypeArray){
			goto backToArrayCounter;
		}
		deadEndOfString=objectLocation;
		if((jsonArrayIndex!=0xFFFF)&&(*jsonString==0x5B)){
			//jsonArrayIndex++;
			jsonString++;
			objectTypeArray=1;
			while(jsonArrayIndex){
				goto searchInsideArray;
				backToArrayCounter:
				jsonString=objectLocation+1;
				if((*objectLocation!=0x2C)&&jsonArrayIndex!=1){
					return 0;
				}
				jsonArrayIndex--;
			}
			objectTypeArray=0;
			jsonArrayIndex=-1;
			goto searchInsideArray;
		}
	}

	// JSON_OBJECT_FOUND_LOCATION=jsonString;
	while(jsonString<deadEndOfString){
		*objectString=*jsonString;
		objectString++;
		jsonString++;
	}
	while(*objectString){
		*objectString=0;
		objectString++;
	}
	return 1;
}



#define JSON_OBJECT_SIZE 101
// unsigned char UNDEFINED[10]="undefined";
unsigned char JSON_OBJECT_FOUND[JSON_OBJECT_SIZE]="";

unsigned char *jsonParse(unsigned char *userObject,unsigned char *jsonObject){
	if(JSON(userObject,jsonObject,JSON_OBJECT_FOUND)){
		if(*JSON_OBJECT_FOUND==0x20){
			unsigned short charSpaceCounter=1;
			while(JSON_OBJECT_FOUND[charSpaceCounter++]==0x22);
			charSpaceCounter--;
			unsigned char *charSpacePointer=JSON_OBJECT_FOUND;
			unsigned char *realDataPointer=charSpacePointer+charSpaceCounter;
			while(*realDataPointer){
				*charSpacePointer=*realDataPointer;
				charSpacePointer++;
				realDataPointer++;
			}
			CLR(charSpacePointer);
		}
		if((*JSON_OBJECT_FOUND)==0x22){
			unsigned char* makeStrJSON=JSON_OBJECT_FOUND;
			unsigned short jsonObjectLength=stringCounter(makeStrJSON)-2;
			unsigned char *clearString=makeStrJSON+1;
			while(jsonObjectLength){
				(*makeStrJSON)=(*clearString);
				*clearString=0;
				clearString++;
				makeStrJSON++;
				jsonObjectLength--;
			}
			*clearString=0;
		}
		return JSON_OBJECT_FOUND;
	}
	return UNDEFINED;
}

#define json(argJSON1,argJSON2) jsonParse((unsigned char *)argJSON1,(unsigned char *)argJSON2)

// unsigned char *jsonReplace(unsigned char *orgObject,unsigned char *jsonKey,unsigned char *newValue){
// 	while(*JSON_OBJECT_FOUND_LOCATION==0x20){_WDF;
// 		JSON_OBJECT_FOUND_LOCATION++;
// 	}
// 	unsigned char *validJsonKey;
// 	if((validJsonKey=json(jsonKey,orgObject))!=UNDEFINED){_WDF;
// 		unsigned short removedValueLength=stringCounter(validJsonKey)+2*(*JSON_OBJECT_FOUND_LOCATION==0x22);
// 		unsigned short addedValueLength=stringCounter(newValue);
// 		unsigned char *allocationPointer=JSON_OBJECT_FOUND_LOCATION+removedValueLength;
// 		unsigned char *destenationPointer=JSON_OBJECT_FOUND_LOCATION+addedValueLength;
// 		if(removedValueLength>addedValueLength){_WDF;
// 			while(*allocationPointer){_WDF;
// 				*destenationPointer=*allocationPointer;
// 				destenationPointer++;
// 				allocationPointer++;
// 			}
// 			CLR(destenationPointer);
// 		}
// 		else if(removedValueLength<addedValueLength){_WDF;
// 			unsigned short nonValueLength=stringCounter(JSON_OBJECT_FOUND_LOCATION)-removedValueLength;
// 			while(nonValueLength--){_WDF;
// 				destenationPointer[nonValueLength]=allocationPointer[nonValueLength];
// 				allocationPointer[nonValueLength]=0;
// 			}
// 		}
// 		unsigned char *mkFinalObject=JSON_OBJECT_FOUND_LOCATION;
// 		while(*newValue){_WDF;
// 			*mkFinalObject=*newValue;
// 			mkFinalObject++;
// 			newValue++;
// 		}
// 	}
// 	return orgObject;
// }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char inverseBase64Table(unsigned char transBuf){
	return (transBuf-((65*((transBuf<0x5B)&&(transBuf>0x40)))|(71*((transBuf>0x60)&&(transBuf<0x7B)))|(-4*((transBuf>0x2F)&&(transBuf<0x3A)))|(-19*(transBuf==0x2B))|(-16*(transBuf==0x2F))))*(transBuf!=0x3D);//(char64-((0x41*(char64<26))|(71*((char64>25)&&(char64<52)))|(-4*((char64>51)&&(char64<62)))))
}

unsigned char *base64Decode(unsigned char *base64Text){
	unsigned char *startAddress=base64Text;
	unsigned char *lastAddress=startAddress+stringCounter(startAddress);
	unsigned char *base256Text=base64Text;
	unsigned short base64Counter=0;
	while(base64Text[base64Counter]){
		unsigned char base256Buffer[4]={inverseBase64Table(base64Text[base64Counter++]),inverseBase64Table(base64Text[base64Counter++]),inverseBase64Table(base64Text[base64Counter++]),inverseBase64Table(base64Text[base64Counter++])};
		*base256Text=(base256Buffer[0]<<2)|((base256Buffer[1]&0x30)>>4);
		base256Text++;
		*base256Text=(base256Buffer[1]<<4)|((base256Buffer[2]&0x3C)>>2);
		base256Text++;
		*base256Text=(base256Buffer[2]<<6)|base256Buffer[3];
		base256Text++;
	}
	while(base256Text<lastAddress){
		*base256Text=0;
		base256Text++;
	}
	return startAddress;
}



//#define base64TableUnused(transBuf) transBuf+((65*(transBuf<26))|(71*((transBuf>25)&&(transBuf<52)))|(-4*((transBuf>51)&&(transBuf<62)))|(-19*(transBuf==62))|(-16*(transBuf==63))) //this was never used.....why?
unsigned char base64Table(unsigned char transBuf){
		return transBuf+((65*(transBuf<26))|(71*((transBuf>25)&&(transBuf<52)))|(-4*((transBuf>51)&&(transBuf<62)))|(-19*(transBuf==62))|(-16*(transBuf==63)));
}
unsigned short base64(unsigned char *rawData, unsigned char *base64Text) {
	static unsigned short lastBase64Length;
	unsigned char *clearText=base64Text;
	while(lastBase64Length){
		*clearText=0;
		clearText++;
		lastBase64Length--;
	}
	unsigned short rawDataLength = stringCounter(rawData);
	unsigned char paddingCount = rawDataLength % 3;
	rawDataLength -= paddingCount;
	// rawDataLength *= 1.25;
	rawDataLength *= 1.3334f;
	unsigned short base64Counter = 0;
	unsigned char *rawData1 = rawData + 1;
	unsigned char *rawData2 = rawData + 2;
	while (base64Counter < rawDataLength) {
		base64Text[base64Counter++] = base64Table((*rawData) >> 2);
		base64Text[base64Counter++] = base64Table((((*rawData) & 0x03) << 4) | ((*rawData1) >> 4));
		base64Text[base64Counter++] = base64Table((((*rawData1) & 0x0F) << 2) | (((*rawData2) & 192) >> 6));
		base64Text[base64Counter++] = base64Table((*rawData2) & 0x3F);
		rawData += 3;
		rawData1 += 3; 
		rawData2 += 3;
	}
	if (paddingCount == 2) {
		base64Text[base64Counter++] = base64Table((*rawData) >> 2);
		base64Text[base64Counter++] = base64Table((((*rawData) & 0x03) << 4) | ((*rawData1) >> 4));
		base64Text[base64Counter++] = base64Table(((*rawData1) & 0x0F) << 2);
		base64Text[base64Counter++] = 0x3D;
	}
	else if (paddingCount == 1) {
		base64Text[base64Counter++] = base64Table((*rawData) >> 2);
		base64Text[base64Counter++] = base64Table(((*rawData) & 0x03) << 4);
		base64Text[base64Counter++] = 0x3D;
		base64Text[base64Counter++] = 0x3D;
	}
	lastBase64Length=base64Counter;
	return base64Counter;
}



unsigned short base64WebSocket(unsigned char *rawData, unsigned char *base64Text,unsigned short rawDataLength ) {
	unsigned char *clearText=base64Text;
	while(*clearText){
		*clearText=0;
		clearText++;
	}
	unsigned char paddingCount = rawDataLength % 3;
	rawDataLength -= paddingCount;
	rawDataLength *= 1.25;
	unsigned short base64Counter = 0;
	unsigned char *rawData1 = rawData + 1;
	unsigned char *rawData2 = rawData + 2;
	while (base64Counter < rawDataLength) {
		base64Text[base64Counter++] = base64Table((*rawData) >> 2);
		base64Text[base64Counter++] = base64Table((((*rawData) & 0x03) << 4) | ((*rawData1) >> 4));
		base64Text[base64Counter++] = base64Table((((*rawData1) & 0x0F) << 2) | (((*rawData2) & 192) >> 6));
		base64Text[base64Counter++] = base64Table((*rawData2) & 0x3F);
		rawData += 3;
		rawData1 += 3; 
		rawData2 += 3;
	}
	if (paddingCount == 2) {
		base64Text[base64Counter++] = base64Table((*rawData) >> 2);
		base64Text[base64Counter++] = base64Table((((*rawData) & 0x03) << 4) | ((*rawData1) >> 4));
		base64Text[base64Counter++] = base64Table(((*rawData1) & 0x0F) << 2);
		base64Text[base64Counter++] = 0x3D;
	}
	else if (paddingCount == 1) {
		base64Text[base64Counter++] = base64Table((*rawData) >> 2);
		base64Text[base64Counter++] = base64Table(((*rawData) & 0x03) << 4);
		base64Text[base64Counter++] = 0x3D;
		base64Text[base64Counter++] = 0x3D;
	}
	return base64Counter;
}

unsigned char* base64Encoder(unsigned char* rawText,unsigned char* base64Text,unsigned short rawDataLength){
	base64WebSocket(rawText,base64Text,rawDataLength);
	return base64Text;
}


unsigned char *secWebSocketAccept(unsigned char *clientBase64){
	unsigned char *originalAddress=clientBase64;
	unsigned char RFC6455[37]="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	unsigned char *readStr=RFC6455;
	clientBase64+=stringCounter(clientBase64);
	while(*readStr){
		*clientBase64=*readStr;
		clientBase64++;
		readStr++;
	}
	clientBase64=originalAddress;
	unsigned char buffer1[21]="";
    sha1((char*)clientBase64,&buffer1[0]);
	unsigned char hashedText[21]="";
	for(unsigned char hashCounter=0;hashCounter<20;hashCounter++){
		hashedText[hashCounter]=buffer1[hashCounter];
	}
	base64WebSocket(hashedText,clientBase64,20);
	return originalAddress;
}




#define FRAME_DECODE 0
#define FRAME_ENCODE 1
#define WEBSOCKET_FRAME_SIZE 1024
unsigned char WEBSOCKET_BUFFER_DATA[WEBSOCKET_FRAME_SIZE]="";
unsigned char FINAL_WEBSOCKET_PAYLOAD=1;
unsigned char WEBSOCKET_LAST_OPCODE=0;

unsigned char *webSocketDataFrame(unsigned char *frameData,unsigned char operation){
	unsigned char *originalLoaction=frameData;
	if(operation==FRAME_DECODE){
		unsigned char *payload=frameData+2;
		unsigned char maskingKey[4]="";
		FINAL_WEBSOCKET_PAYLOAD=((frameData[0]&128)==128);
		unsigned char opcode=(frameData[0]&0x0F);
		WEBSOCKET_LAST_OPCODE=opcode;
		unsigned char dataMasked=((frameData[1]&128)==128);
		unsigned long long payloadLength=(frameData[1]&0x7F);
		payload+=(2*(payloadLength==126));
		payload+=(8*(payloadLength==127));
		unsigned char *maskingKeyLocation=payload;
		payload+=(4*dataMasked);
		if(dataMasked){
			for(unsigned char maskCounter=0;maskCounter<4;maskCounter++){
				maskingKey[maskCounter]=maskingKeyLocation[maskCounter];
			}
		}
		unsigned char extraLength=(2*(payloadLength==126))|(8*(payloadLength==127));
		if(extraLength){
			unsigned char *payloadLengthLocation=maskingKeyLocation-1;
			unsigned char loopCounter=0;
			payloadLength=0;
			while(extraLength){
				payloadLength|=((*payloadLengthLocation)<<(8*loopCounter));
				loopCounter++;
				payloadLengthLocation--;
				extraLength--;
			}
		}
		unsigned long long frameLength=payloadLength+(payload-originalLoaction);
		for(unsigned long long transfarCounter=0;transfarCounter<payloadLength;transfarCounter++){
			*frameData=(*payload);
			frameData++;
			payload++;
		}
		unsigned char *endLocation=originalLoaction+frameLength; //avoid wint conv. bullshit
		while(frameData<endLocation){
			*frameData=0;
			frameData++;
		}
		frameData=originalLoaction;
		if(dataMasked){
			for(unsigned long long transfarCounter=0;transfarCounter<payloadLength;transfarCounter++){
				*frameData^=maskingKey[transfarCounter%4];
				frameData++;
			}
		}
	}
	else if(operation==FRAME_ENCODE){
		originalLoaction=WEBSOCKET_BUFFER_DATA;
		while(*originalLoaction){
			*originalLoaction=0;
			originalLoaction++;
		}
		originalLoaction=WEBSOCKET_BUFFER_DATA;
		*originalLoaction=0x81;
		originalLoaction++;
		unsigned long long payloadSize=stringCounter(frameData);
		payloadSize=(payloadSize*(payloadSize<WEBSOCKET_FRAME_SIZE))|((WEBSOCKET_FRAME_SIZE-1)*(payloadSize>(WEBSOCKET_FRAME_SIZE-1)));
		*originalLoaction=(((unsigned char)payloadSize)*(payloadSize<126))|(126*((payloadSize>125)&&(payloadSize<0x10000ULL)))|(127*(payloadSize>0xFFFF));
		unsigned char extraSize=(2*((*originalLoaction)==126))|(8*((*originalLoaction)==127));
		originalLoaction++;
		while(extraSize){
			extraSize--;
			*originalLoaction=(unsigned char)((payloadSize&(0xFF<<(8*extraSize)))>>(8*extraSize));
			originalLoaction++;
		}
		while(payloadSize){
			*originalLoaction=(*frameData);
			frameData++;
			originalLoaction++;
			payloadSize--;
		}
		originalLoaction=WEBSOCKET_BUFFER_DATA;
	}
	return originalLoaction;
}

unsigned char *webSocketDataFrameEncode(unsigned char *frameData){
	uint64_t payloadSize=stringCounter(frameData);
	unsigned char frameLength=(((unsigned char)payloadSize)*(payloadSize<126))|(126*((payloadSize>125)&&(payloadSize<0x10000ULL)))|(127*(payloadSize>0xFFFF));
	unsigned char extraSize=(2*((frameLength)==126))|(8*((frameLength)==127));
	unsigned char *originalLoaction=frameData-(2+extraSize);
	unsigned char *webSocketData=originalLoaction;
	*originalLoaction=0x81;
	originalLoaction++;
	*originalLoaction=frameLength;
	originalLoaction++;
	while(extraSize){
		extraSize--;
		*originalLoaction=(unsigned char)((payloadSize&(0xFF<<(8*extraSize)))>>(8*extraSize));
		originalLoaction++;
	}
	return webSocketData;
}

unsigned char *webSocketDataFrameEncodeBinary(unsigned char *frameData){
	uint64_t payloadSize=stringCounter(frameData);
	unsigned char frameLength=(((unsigned char)payloadSize)*(payloadSize<126))|(126*((payloadSize>125)&&(payloadSize<0x10000ULL)))|(127*(payloadSize>0xFFFF));
	unsigned char extraSize=(2*((frameLength)==126))|(8*((frameLength)==127));
	unsigned char *originalLoaction=frameData-(2+extraSize);
	unsigned char *webSocketData=originalLoaction;
	*originalLoaction=0x82;
	originalLoaction++;
	*originalLoaction=frameLength;
	originalLoaction++;
	while(extraSize){
		extraSize--;
		*originalLoaction=(unsigned char)((payloadSize&(0xFF<<(8*extraSize)))>>(8*extraSize));
		originalLoaction++;
	}
	return webSocketData;
}

// unsigned char *dataMask(unsigned char *dataStr,unsigned char *keyStr){
// 	unsigned short dataStrCounter=0;
// 	unsigned char keyStrLength=stringCounter(keyStr);
// 	while(dataStr[dataStrCounter])
// 		dataStr[dataStrCounter]^=keyStr[(dataStrCounter++)%keyStrLength];	
// 	return dataStr;
// }

unsigned char *dataMask(unsigned char *dataStr,unsigned char *keyStr){
	unsigned short dataStrCounter=0;
	unsigned char keyStrLength=stringCounter(keyStr);
	unsigned short dataStrLength=stringCounter(dataStr);
	while(dataStrLength--){
		dataStr[dataStrCounter]=dataStr[dataStrCounter]^keyStr[(dataStrCounter++)%keyStrLength];
	}	
	return dataStr;
}

unsigned char *tcps(unsigned char *rawText,unsigned char *keyStr,unsigned char *cipherText){
	base64(dataMask(rawText,keyStr),cipherText);
	return cipherText;
}

unsigned long long encKey64(unsigned char *base256Str){
	unsigned char encKeyCounter=0;
	unsigned long long key64Bit=0;
	while(encKeyCounter<8)
		key64Bit|=base256Str[encKeyCounter++]<<(64-(encKeyCounter*8));
	return key64Bit;
}



unsigned char asciiHex(unsigned char *base16){
	unsigned char res=0;
	for(unsigned char i=0;i<2;i++){
		//res=res<<4;
		if(((*base16)>0x2F)&&((*base16)<0x3A)){
			res=res<<4;
			res+=(*base16)-0x30;
		}
		else if(((*base16)>0x60)&&((*base16)<0x67)){
			res=res<<4;
			res+=(*base16)-0x57;
		}
		else if(((*base16)>0x40)&&((*base16)<0x47)){
			res=res<<4;
			res+=(*base16)-0x37;
		}
		base16++;
	}
	return res;
}




unsigned char validHex(unsigned char *base16){
	return((((*base16)>0x2F)&&((*base16)<0x3A))||(((*base16)>0x60)&&((*base16)<0x67))||(((*base16)>0x40)&&((*base16)<0x47)));
}




unsigned char *URIdecode(unsigned char *url){ //this fuction will work without checking if the the chars followed by the percentage char is valid
	unsigned char *originalLocation=url;
	unsigned char *shift;
	unsigned char *allocate;
	while(*url){
		if((*url)==0x25){
			shift=url+1;
			//if(validHex(shift)){
				(*url)=asciiHex(shift);
				allocate=shift+2;
				while(*allocate){
					(*shift)=(*allocate);
					(*allocate)=0;
					allocate++;
					shift++;
				}
			//}
		}
		url++;
	}
	return originalLocation;
}





#define clkPinNum 16
#define dataPinNum 10



void clk(unsigned char state){
	_DW(clkPinNum,state);
}

void data(unsigned char state){
	_DW(dataPinNum,state);
}

#define spiConsoleClk 16
#define spiConsoleData 10

#define shiftRegisterData 16
#define shiftRegisterClk 5
#define shiftRegisterLatch 4

void spiConsoleSetup(void){
	_PM(spiConsoleClk,OUTPUT);
	_PM(spiConsoleData,OUTPUT);
	_DW(spiConsoleData,1);
	_DW(spiConsoleClk,0);

	_PM(shiftRegisterClk,OUTPUT);
	_PM(shiftRegisterLatch,OUTPUT);
	_DW(shiftRegisterClk,0);
	_DW(shiftRegisterLatch,0);
	
	return;
}





const float SPI_CONSOLE_SPEED=(1e6f/360000)/2;			//max is 1.2Mbps

unsigned char *spiConsoleLog(unsigned char *consoleData){
	unsigned char *loggedData=consoleData;
	_DW(spiConsoleData,0);
	_delay_us(60);										//to sync with the receiver
	while(*consoleData){
		unsigned char spiBitCounter=8;
		while(spiBitCounter--){
			_DW(spiConsoleData,((*consoleData&(1<<spiBitCounter))!=0));
			_delay_us(SPI_CONSOLE_SPEED*0.4);
			_DW(spiConsoleClk,1);
			_delay_us(SPI_CONSOLE_SPEED);
			_DW(spiConsoleClk,0);
			_delay_us(SPI_CONSOLE_SPEED*0.6);
		}
		consoleData++;
	}
	_DW(spiConsoleData,1);
	return loggedData;
}






#define consoleLog(inputStringToConsole) spiConsoleLog((unsigned char *)inputStringToConsole)
#define cout_hex(coutStr,coutNum) consoleLog(coutStr); consoleLog(C_HEX(coutNum)); consoleLog("\r\n");
#define COUT_HEX cout_hex
#define cout_int(coutStr,coutNum) consoleLog(coutStr); consoleLog(C_INT(coutNum)); consoleLog("\r\n");
#define COUT_INT cout_int
#define consoleDebug(VARIABLE_X) consoleLog("\n"); consoleLog(#VARIABLE_X); consoleLog(" >> "); consoleLog(C_INT(VARIABLE_X)); consoleLog("\n"); 
#define _endl "\r\n"

#define systemLog(consoleLogText0) consoleLog("\n<div style=\"color: yellow;\">");consoleLog(consoleLogText0);consoleLog("</div>");
#define systemError(consoleLogText1) consoleLog("\n<div style=\"color: red;\">");consoleLog(consoleLogText1);consoleLog("</div>");




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

#define LINKER_CONFIG WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH
#define LINKER_CONFIG_LENGTH 100

#define EXTERNAL_PORT_CONFIG LINKER_CONFIG+LINKER_CONFIG_LENGTH
#define EXTERNAL_PORT_LENGTH 8

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

unsigned char linkerSave(unsigned char *linkerJson){
	unsigned short startAddress=LINKER_CONFIG;
	while(startAddress<(LINKER_CONFIG+LINKER_CONFIG_LENGTH)){
		EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),*linkerJson);
		startAddress++;
		linkerJson+=(*linkerJson!=0);
	}
	return (unsigned char)EEPROM.commit();
}

unsigned char *linkerData(void){
	CLR_LENGTH=EEPROM_BUFFER_SIZE;
	CLR(EEPROM_BUFFER);
	unsigned char *getLinkerData=EEPROM_BUFFER;
	unsigned short startAddress=LINKER_CONFIG;
	while(startAddress<(LINKER_CONFIG+LINKER_CONFIG_LENGTH)){
		*getLinkerData=EEPROM.read(startAddress);
		getLinkerData++;
		startAddress++;
	}
	return EEPROM_BUFFER;
}

unsigned char *EXTERNAL_PORT_LAST_VALUE=EEPROM_BUFFER;
const float EXTERNAL_OUTPUT_SPEED=(1e6f/50000)/2;
unsigned char EXTERNAL_PORT_SET_COUNTER=1;

unsigned char *externalPort(void){
	static unsigned char externalPortOnChange;
	CLR_LENGTH=EEPROM_BUFFER_SIZE;
	CLR(EEPROM_BUFFER);
	unsigned char externalPortBuffer[EXTERNAL_PORT_LENGTH];
	unsigned char externalPortLength=EXTERNAL_PORT_LENGTH;
	while(externalPortLength--){
		externalPortBuffer[externalPortLength]=EEPROM.read(EXTERNAL_PORT_CONFIG+externalPortLength);
	}
	
	if(externalPortOnChange!=EXTERNAL_PORT_SET_COUNTER){
		externalPortLength=EXTERNAL_PORT_LENGTH;
		while(externalPortLength--){
			unsigned char shiftRegisterByte=EEPROM.read(EXTERNAL_PORT_CONFIG+externalPortLength);
			unsigned char bitCounter=8;
			while(bitCounter--){
				_DW(shiftRegisterData,(shiftRegisterByte&(1<<bitCounter)));
				_DW(shiftRegisterClk,1);
				_delay_us(EXTERNAL_OUTPUT_SPEED);
				_DW(shiftRegisterClk,0);
				_delay_us(EXTERNAL_OUTPUT_SPEED);
			}
		}
		_DW(shiftRegisterData,0);

		_DW(shiftRegisterLatch,1);
		_delay_us(EXTERNAL_OUTPUT_SPEED*10);
		_DW(shiftRegisterLatch,0);
		_delay_us(EXTERNAL_OUTPUT_SPEED);
	}
	externalPortOnChange=EXTERNAL_PORT_SET_COUNTER;

	unsigned char *getExternalPortObject=EEPROM_BUFFER;
	const char *constExternalPortObjectStart="{\"externalPort\":[";
	unsigned char *externalPortObject=(unsigned char*)constExternalPortObjectStart;
	while(*externalPortObject){
		*getExternalPortObject=*externalPortObject;
		getExternalPortObject++;
		externalPortObject++;
	}
	externalPortLength=0;
	while(externalPortLength<EXTERNAL_PORT_LENGTH){
		externalPortObject=inttostring(externalPortBuffer[externalPortLength++]);
		while(*externalPortObject){
			*getExternalPortObject=*externalPortObject;
			getExternalPortObject++;
			externalPortObject++;
		}
		*getExternalPortObject=0x2C*(externalPortLength<EXTERNAL_PORT_LENGTH);
		getExternalPortObject+=(externalPortLength<EXTERNAL_PORT_LENGTH);
	}
	*getExternalPortObject=0x5D;
	getExternalPortObject++;
	*getExternalPortObject=0x7D;
	getExternalPortObject++;

	return EEPROM_BUFFER;
}

unsigned char *externalPortSet(unsigned char *externalPortArrayObject){
	unsigned char *orgLocation=externalPortArrayObject;
	unsigned char *finalInt;
	unsigned char arrayIndexBuffer[19]="externalPort[";
	unsigned char *arrayIndexReset=arrayIndexBuffer+stringCounter(arrayIndexBuffer);
	if(json("externalPort",externalPortArrayObject)!=UNDEFINED){
		EXTERNAL_PORT_SET_COUNTER++;
		unsigned short startAddress=EXTERNAL_PORT_CONFIG;
		unsigned char arrayIndex=0;
		while(startAddress<EXTERNAL_PORT_CONFIG+EXTERNAL_PORT_LENGTH){
			unsigned char *mkIndexStr=arrayIndexReset;
			CLR(mkIndexStr);
			unsigned char *arrayIndexStr=inttostring(arrayIndex++);
			while(*arrayIndexStr){
				*mkIndexStr=*arrayIndexStr;
				mkIndexStr++;
				arrayIndexStr++;
			}
			*mkIndexStr=0x5D;
			mkIndexStr++;
			if((finalInt=json(arrayIndexBuffer,externalPortArrayObject))!=UNDEFINED){
				EEPROM.write(startAddress&((MAX_EEPROM_SIZE-1)),(strint(finalInt)&0xff));
			}
			startAddress++;
		}
		EEPROM.commit();
	}
	return orgLocation;
}

unsigned char *externalPortBitToggle(unsigned char bitNumber){
	unsigned char bitValue=1<<(bitNumber%8);
	unsigned char byteIndex=EXTERNAL_PORT_CONFIG+(bitNumber/8)%8;
	EEPROM.write(byteIndex,(EEPROM.read(byteIndex)^bitValue));
	EEPROM.commit();
	EXTERNAL_PORT_SET_COUNTER++;
	return externalPort();
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
		linkerSave(UNDEFINED);
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

unsigned char USER_KEY[21]="";

unsigned char serverConnect(WiFiClient &socket){
	unsigned char *remoteHostObject=linkerData();
	if((json("D",remoteHostObject)!=UNDEFINED)&&(json("P",remoteHostObject)!=UNDEFINED)&&(json("K",remoteHostObject)!=UNDEFINED)&&(json("U",remoteHostObject)!=UNDEFINED)){
		CLR(USER_KEY);
		unsigned char *mkUserKey=USER_KEY;
		unsigned char *getUserKey=base64Decode(json("K",remoteHostObject));
		while(*getUserKey){
			*mkUserKey=*getUserKey;
			mkUserKey++;
			getUserKey++;
		}
		unsigned short socketPort=strint(json("P",remoteHostObject));
		if(!(socket.connect((char*)json("D",remoteHostObject),socketPort))){
			socket.stop();
			consoleLog("failed to connect\n");
			return 0;
		}
		_delay_ms(150);
		socket.write((char*)json("U",remoteHostObject));
		socket.write("new xtensa connection");
		consoleLog("server connected\n");
		return 1;
	}
	consoleLog("invalid network object \n");
	return 0;
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

unsigned char responeseHeaders=0;
#define fecthHeadersEnable() responeseHeaders=1
#define fetchHeadersDisable() responeseHeaders=0

unsigned char *urlEncode(unsigned char *originalUrl){
	const unsigned char urlSpecialChars[23]="-._~:/?#[]@!$&'()*+,;=";
	unsigned char *originalUrlStartLocation=originalUrl;
	while(*originalUrlStartLocation){
		if(!(((*originalUrlStartLocation>0x40)&&(*originalUrlStartLocation<0x5B))||((*originalUrlStartLocation>0x60)&&(*originalUrlStartLocation<0x7B))||((*originalUrlStartLocation>0x2F)&&(*originalUrlStartLocation<0x3A))||((*originalUrlStartLocation==0x25)&&validHex(originalUrlStartLocation+1)&&validHex(originalUrlStartLocation+2)))){
			unsigned char specialCharCounter=sizeof(urlSpecialChars);
			while((*originalUrlStartLocation!=urlSpecialChars[--specialCharCounter])&&specialCharCounter)
			if(*originalUrlStartLocation!=urlSpecialChars[specialCharCounter]){
				unsigned char *urlEncodedValue=intToHexaDecimal(*originalUrlStartLocation)+1;
				urlEncodedValue[0]=0x25;
				unsigned short originalUrlStartLocationLength=stringCounter(originalUrlStartLocation);
				while(originalUrlStartLocationLength--){
					originalUrlStartLocation[originalUrlStartLocationLength+2]=originalUrlStartLocation[originalUrlStartLocationLength];
					originalUrlStartLocation[originalUrlStartLocationLength]=0;
				}
				_CS(originalUrlStartLocation,urlEncodedValue);
			}
		}
		originalUrlStartLocation++;
	}
	return originalUrl;
}

struct httpLink{
	unsigned char domain[40]="";
	unsigned short port;
	unsigned char secure;
	unsigned char userName[30]="";
	unsigned char password[30]="";
	unsigned char authDefined=0;
	unsigned char *requestPath=UNDEFINED;
};

struct httpLink urlBreakDown(unsigned char *httpRequest){
	struct httpLink urlParameters;
	unsigned short httpRequestLength=stringCounter(httpRequest);

	unsigned short requestPathLocatorLimit=httpRequestLength;
	unsigned short requestPathLocator=0;
	unsigned char slashCounter=0;
	while(((slashCounter+=(httpRequest[requestPathLocator++]==0x2F))<3)&&(requestPathLocatorLimit--));
	if(slashCounter!=3)
		urlParameters.requestPath=UNDEFINED;
	else
		urlParameters.requestPath=(httpRequest+requestPathLocator-1);

	urlParameters.secure=(httpRequest[4]==0x73);
	urlParameters.port=80+((443-80)*(httpRequest[4]==0x73));
	unsigned short urlStartLocation=7+urlParameters.secure;

	while((--httpRequestLength)&&(httpRequest[httpRequestLength]!=0x40));
	if(httpRequestLength){			//get basic auth
		urlParameters.authDefined=1;
		unsigned char authCharCounter=0;
		while(httpRequest[urlStartLocation]!=0x3A){	
			urlParameters.userName[authCharCounter++]=httpRequest[urlStartLocation++];
		}
		urlStartLocation++;
		authCharCounter=0;
		while(httpRequest[urlStartLocation]!=0x40){
			urlParameters.password[authCharCounter++]=httpRequest[urlStartLocation++];
		}
		urlStartLocation=httpRequestLength+1;
	}

	unsigned short domainStartLocation=urlStartLocation;
	while((httpRequest[urlStartLocation]!=0x3A)&&(httpRequest[urlStartLocation++]!=0x2F));		//trying to locate : or /
	if(httpRequest[(urlStartLocation)]==0x3A)
		urlParameters.port=strint(httpRequest+urlStartLocation+1);
	unsigned char domainFillCounter=0;
	while(domainStartLocation<(urlStartLocation-=(httpRequest[(urlStartLocation-1)]==0x2F))){
		urlParameters.domain[domainFillCounter++]=httpRequest[domainStartLocation++];
	}
	if(!urlParameters.port)
		urlParameters.port=80+((443-80)*(httpRequest[4]==0x73));
	return urlParameters;
}

unsigned short fetchMemoryLimiter=4069;

unsigned char *fetch(unsigned char *httpRequest,unsigned char *requestBody,unsigned char *responseBuffer){
	unsigned char* returnedBuffer=responseBuffer;
	struct httpLink urlParameters=urlBreakDown(httpRequest);
	unsigned char *requestBodyLocation=_CS(responseBuffer,(unsigned char*)((requestBody==UNDEFINED)?("GET "):("POST ")));
	if(urlParameters.requestPath!=UNDEFINED)
		urlEncode(_CS(requestBodyLocation,urlParameters.requestPath)+4+(requestBody!=UNDEFINED));
	_CS(responseBuffer,(unsigned char*)((urlParameters.requestPath==UNDEFINED)?("/"):("")));
	_CS(responseBuffer,(unsigned char*)" HTTP/1.1\r\n");
	_CS(responseBuffer,(unsigned char*)"Host: ");
	_CS(responseBuffer,urlParameters.domain);
	if(urlParameters.port!=80+((443-80)*(httpRequest[4]==0x73))){
		_CS(responseBuffer,(unsigned char*)":");
		_CS(responseBuffer,inttostring((unsigned long)(urlParameters.port)));
	}
	_CS(responseBuffer,(unsigned char*)"\r\n");
	_CS(responseBuffer,(unsigned char*)"User-Agent: PostmanRuntime/7.29.0\r\n");
	_CS(responseBuffer,(unsigned char*)"Accept: */*\r\n");
	_CS(responseBuffer,(unsigned char*)"Connection: keep-alive\r\n");
	if(requestBody!=UNDEFINED){
		_CS(responseBuffer,(unsigned char*)"Content-Type: application/json\r\n");
		_CS(responseBuffer,(unsigned char*)"Content-Length: ");
		_CS(responseBuffer,(unsigned char*)inttostring(stringCounter(requestBody)));
		_CS(responseBuffer,(unsigned char*)"\r\n\r\n");
		_CS(responseBuffer,requestBody);
		_CS(responseBuffer,(unsigned char*)"\r\n");
	}
	_CS(responseBuffer,(unsigned char*)"\r\n");

	if(urlParameters.secure){
		WiFiClientSecure client;
		client.setInsecure();
		if(!client.connect((char*)urlParameters.domain,urlParameters.port)){
			consoleLog("couldn't connect !!\n");
			return UNDEFINED;
		}
		client.write((char*)responseBuffer);
		CLR(responseBuffer);
		while(!client.available());
		unsigned char *makeStr=responseBuffer;
		unsigned long memoryLimter=0;
		while(client.available()){
			if(memoryLimter<fetchMemoryLimiter){
				*makeStr=client.read();
				makeStr++;
			}
			else{
				client.read();
			}
			memoryLimter++;
		}
		makeStr=responseBuffer;
	}
	else{
		WiFiClient client;
		if(!client.connect((char*)urlParameters.domain,urlParameters.port)){
			consoleLog("couldn't connect !!\n");
			return UNDEFINED;
		}
		client.write((char*)responseBuffer);
		CLR(responseBuffer);
		while(!client.available());
		unsigned char *makeStr=responseBuffer;
		unsigned long memoryLimter=0;
		while(client.available()){
			if(memoryLimter<fetchMemoryLimiter){
				*makeStr=client.read();
				makeStr++;
			}
			else{
				client.read();
			}
			memoryLimter++;
		}
		makeStr=responseBuffer;
	}

	return returnedBuffer;
}



unsigned char nextClient(unsigned char allClients){
	for(unsigned char i=0;i<8;i++){
		if(!(allClients&(1<<i)))
			return i;
	}
	return (-1U);
}

void hw_wdt_disable(){
	*((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF
//	volatile uint32_t *watchDogRegister=((volatile uint32_t*) 0x60000900);
//	*watchDogRegister &= ~(1);
}

void hw_wdt_enable(){
	*((volatile uint32_t*) 0x60000900) |= 1; // Hardware WDT ON
}

void setup() {
	// ESP.wdtDisable();
	hw_wdt_disable();
}

void loop() {
	main();
}


#define REMOTE_HOST "192.168.1.130"
#define REMOTE_PORT 80U
#define GENERAL_RESPONSE "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: *\r\n\r\nSk9ITiBLSEFMSUw=\r\n"
#define CORS_HEADERS (char *)"HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: *\r\n\r\n"
#define CLIENT_ACK "REQUEST_ACK\r\n"

#define EXPORTED_DATA_MAX_SIZE 10240UL
unsigned char EXPORTED_DATA[EXPORTED_DATA_MAX_SIZE]="";


int main(void){
    disableAutoReset();
	eepromInit();
	spiConsoleSetup();
	static unsigned short lastNetworkStat;

	// unsigned char fileSystemMounted=1;
	// if(!fileSystem.begin()){
	// 	fileSystemMounted=0;
	// 	consoleLog("cannot mount FileSystem\r\n");
	// }

	spiConsole console;
	console.setup(clk,data,microSec,360000);
// 
// 
	// console.log("test >> ", 5012);

	

	consoleLog("\nserial port\n");



	const char *ssid="RISC-V";
	const char *password="threadripper";
	ssidSave((unsigned char*)ssid);
	wifiPasswordSave((unsigned char*)password);
	consoleLog(userSSID());
	consoleLog("\nserial port\n");
	consoleLog(userPassword());
	consoleLog("\nserial port\n");
	
	externalPort();
	lastNetworkStat=networkConnect();
	consoleLog(inttostring(lastNetworkStat==WL_CONNECTED));
 	consoleLog(" connected\n");
	unsigned char networkWasConnect=(lastNetworkStat==WL_CONNECTED);

	WiFiServer server(80);
	server.begin();


	WiFiClient socket;
	//socket.connect(REMOTE_HOST,REMOTE_PORT);
	socket.setTimeout(5000);
	// linkerSave((unsigned char*)"{\"D\":\"xtensa32plus.ddns.net\",\"P\":50,\"K\":\"0123456789123456\",\"U\":{\"auth\":\"test0\"}}");
	unsigned char serverConnected=serverConnect(socket);
	uint64_t lastServerTime=0;

	WiFiClient client;
	client.setTimeout(5000);

	#define enc(_RAWTEXT) tcps((unsigned char*)_RAWTEXT,USER_KEY,EXPORTED_DATA)
	unsigned char *_serverData;// not to call a pointer returnning func twice
	#define SERVER_SEND(serverData)_serverData=serverData; if(serverConnected){socket.write((char*)enc(_serverData));dataMask((unsigned char*)_serverData,USER_KEY);CLR(EXPORTED_DATA);}

	#define remoteUserData 0x00010000UL
	#define remoteUserDisconnected 0x00020000UL
	#define portListener 0x00040000UL
	#define clientDisconnected 0x00080000UL

	unsigned long eventListener=0;
	unsigned long staticListener=0;
	unsigned char clientJustConnected=0; //this method might lead to lots of synchronization problems but in a very few conditions
	unsigned char socketConnection=0;
	
	consoleLog("start the loop\n");
	while(1){
		eventListener=0;
		unsigned short reconnectTimer=0;
		#define NETWORK_BUFFER_SIZE 2048
		unsigned char tcpText[NETWORK_BUFFER_SIZE]="";

		while(1){
			//ESP.wdtFeed();
			if(lastNetworkStat!=WiFi.status()){ //network event listener -auto handle
				lastNetworkStat=WiFi.status();
				if(lastNetworkStat==WL_CONNECTED){
					networkWasConnect=1;
					WiFi.softAP((char*)ipAddressString(WiFi.localIP()),(char*)userPassword());
				}
				else if(networkWasConnect){ //open up an access point with the matching ip address 
					networkWasConnect=0;
					WiFi.softAP("",(char*)userPassword());
					WiFi.softAP((char*)ipAddressString(WiFi.softAPIP()),(char*)userPassword());
				}
			}
			if(!clientJustConnected&&(!socketConnection)){//if there was a connection it would tell new connection simply to fuck off
				if(client=server.available()){
					clientJustConnected=1;
				}
			}
			if((clientJustConnected||socketConnection)&&(client.available())){
				clientJustConnected=0;
				eventListener|=portListener;
				staticListener|=portListener;
			}
			if(socketConnection&&(!client.connected())){
				socketConnection=0;
				eventListener|=clientDisconnected;
				staticListener|=clientDisconnected;
			}

			if(socket.available()&&serverConnected){
				tcpText[NETWORK_BUFFER_SIZE-1]=0xFF;
				dataMask(base64Decode(tcpGetString(socket,tcpText)),base64Decode(json("K",linkerData())));
				if(!lastServerTime){
					lastServerTime=getInt(json("time",tcpText));
					CLR(tcpText);
				}
				else if(getInt(json("time",tcpText))>lastServerTime){
					lastServerTime=getInt(json("time",tcpText));

					unsigned char timeDigits=stringCounter(json("time",tcpText))+9;
					CLR(tcpText+(stringCounter(tcpText)-timeDigits));
					unsigned char *orgLoc=tcpText;
					unsigned char *dataLoc=tcpText+8;
					while(*dataLoc){
						*orgLoc=*dataLoc;
						dataLoc++;
						orgLoc++;
					}
					CLR(orgLoc);

					eventListener|=remoteUserData;
					staticListener|=remoteUserData;
				}
				else
					CLR(tcpText);
			}
			if(!(--reconnectTimer)){
				unsigned char *remoteHostObject=linkerData();
				static unsigned char remoteConnectionResetTimer;
				if(!(--remoteConnectionResetTimer)&&serverConnected)
					socket.write("Sk9ITiBLSEFMSUw=");
				if((json("D",remoteHostObject)!=UNDEFINED)&&(json("P",remoteHostObject)!=UNDEFINED)&&(json("K",remoteHostObject)!=UNDEFINED)&&(json("U",remoteHostObject)!=UNDEFINED)){
					if(!serverConnected)
						serverConnected=serverConnect(socket);
					else if(!(serverConnected=socket.connected()))
						consoleLog("server disconnected\n");
						//socket.stop();
				}
			}

			if(eventListener)
				break;
		}




		ALL_EVENTS_HANDLER:
		
		
		unsigned char *exportedData=EXPORTED_DATA;
		CLR(exportedData);

		if(eventListener&portListener){

			unsigned char *makeStr=tcpText;
			unsigned long memoryLimter=0;
			while(client.available()){
				if(memoryLimter<(NETWORK_BUFFER_SIZE-1)){
					*makeStr=client.read();
					makeStr++;
				}
				else{
					client.read();
				}
				memoryLimter++;
			}
			makeStr=tcpText;

			if(socketConnection){
				webSocketDataFrame(tcpText,FRAME_DECODE);
				if(WEBSOCKET_LAST_OPCODE==9){
					unsigned char *webSocketPongFrame=webSocketDataFrame(tcpText,FRAME_ENCODE);
					webSocketPongFrame[0]=0x8A;
					_delay_ms(1);
					client.write((char*)webSocketPongFrame);
					CLR(tcpText);
					CLR(webSocketPongFrame);
					continue;
				}
				//client.write((char*)webSocketDataFrame(tcpText,FRAME_ENCODE));
			}
			else{
				if(!((eventIdentifier(tcpText)&WEB_SERVER)==WEB_SERVER))
					URIdecode(tcpText);
			}
			
			consoleLog(tcpText);
			consoleLog("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");



			unsigned char eventType=eventIdentifier(tcpText);


			if((eventType&WEB_SOCKET)==WEB_SOCKET){// never wanna mess with this line
				consoleLog("user data >> ");
				consoleLog(eventData(EVENT_DATA,tcpText,(unsigned char*)"\r\n"));
				consoleLog("\n");

				socketConnection=1;
				client.write("HTTP/1.1 101 Switching Protocols\r\n");
				client.write("Upgrade: websocket\r\n");
				client.write("Connection: Upgrade\r\n");
				client.write("Sec-WebSocket-Accept: ");
				client.write((char*)secWebSocketAccept(tcpText));
				client.write("\r\n\r\n");
			}

			
			// else if(((eventType&WEB_SERVER)==WEB_SERVER)&&(!socketConnection)){
			// 	unsigned char *filePath=URIdecode(eventData(EVENT_DATA,tcpText,(unsigned char*)" HTTP/"));
			// 	consoleLog("filePath >> ");
			// 	consoleLog(filePath);
			// 	consoleLog(_endl);
				
			// 	if(fileSystemMounted){_WDF;
			// 		client.write(CORS_HEADERS);
			// 		unsigned char *finalFilePath=filePath;
			// 		unsigned char homePageHTML[19]="/oscilloscope.html";
			// 		if(equalStrings(filePath,(unsigned char*)"/")||(!stringCounter(filePath)))
			// 			finalFilePath=homePageHTML;
			// 		File xtensaFile=fileSystem.open((char*)finalFilePath,"r");
			// 		while(1){_WDF;
			// 			unsigned short webPageBuffer=4069;
			// 			unsigned char *mkWebPageBuffer=exportedData;
			// 			while((--webPageBuffer)&&xtensaFile.available()){_WDF;
			// 				*mkWebPageBuffer=(unsigned char)xtensaFile.read();
			// 				mkWebPageBuffer++;
			// 			}
			// 			client.write((char *)exportedData);
			// 			CLR(exportedData);
			// 			if(webPageBuffer)
			// 				break;
			// 		}
			// 		client.flush();
			// 		client.stop();
			// 		xtensaFile.close();
			// 	}
			// 	else{
			// 		client.write(GENERAL_RESPONSE);
			// 		client.flush();
			// 		client.stop();
			// 	}
			// 	CLR(tcpText);
			// 	CLR(filePath);
			// }



			else if((eventType&UPLOADED_DATA)==UPLOADED_DATA){
				#define SCAN_FOR_WIFI 0x00000001UL
				#define NETWORK_DATA 0x00000002UL
				#define LOGIC_ANALIZER 0x00000004UL
				#define SUPER_SPEED_SAMPLE 0x00000008UL
				#define ENABLE_SERIAL_PORT 0x00000010UL
				#define SERVER_DATA 0x00000020UL
				#define EXTERNAL_PORT 0x00000040UL
				#define EXTERNAL_PORT_LIVE 0x00000100UL

				consoleLog("user data >> ");
				consoleLog(eventData(EVENT_DATA,tcpText,(unsigned char*)"<@>"));
				consoleLog("\n");

				unsigned char *instructionDecode=json("instruction",tcpText);
				if(instructionDecode!=UNDEFINED){
					unsigned long userInstruction=strint(instructionDecode);
					consoleLog("user instruction >> ");
					consoleLog(intToHexaDecimal(userInstruction));
					consoleLog("\n");
					
					if(userInstruction&SCAN_FOR_WIFI){
						if(socketConnection){
							client.write((char*)webSocketDataFrame(scanForWifi(exportedData),FRAME_ENCODE));
						}
						else{
							client.write(CORS_HEADERS);
							client.write((char*)scanForWifi(exportedData));
							client.flush();
							client.stop();
						}
						CLR(exportedData);
					}

					if(userInstruction&NETWORK_DATA){
						ssidSave(json("networkSSID",tcpText));
						wifiPasswordSave(json("networkPASS",tcpText));
						if(socketConnection){
							client.write((char*)webSocketDataFrame((unsigned char*)CLIENT_ACK,FRAME_ENCODE));
						}
						else{
							client.write(CORS_HEADERS);
							client.write(CLIENT_ACK);
							client.flush();
							client.stop();
						}
						lastNetworkStat=networkConnect();
						networkWasConnect=(lastNetworkStat==WL_CONNECTED);
					}

					if((userInstruction&SERVER_DATA)&&(!socketConnection)){
						unsigned char *serverConfig;
						client.write(CORS_HEADERS);
						if((serverConfig=json("serverData",tcpText))!=UNDEFINED)
							linkerSave(serverConfig);
						client.write((char*)linkerData());
						client.flush();
						client.stop();
					}

					if(userInstruction&EXTERNAL_PORT){
						unsigned char *checkLastServerTime;
						if((checkLastServerTime=json("lastServerTime",tcpText))!=UNDEFINED){
							uint64_t externalPortInstructionOrder=getInt(checkLastServerTime);
							if(externalPortInstructionOrder!=lastServerTime){
								lastServerTime=externalPortInstructionOrder;
								externalPortSet(tcpText);
								unsigned char *extPrtDirectBit;
								if((extPrtDirectBit=json("extPrtDirectBit",tcpText))!=UNDEFINED){
									externalPortBitToggle(getInt(extPrtDirectBit));
								}
							}
						}
						SERVER_SEND(externalPort());
						if(socketConnection){
							client.write((char*)webSocketDataFrame(externalPort(),FRAME_ENCODE));
						}
						else{
							client.write(CORS_HEADERS);
							client.write((char*)externalPort());
							client.flush();
							client.stop();
						}
					}
					
					if((userInstruction&EXTERNAL_PORT_LIVE)&&socketConnection){

						while(client.connected()){
							if(client.available()){
								unsigned char *makeStr=exportedData;
								unsigned long memoryLimter=0;
								while(client.available()){
									if(memoryLimter<(EXPORTED_DATA_MAX_SIZE-1)){
										*makeStr=client.read();
										makeStr++;
									}
									else{
										client.read();
									}
									memoryLimter++;
								}
								makeStr=exportedData;
								webSocketDataFrame(exportedData,FRAME_DECODE);

								unsigned char *liveExternalPortObject;
								if((liveExternalPortObject=json("liveExternalPort",exportedData))!=UNDEFINED){
									uint64_t liveExternalPort=getInt(liveExternalPortObject);
									unsigned long livePortBitCounter=32;											//limited to 4 bytes
									while(livePortBitCounter--){
										_DW(shiftRegisterData,((liveExternalPort&(1<<livePortBitCounter))!=0));		// !=0 not necessary but it only works that way 
										_DW(shiftRegisterClk,1);
										_delay_us(EXTERNAL_OUTPUT_SPEED);
										_DW(shiftRegisterClk,0);
										_delay_us(EXTERNAL_OUTPUT_SPEED);
									}
									_DW(shiftRegisterData,0);
									_DW(shiftRegisterLatch,1);
									_delay_us(EXTERNAL_OUTPUT_SPEED*10);
									_DW(shiftRegisterLatch,0);
									_delay_us(EXTERNAL_OUTPUT_SPEED);
								}

								else if(json("JRM",exportedData)!=UNDEFINED){
									unsigned char *xtensaJrm=exportedData+9;
									unsigned short clearTextSize=stringCounter(xtensaJrm);
									base64Decode(xtensaJrm);					//JRM is not turing complete
									#define JRM_OPERAND(OPERAND_POINTER) (unsigned long)((xtensaJrm[(OPERAND_POINTER*4)+3]|(xtensaJrm[(OPERAND_POINTER*4)+2]<<8)|(xtensaJrm[(OPERAND_POINTER*4)+1]<<16)|(xtensaJrm[OPERAND_POINTER*4]<<24))&0x3FFFFFFF)
									#define JRM_OPCODE(OPCODE_POINTER) (xtensaJrm[OPCODE_POINTER*4]>>6)
									
									#define _output 		0
									#define _delay 			1
									#define _pointerSet 	2
									#define _endOfStack 	3

									unsigned short stackPointer=0;
									unsigned short stackLimiter=-1;
									while(stackLimiter--){
										unsigned char opCode=JRM_OPCODE(stackPointer);
										unsigned long operand=JRM_OPERAND(stackPointer);
										stackPointer++;
										if(opCode==_output){
											const float JRM_PORT_SPEED=(1e6f/1000000)/2;
											unsigned long livePortBitCounter=30;											//30 bits in total 
											while(livePortBitCounter--){
												_DW(shiftRegisterData,((operand&(1<<livePortBitCounter))!=0));		// !=0 not necessary but it only works that way 
												_DW(shiftRegisterClk,1);
												_delay_us(JRM_PORT_SPEED);
												_DW(shiftRegisterClk,0);
												_delay_us(JRM_PORT_SPEED);
											}
											_DW(shiftRegisterData,0);
											_DW(shiftRegisterLatch,1);
											_delay_us(JRM_PORT_SPEED*10);
											_DW(shiftRegisterLatch,0);
											_delay_us(JRM_PORT_SPEED);
										}
										else if(opCode==_delay){
											_delay_us((operand&255));
										}
										else if(opCode==_pointerSet){
											// gpioStack[stackPointer]&=0xC000FFFFUL;
											// gpioStack[stackPointer]|=((operand>>16)-((operand>>16)!=0))<<16;
											unsigned short countPointerDown=((operand>>16)-((operand>>16)!=0));
											xtensaJrm[(stackPointer-1)*4]=0x80|(0x3f&((countPointerDown)>>8));
											xtensaJrm[((stackPointer-1)*4)+1]=countPointerDown&0xFF;
											if((operand>>16))
												stackPointer=(unsigned short)(operand&0xFFFF);
										}
										else if(opCode==_endOfStack){
											if(!operand)
												break;
											else{
												xtensaJrm[(stackPointer-2)*4]=xtensaJrm[(stackPointer-1)*4]&0xBF;
												xtensaJrm[((stackPointer-2)*4)+1]=xtensaJrm[((stackPointer-1)*4)+1];
												xtensaJrm[((stackPointer-2)*4)+2]=xtensaJrm[((stackPointer-1)*4)+2];
												xtensaJrm[((stackPointer-2)*4)+3]=xtensaJrm[((stackPointer-1)*4)+3];
											}
										}

										static unsigned char clientCallTimeOut;
										if(!(--clientCallTimeOut)){
											if(client.available())
												break;
											if(!(client.connected()))
												break;
										}


									}

									CLR_LENGTH=clearTextSize;
									CLR(xtensaJrm);_NOP();

								}

								// #define stackIdentifier "S"
								// else if((liveExternalPortObject=json(stackIdentifier,tcpText))!=UNDEFINED){WD_FEED;
								// 	unsigned short stackPointer=0;
								// 	unsigned char stackIdentifierList[20]=stackIdentifier;
								// 	stackIdentifierList[stringCounter(stackIdentifierList)]=0x5B;	//square bracket


								// }
								// CLR_LENGTH=EXPORTED_DATA_MAX_SIZE;
								CLR(exportedData);
							}
						}
						socketConnection=0;

						
					}

				}	
			}




			else if(!socketConnection){
				client.write(GENERAL_RESPONSE);
				client.flush();
				client.stop();
			}
			


			
		}

		if(eventListener&=remoteUserData){
			consoleLog("DATA FROM SERVER >> ");
			consoleLog(tcpText);
			consoleLog("\r\n");

			#define SERVRE_EXTERNAL_PORT 0x00000040UL
			#define SERVER_SYNC_EXTERNAL_PORT 0x00000080UL

			unsigned char *serverInstructionDecode;
			if((serverInstructionDecode=json("instruction",tcpText))!=UNDEFINED){
				unsigned long serverInstruction=getInt(serverInstructionDecode);

				if(serverInstruction&SERVRE_EXTERNAL_PORT){
					externalPortSet(tcpText);
					unsigned char *extPrtDirectBit;
					if((extPrtDirectBit=json("extPrtDirectBit",tcpText))!=UNDEFINED){
						SERVER_SEND(externalPortBitToggle(getInt(extPrtDirectBit)));		
					}
					else
						SERVER_SEND(externalPort());
				}

				if(serverInstruction&SERVER_SYNC_EXTERNAL_PORT){
					unsigned char *localPortValue=json("externalPort",externalPort());
					CLR(exportedData);
					unsigned char *localPortValueCopy=exportedData;
					while(*localPortValue){
						*localPortValueCopy=*localPortValue;
						localPortValue++;
						localPortValueCopy++;
					}
					if(!equalStrings(exportedData,json("externalPort",tcpText))){
						externalPortSet(tcpText);
						externalPort();
					}
					CLR(exportedData);
				}
			}
		}



		
	}
}