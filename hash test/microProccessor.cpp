#include <stdint.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Hash.h>

#include "extendedIO.cpp"
#include "spiConsole.cpp"
#include "HCSR04.cpp"



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


void  TimerHandler(){
	ESP.wdtFeed();
}
#define disableAutoReset() ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler)
#define enableInt() //interruptEnable=1
#define disableInt() //interruptEnable=0


#define inputRegister (*((volatile uint16_t*)0x60000318))

#define inputRegisterHigh (*((volatile uint8_t*)0x60000319))
#define inputRegisterLow (*((volatile uint8_t*)0x60000318))
// volatile uint16_t *inputRegister = (uint16_t*) 0x60000318;
volatile uint16_t *outputRegister=(uint16_t*)0x60000300;

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

		while(*jsonObject){ //start searching
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
		unsigned char *endLocation=originalLoaction+frameLength;
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






// #define consoleLog(inputStringToConsole) spiConsoleLog((unsigned char *)inputStringToConsole)
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

extendedIO extPort;

#define externalPortClk 5
#define externalPortData 4 
#define externalPortLatchWrite 14
#define externalPortLatchRead 12

#define externalPortWidth 32
#define externalPortSpeed 5e6

void externalPortClkSet(unsigned char state){
	_DW(externalPortClk,(state!=0));
}

void externalPortLatchWriteSet(unsigned char state){
	_DW(externalPortLatchWrite,(state!=0));
}

void externalPortLatchReadSet(unsigned char state){
	_DW(externalPortClk,(state!=0));
}

void externalPortDataWrite(unsigned char state){
	_DW(externalPortData,(state!=0));
}

unsigned char externalPortDataRead(void){
	return _DR(externalPortData);
}

void setDataOutput(unsigned char state){
	if(!state)
		_PM(externalPortData,INPUT_PULLUP);
	else
		_PM(externalPortData,OUTPUT);
}

void externalPortClassSetup(void){
	_PM(externalPortClk,OUTPUT);
	_PM(externalPortData,OUTPUT);
	_PM(externalPortLatchWrite,OUTPUT);
	_PM(externalPortLatchRead,OUTPUT);
	
	externalPortClkSet(0);
	externalPortDataWrite(0);
	externalPortLatchWriteSet(0);
	externalPortLatchReadSet(0);

	extPort.setup(externalPortClkSet,externalPortLatchWriteSet,externalPortLatchReadSet,externalPortDataWrite,externalPortDataRead,setDataOutput,externalPortWidth,microSec,externalPortSpeed);
}

#define extPortData 5
#define extPortClk 2
#define extPortLatch 4

void extenalPortSetup(void){
	_PM(extPortData,OUTPUT);
	_PM(extPortClk,OUTPUT);
	_PM(extPortLatch,OUTPUT);

	_DW(extPortData,0);
	_DW(extPortClk,0);
	_DW(extPortLatch,0);
}

// unsigned long externalPort(unsigned long externalPortValue){
// 	unsigned char portBitCounter=32;
// 	while(portBitCounter--){
// 		_DW(extPortData,(externalPortValue>>portBitCounter)&0x01);
// 		_DW(extPortClk,1);
// 		_DW(extPortClk,0);
// 	}
// 	_DW(extPortData,0);
// 	_DW(extPortLatch,1);
// 	_DW(extPortLatch,0);
// 	return externalPortValue;
// }

const float clkSpeed=1e6f/5e6f;

unsigned long EXTERNAL_PORT_LAST_VALUE=0;
unsigned long externalPort(unsigned long externalPortValue){
	EXTERNAL_PORT_LAST_VALUE=externalPortValue;
	unsigned char portBitCounter=32;
	while(portBitCounter--){
		_DW(extPortData,(externalPortValue>>portBitCounter)&0x01);
		*outputRegister^=(1<<extPortClk);
		_delay_us(clkSpeed);
		*outputRegister^=(1<<extPortClk);
		_delay_us(clkSpeed);
	}
	*outputRegister^=(1<<extPortLatch);
	_delay_us(clkSpeed);
	*outputRegister^=(1<<extPortLatch);
	return externalPortValue;
}

unsigned long externalPort(void){
	return EXTERNAL_PORT_LAST_VALUE;
}

#define hcsr04TrigPin 13
#define hcsr04EchoPin 12

void trigPin(unsigned char state){
	if(!state)
		*outputRegister&=~(1<<hcsr04TrigPin);
	else
		*outputRegister|=(1<<hcsr04TrigPin);
}

unsigned char echoPin(void){
	return (inputRegister>>hcsr04EchoPin)&0x01;
}

hcsr04 distanceSensor;

void hcsr04Setup(void){
	_PM(hcsr04TrigPin,OUTPUT);
	_PM(hcsr04EchoPin,INPUT_PULLUP);
	_DW(hcsr04TrigPin,0);
	distanceSensor.setup(trigPin,echoPin,microSec,0.8);								// this 0.8 calibration factor was clibrated by measuring a known distance 
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
	extenalPortSetup();
	hcsr04Setup();
	static unsigned short lastNetworkStat;
	_PM(14,OUTPUT);															
	#define speedControl(tenBitValue) analogWrite(14,tenBitValue) 			//pin 14 is where the mosfet is connected

	spiConsole console;
	console.setup(clk,data,microSec,360000);



	externalPort(0);
	speedControl(0);

	console.log("serial port");



	const char *ssid="RISC-V";
	const char *password="threadripper";
	ssidSave((unsigned char*)ssid);
	wifiPasswordSave((unsigned char*)password);
	console.log(userSSID());
	console.log("serial port");
	console.log(userPassword());
	console.log("serial port");
	
	lastNetworkStat=networkConnect();
	console.log("connected >> ",(lastNetworkStat==WL_CONNECTED));
 	
	unsigned char networkWasConnect=(lastNetworkStat==WL_CONNECTED);

	WiFiServer server(80);
	server.begin();


	WiFiClient client;
	client.setTimeout(5000);


	#define portListener 0x00040000UL
	#define clientDisconnected 0x00080000UL

	unsigned long eventListener=0;
	unsigned long staticListener=0;
	unsigned char clientJustConnected=0; //this method might lead to lots of synchronization problems but in a very few conditions
	unsigned char socketConnection=0;
	
	console.log("start the loop");
	while(1){
		eventListener=0;
		unsigned short reconnectTimer=0;
		#define NETWORK_BUFFER_SIZE 2048
		unsigned char tcpText[NETWORK_BUFFER_SIZE]="";

		while(1){
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
			if(!clientJustConnected&&(!socketConnection)){
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
			}
			else{
				if(!((eventIdentifier(tcpText)&WEB_SERVER)==WEB_SERVER))
					URIdecode(tcpText);
			}
			
			console.log(tcpText);
			console.log("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");



			unsigned char eventType=eventIdentifier(tcpText);


			if((eventType&WEB_SOCKET)==WEB_SOCKET){
				console.log("user data >> ",eventData(EVENT_DATA,tcpText,(unsigned char*)"\r\n"));
				
				socketConnection=1;
				client.write("HTTP/1.1 101 Switching Protocols\r\n");
				client.write("Upgrade: websocket\r\n");
				client.write("Connection: Upgrade\r\n");
				client.write("Sec-WebSocket-Accept: ");
				client.write((char*)secWebSocketAccept(tcpText));
				client.write("\r\n\r\n");
			}

			else if((eventType&UPLOADED_DATA)==UPLOADED_DATA){
				#define SCAN_FOR_WIFI 0x00000001UL
				#define NETWORK_DATA 0x00000002UL
				#define vehicleControl 0x00000004UL

				console.log("user data >> ",eventData(EVENT_DATA,tcpText,(unsigned char*)"<@>"));
				

				unsigned char *instructionDecode=json("instruction",tcpText);
				if(instructionDecode!=UNDEFINED){
					unsigned long userInstruction=strint(instructionDecode);
					console.log("user instruction >> ",intToHexaDecimal(userInstruction));
					
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

					if((userInstruction&vehicleControl)&&socketConnection){
						
						// console.log("request object >> ",_CS(exportedData,json("requestObject",tcpText)));
						unsigned char *lastOjectFound=UNDEFINED;
						static unsigned short maxValue=120;
						maxValue=strint(lastOjectFound=json("requestObject.maxValue",tcpText))|((maxValue)*(lastOjectFound==UNDEFINED));
						unsigned short midPoint=maxValue/2;
						
						static unsigned short userSelectedStartPoint=400;
						userSelectedStartPoint=strint(lastOjectFound=json("requestObject.userSelectedStartPoint",tcpText))|((userSelectedStartPoint)*(lastOjectFound==UNDEFINED));


						#define undefinedValue midPoint//((unsigned short) -1) 							//65535
						#define setControlPin_sth_midPoint(controlPinValue,userSelectedValue) (controlPinValue*(userSelectedValue<midPoint))//((controlPinValue*(userSelectedValue<midPoint))*(userSelectedValue!=undefinedValue))|((controlPinValue&externalPort())*(userSelectedValue==undefinedValue))
						#define setControlPin_gth_midPoint(controlPinValue,userSelectedValue) (controlPinValue*(userSelectedValue>midPoint))//((controlPinValue*(userSelectedValue>midPoint))*(userSelectedValue!=undefinedValue))|((controlPinValue&externalPort())*(userSelectedValue==undefinedValue))

						#define maxMidPoint(userSelectedValue) (((midPoint-userSelectedValue)*(userSelectedValue<midPoint))|((userSelectedValue-midPoint)*(userSelectedValue>midPoint)))
						#define MIN(x,y) ((x<y)?x:y)
						#define minNotZero(operand1,operand2) ((MIN(operand1,operand2)==0)?operand1:MIN(operand1,operand2))

						#define forwardControlPin			(1<<11)					//setting every function to its crosponding pin on the shift register
						#define backwardControlPin			(1<<10)					//setting every function to its crosponding pin on the shift register
						#define leftControlPin			 	(1<<9)					//setting every function to its crosponding pin on the shift register
						#define rightControlPin				(1<<8)					//setting every function to its crosponding pin on the shift register
						#define armLeftControlPin			(1<<7)					//setting every function to its crosponding pin on the shift register
						#define armRightControlPin			(1<<6)					//setting every function to its crosponding pin on the shift register
						#define armUpControlPin				(1<<5)					//setting every function to its crosponding pin on the shift register
						#define armDownControlPin			(1<<4)					//setting every function to its crosponding pin on the shift register
						#define fireControlPin			 	(1<<12)					//setting every function to its crosponding pin on the shift register
						#define laserPin 					(1<<13)					//setting every function to its crosponding pin on the shift register

						unsigned short vehicleControl_X=strint(lastOjectFound=json("requestObject.x",tcpText))|((undefinedValue)*(lastOjectFound==UNDEFINED));
						unsigned short vehicleControl_Y=strint(lastOjectFound=json("requestObject.y",tcpText))|((undefinedValue)*(lastOjectFound==UNDEFINED));
						unsigned short armControl_X=strint(lastOjectFound=json("requestObject.armControl_x",tcpText))|((undefinedValue)*(lastOjectFound==UNDEFINED));
						unsigned short armControl_Y=strint(lastOjectFound=json("requestObject.armControl_y",tcpText))|((undefinedValue)*(lastOjectFound==UNDEFINED));
						unsigned short fireControl=strint(lastOjectFound=json("requestObject.fire",tcpText))|((0)*(lastOjectFound==UNDEFINED));
						unsigned short getDistance=strint(lastOjectFound=json("requestObject.getDistance",tcpText))|((0)*(lastOjectFound==UNDEFINED));
						unsigned short laser=strint(lastOjectFound=json("requestObject.laserOn",tcpText))|(((externalPort()&laserPin)!=0)*(lastOjectFound==UNDEFINED));

						unsigned short outputAccumulator=0;

						outputAccumulator|=setControlPin_sth_midPoint(forwardControlPin,vehicleControl_Y);
						outputAccumulator|=setControlPin_gth_midPoint(backwardControlPin,vehicleControl_Y);
						outputAccumulator|=setControlPin_sth_midPoint(leftControlPin,vehicleControl_X);
						outputAccumulator|=setControlPin_gth_midPoint(rightControlPin,vehicleControl_X);
						outputAccumulator|=setControlPin_sth_midPoint(armLeftControlPin,armControl_Y);
						outputAccumulator|=setControlPin_gth_midPoint(armRightControlPin,armControl_Y);
						outputAccumulator|=setControlPin_sth_midPoint(armUpControlPin,armControl_X );
						outputAccumulator|=setControlPin_gth_midPoint(armDownControlPin,armControl_X );

						outputAccumulator|=fireControlPin*(fireControl==1);
						outputAccumulator|=laserPin*(laser==1);


						
						externalPort(outputAccumulator);

						vehicleControl_X=maxMidPoint(vehicleControl_X);
						vehicleControl_Y=maxMidPoint(vehicleControl_Y);
						armControl_X=maxMidPoint(armControl_X);
						armControl_Y=maxMidPoint(armControl_Y);

						unsigned short maxSpeed=-1;
						maxSpeed=minNotZero(maxSpeed,vehicleControl_X);
						maxSpeed=minNotZero(maxSpeed,vehicleControl_Y);
						maxSpeed=minNotZero(maxSpeed,armControl_X);
						maxSpeed=minNotZero(maxSpeed,armControl_Y);

						maxSpeed*=(maxSpeed!=((unsigned short)-1));
						


						speedControl(userSelectedStartPoint+((1023-userSelectedStartPoint)*((float)maxSpeed/(float)midPoint)));

						if(getDistance){
							CLR(exportedData);
							_CS(exportedData,(unsigned char*)"{\"distance\":");
							_CS(exportedData,inttostring(distanceSensor.getDistance()));
							_CS(exportedData,(unsigned char*)"}");
							client.write((char*)webSocketDataFrame(exportedData,FRAME_ENCODE));
							CLR(exportedData);
						}
							

						// console.log(externalPort());


						// console.log(vehicleControl_X);
						// console.log(vehicleControl_Y);
						// console.log(armControl_X);
						// console.log(armControl_Y);
						// console.log(fireControl);


					}
					
					

				}	
			}




			else if(!socketConnection){
				client.write(GENERAL_RESPONSE);
				client.flush();
				client.stop();
			}
			


			
		}
		else if(eventListener&clientDisconnected){
			externalPort(0);
			speedControl(0);
		}
	}
}