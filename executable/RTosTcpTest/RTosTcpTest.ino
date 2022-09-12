#include <stdint.h>
#include <algorithm>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include "SHA1_HASH.h"
#include "EEPROM.h"
#include "spiConsoleSync.cpp"
#include <functional>
#include <vector>

#include "consoleLogger.h"

#include "esp_task_wdt.h"

// typedef unsigned long loopVar;

// loopVar _LOOP_COUNTER_ = 0;
// loopVar *_NESTED_LOOPS_= (loopVar*)malloc(1*sizeof(loopVar));
// unsigned char _NESTED_LOOPS_COUNTER_ = 0;
// #define within(_LOOP_ITERATIONS_,_LOOP_BODY_) _NESTED_LOOPS_[_NESTED_LOOPS_COUNTER_++]=_LOOP_COUNTER_; _NESTED_LOOPS_=(loopVar *)realloc(_NESTED_LOOPS_,(_NESTED_LOOPS_COUNTER_+1) *sizeof(loopVar));_LOOP_COUNTER_=_LOOP_ITERATIONS_;while(_LOOP_COUNTER_--) _LOOP_BODY_; _LOOP_COUNTER_=_NESTED_LOOPS_[--_NESTED_LOOPS_COUNTER_];  _NESTED_LOOPS_=(loopVar *)realloc(_NESTED_LOOPS_, (_NESTED_LOOPS_COUNTER_+1) * sizeof(loopVar));

#define lengthInBytes(object) (unsigned char *)(&object+1)-(unsigned char *)(&object)



template <typename T,typename FUNC>
void withinTemplate(T loopIterations,FUNC loopCAllBack) {
	unsigned long loopCounter = loopIterations;	//just in case it was passed a const
	while (loopCounter--) {
		//we can do any other thing whithin this loop to do it sort of in parallel 
		loopCAllBack();
	}
}


template<typename T,typename FUNC>
void duringTemplate(T loopIterations,FUNC loopCAllBack) {
	unsigned long loopCounter = loopIterations;	//just in case it was passed a const
	while (loopCounter--) {
		//we can do any other thing whithin this loop to do it sort of in parallel 
		loopCAllBack((loopIterations - (loopCounter + 1)));
	}
}

typedef unsigned long argLoop;

#define within(_LOOP_ITERATIONS_,_LOOP_BODY_) withinTemplate <const unsigned long,const std::function<void(void)>> (_LOOP_ITERATIONS_,[&](void){_LOOP_BODY_})			//every thing is passed by ref 
#define during(_LOOP_ITERATIONS_,_LOOP_BODY_) duringTemplate <const unsigned long,const std::function<void(unsigned long)>> (_LOOP_ITERATIONS_,[&]_LOOP_BODY_)			//every thing is passed by ref






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

std::vector<std::function<void(unsigned char*,unsigned char)>>consoleFeedback;

void consoleSetup(void){
    _PM(consoleClkPin,OUTPUT);
    _PM(consoleDataPin,INPUT_PULLUP);					// this should be defined as OUTPUT but its handeled internally by the class
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

		[&](unsigned char pinState){
            if(pinState)
				_PM(consoleClkPin,OUTPUT);
			else
				_PM(consoleClkPin,INPUT_PULLUP);
        },
		[&](unsigned char pinState){
            if(pinState)
				_PM(consoleDataPin,OUTPUT);
			else
				_PM(consoleDataPin,INPUT_PULLUP);
        },
		[&](void){
            return ((inputRegisterLow>>consoleClkPin)&0x01);
        },
		[&](void){
            return ((inputRegisterLow>>consoleDataPin)&0x01);
        },
		[&](unsigned char*dataFromConsole,unsigned char autoNLCR){
			during(consoleFeedback.size(),(argLoop callbackIndex){
				consoleFeedback[callbackIndex](dataFromConsole,autoNLCR);
			});
		},
        250000
    );
}




unsigned char *sha1Hash(unsigned char *rawData){
	Sha1.init();
	Sha1.print((char*)rawData);
	return Sha1.result();
}

#define endTask() vTaskDelete(NULL)













unsigned char *_$StrLastUsedStr=(unsigned char*)malloc(1);
unsigned char* _$Str(const char* basicStr){
	free(_$StrLastUsedStr);
	unsigned short basicStrLength=stringCounter((unsigned char*)basicStr)+1;		//better not to calculate it twice !!
	_$StrLastUsedStr=(unsigned char*)calloc(basicStrLength,sizeof(unsigned char));
	_CS(_$StrLastUsedStr,(unsigned char*)basicStr);
	return _$StrLastUsedStr;
}

unsigned char* _$Str(unsigned char* basicStr){
	return basicStr;
}

unsigned char* _$Str(char* basicStr){
	return (unsigned char*)basicStr;
}



unsigned char* _$Str(unsigned long num){
	return inttostring(num);
}

unsigned char* _$Str(unsigned short num){
	return inttostring(num);
}

unsigned char* _$Str(unsigned char num){
	return inttostring(num);
}

unsigned char* _$Str(long num){
	return longToString(num);
}

unsigned char* _$Str(short num){
	return longToString(num);
}

unsigned char* _$Str(char num){
	return longToString(num);
}


unsigned char* _$Str(int num){
	return longToString(num);
}

unsigned char *_$Str(double num){
	#define extraDigits 5
	const float decimalPlace=1e5f;
	unsigned char *biggerNumber=longToString(num*decimalPlace);
	unsigned char biggerNumberCharCount=0;
	while(biggerNumber[biggerNumberCharCount++]);
	// biggerNumberCharCount--;                                                             //not sure why this is cancelled
	unsigned char decimalPointIndex=(--biggerNumberCharCount)-extraDigits;
	unsigned char endsWithZero=1;
	while((biggerNumberCharCount--)-decimalPointIndex){
		biggerNumber[biggerNumberCharCount+1]=biggerNumber[biggerNumberCharCount];
		if(endsWithZero){
			endsWithZero=!(biggerNumber[biggerNumberCharCount]-0x30);
			biggerNumber[biggerNumberCharCount+1]*=(biggerNumber[biggerNumberCharCount+1]!=0x30);
		}
	}    
	biggerNumber[decimalPointIndex]=0x2E;                                                   //finally adding the decimal point
	if(!biggerNumber[decimalPointIndex+1])
		biggerNumber[decimalPointIndex+1]=0x30;
	return biggerNumber;
}







unsigned long globalVariadicStringCounter=0;
unsigned long variadicStringCounter(void){
	return globalVariadicStringCounter;
}
template<typename T,typename... Types>
unsigned long variadicStringCounter(T strArg,Types... str2){

	unsigned char* str1=_$Str(strArg);
	globalVariadicStringCounter+=stringCounter(str1);

	return variadicStringCounter(str2...);
}




unsigned char *_$CS=(unsigned char*)malloc(1);
unsigned char _CSS_FirstTimeRunning=1;


unsigned char* $(void){
	_CSS_FirstTimeRunning=1;
	return _$CS;
}



template<typename T,typename... Types>
unsigned char* $(T strArg,Types... str2){
	if(_CSS_FirstTimeRunning){
		globalVariadicStringCounter=variadicStringCounter(str2...)+stringCounter(_$Str(strArg))+1;
		free(_$CS);
		_$CS=(unsigned char*)calloc(globalVariadicStringCounter,sizeof(unsigned char));
		_CSS_FirstTimeRunning=0;
	}
	unsigned char* str1=_$Str(strArg);
	_CS(_$CS,str1);
	return $(str2...);
}


#define CACHE_BYTES(CACHED_DATA) (CACHED_DATA=_CS(((unsigned char*)calloc(stringCounter(CACHED_DATA)+1,sizeof(unsigned char))),CACHED_DATA))




unsigned char included(unsigned char singleChar,unsigned char *targetStr){
	unsigned short targetStrCounter=0;
	while((singleChar!=targetStr[targetStrCounter++])&&targetStr[targetStrCounter-1]);	//that way its more consistant
	return (singleChar==targetStr[targetStrCounter-1]);
}








//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    LEGACY-CODE    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

unsigned char UNDEFINED[10]="undefined";

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

unsigned long getInt32_t(unsigned char *numStr){
	unsigned char numLength=stringCounter(numStr);
	unsigned long result=0;
	unsigned long factor=1;
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
	while((stringPointer1[--diffCounter]==stringPointer2[diffCounter])&&diffCounter);
	return (diffCounter==0);
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


unsigned char *JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=NULL;
unsigned char *JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND=NULL;

unsigned char JSON_LOW_MEMORY_USAGE(unsigned char *requestedJSON,unsigned char *jsonString){		//this has to be only used with one object to avoid memory corruption  
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

	JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=deadEndOfString;
	JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND=jsonString;


	// while(jsonString<deadEndOfString){
	// 	*objectString=*jsonString;
	// 	objectString++;
	// 	jsonString++;
	// }
	// while(*objectString){
	// 	*objectString=0;
	// 	objectString++;
	// }
	return 1;
}

unsigned char *LAST_CONST_JSON_OBJECT;
unsigned char lastByteRemoved=0;
unsigned char *_constJson(unsigned char *requestedJSON,unsigned char *jsonString){
	// static unsigned char lastByteRemoved;
	LAST_CONST_JSON_OBJECT=jsonString;
	if(lastByteRemoved&&(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR!=NULL)&&(!(*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR)))
		*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=lastByteRemoved;		//lucky for us cpp support pointer arthematic
	unsigned char objectDefined=JSON_LOW_MEMORY_USAGE(requestedJSON,jsonString);
	while(*JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND==0x20)JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND++;		//when you make a simple algorithm it really pays off
	JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR-=(*JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND==0x22);			//when you make a simple algorithm it really pays off
	JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND+=(*JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND==0x22);			//when you make a simple algorithm it really pays off
	lastByteRemoved=*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR;
	*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=0;
	if(!objectDefined)
		return UNDEFINED;
	return JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND;
}

#define clearConstJsonBuffer() *JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=1; CLR(LAST_CONST_JSON_OBJECT);

#define constJsonReset() *JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=lastByteRemoved
#define constJsonResetUndo() *JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=0

#define constJson(REQUESTED_JSON,JSON_STRING) _constJson((unsigned char*)REQUESTED_JSON,JSON_STRING)

unsigned char *constJsonValidate(unsigned char *jsonString){
	constJsonReset();
	unsigned char *validJsonString=jsonString;
	unsigned char *deadEndOfString=jsonString+stringCounter(jsonString);
	unsigned short jsonArrayIndex=-1;
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
			// constJsonResetUndo();
			return validJsonString;
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
				// constJsonResetUndo();
				return validJsonString;
			}
			jsonArrayIndex--;
		}
		objectTypeArray=0;
		jsonArrayIndex=-1;
		goto searchInsideArray;
	}

	JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR=deadEndOfString;

	lastByteRemoved=*deadEndOfString;
	*deadEndOfString=0;

	return validJsonString;
}

unsigned char *recursiveJsonAlgorithm(unsigned char *recursiveAlgorithmData){
	constJsonReset();
	return recursiveAlgorithmData;
}


unsigned char *makeJsonObject(unsigned char **objectKeys,unsigned char **objectValues){
	unsigned long finalObjectSize=3;													// taking acount for {} and \0
	static unsigned char *finalObject=NULL;
	if(finalObject!=NULL)
		free(finalObject);
	unsigned long objectKeysCounter=0;
	unsigned long objectValuesCounter=0;
	while(objectKeys[objectKeysCounter++]!=NULL){			// following the rules of ecma script skipping undefined objects
		if(objectValues[objectValuesCounter]!=UNDEFINED)
			finalObjectSize+=stringCounter(objectKeys[objectKeysCounter-1])+stringCounter((objectValues[objectValuesCounter]==NULL)? (unsigned char*)"null" : objectValues[objectValuesCounter++])+4;				// taking acount for the "":,
		else
			objectValuesCounter++;
	}	
	finalObject=(unsigned char*)calloc(finalObjectSize,sizeof(unsigned char));
	_CS(finalObject,(unsigned char*)"{");
	objectKeysCounter=0;
	objectValuesCounter=0;
	while(objectKeys[objectKeysCounter++]!=NULL){			// following the rules of ecma script skipping undefined objects
		if(objectValues[objectValuesCounter]!=UNDEFINED){
			_CS(finalObject,(unsigned char*)"\"");
			_CS(finalObject,objectKeys[objectKeysCounter-1]);
			_CS(finalObject,(unsigned char*)"\":");
			_CS(finalObject,((objectValues[objectValuesCounter]==NULL)? (unsigned char*)"null" : objectValues[objectValuesCounter++]));
			_CS(finalObject,(unsigned char*)",");
		}
		else
			objectValuesCounter++;
	}
	finalObject[stringCounter(finalObject)-1]=0x7D;
	return finalObject;
}

unsigned char **makeJsonObjectKeys=NULL;
unsigned char **makeJsonObjectValues=NULL;

unsigned char jsonObjectKeysInitializer=0;
unsigned short jsonObjectKeysCounter=0;
unsigned char **jsonObjectKeys(void){
	jsonObjectKeysInitializer=0;
	jsonObjectKeysCounter=0;
	return makeJsonObjectKeys;
}

template<typename T,typename... Types>
unsigned char **jsonObjectKeys(T key,Types... keys){
	if(!jsonObjectKeysInitializer){
		jsonObjectKeysInitializer=1;
		if(makeJsonObjectKeys!=NULL)
			free(makeJsonObjectKeys);
		makeJsonObjectKeys=(unsigned char **)calloc((sizeof...(Types)+2),sizeof(unsigned char*));
	}
	makeJsonObjectKeys[jsonObjectKeysCounter++]=(unsigned char*)key;
	return jsonObjectKeys(keys...);
}

unsigned char jsonObjectValuesInitializer=0;
unsigned short jsonObjectValuesCounter=0;
unsigned char **jsonObjectValues(void){
	jsonObjectValuesInitializer=0;
	jsonObjectValuesCounter=0;
	return makeJsonObjectValues;
}

template<typename T,typename... Types>
unsigned char **jsonObjectValues(T value,Types... values){
	if(!jsonObjectValuesInitializer){
		jsonObjectValuesInitializer=1;
		if(makeJsonObjectValues!=NULL)
			free(makeJsonObjectValues);
		makeJsonObjectValues=(unsigned char **)calloc((sizeof...(Types)+2),sizeof(unsigned char*));
	}
	makeJsonObjectValues[jsonObjectValuesCounter++]=(unsigned char*)value;
	return jsonObjectValues(values...);
}

#define JSON_KEYS jsonObjectKeys
#define JSON_VALUES jsonObjectValues

class JSON_PARSER{
	public:
	unsigned char *JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT=NULL;
	unsigned char *JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT=NULL;

	unsigned char JSON_LOW_MEMORY_USAGE_OBJECT(unsigned char *requestedJSON,unsigned char *jsonString){		//this has to be only used with one object to avoid memory corruption  
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

		JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT=deadEndOfString;
		JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT=jsonString;


		// while(jsonString<deadEndOfString){
		// 	*objectString=*jsonString;
		// 	objectString++;
		// 	jsonString++;
		// }
		// while(*objectString){
		// 	*objectString=0;
		// 	objectString++;
		// }
		return 1;
	}


	unsigned char *LAST_CONST_JSON_OBJECT_OBJECT;
	unsigned char lastByteRemoved_OBJECT=0;
	unsigned char *parse(unsigned char *requestedJSON,unsigned char *jsonString){
		// static unsigned char lastByteRemoved_OBJECT;
		LAST_CONST_JSON_OBJECT_OBJECT=jsonString;
		if(lastByteRemoved_OBJECT&&(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT!=NULL)&&(!(*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT)))
			*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT=lastByteRemoved_OBJECT;		//lucky for us cpp support pointer arthematic
		unsigned char objectDefined=JSON_LOW_MEMORY_USAGE(requestedJSON,jsonString);
		while(*JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT==0x20)JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT++;		//when you make a simple algorithm it really pays off
		JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT-=(*JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT==0x22);			//when you make a simple algorithm it really pays off
		JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT+=(*JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT==0x22);			//when you make a simple algorithm it really pays off
		lastByteRemoved_OBJECT=*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT;
		*JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR_OBJECT=0;
		if(!objectDefined)
			return UNDEFINED;
		return JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND_OBJECT;
	}

	unsigned char *parse(char *requestedJson,unsigned char *jsonString){
		return parse((unsigned char*)requestedJson,jsonString);
	}

};












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
	// unsigned char buffer1[21]="";
    // sha1((char*)clientBase64,&buffer1[0]);
	unsigned char* buffer1=sha1Hash(clientBase64);
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












//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    LEGACY-CODE    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


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

#define LINKER_CONFIG WIFI_CONFIG+SSID_MAX_LENGTH+WIFI_PASSWORD_MAX_LENGTH
#define LINKER_CONFIG_LENGTH 100

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
			console.log("failed to connect");
			return 0;
		}
		_delay_ms(150);
		socket.write((char*)json("U",remoteHostObject));
		socket.write("new xtensa connection");
		console.log("server connected");
		return 1;
	}
	console.log("invalid network object");
	return 0;
}


#define USER_REQUEST 0x01
#define WEB_SOCKET 0x02
#define UPLOADED_DATA 0x04
#define WEB_SERVER 0x08
unsigned char *EVENT_DATA;



unsigned char eventIdentifier(unsigned char *userRequest){
	unsigned char *orginalRequest=userRequest;
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
	if(!(*userRequest)){
		userRequest=orginalRequest;
		unsigned char *headersEnding=(unsigned char*)"\r\n\r\n";
		unsigned short dataLocationChecker=0;
		while(((dataLocationChecker*=(*(userRequest++)==headersEnding[dataLocationChecker++]))<4)&&*(userRequest-1));
		if(!(*userRequest))
			return 0;	 
		EVENT_DATA=userRequest;
		return UPLOADED_DATA;
	}
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
	endLocation-=endStringLength*(endStringLength==valueMatchCounter);
	CLR(endLocation);
	unsigned char *makeStr=originalString;
	while(*startLocation){
		*makeStr=(*startLocation);
		makeStr++;
		startLocation++;
	}
	while(*makeStr){
		*makeStr=0;
		makeStr++;
	}
	return originalString;
}


unsigned char responeseHeaders=0;
#define fecthHeadersEnable() responeseHeaders=1
#define fetchHeadersDisable() responeseHeaders=0

unsigned char *urlEncodeNotUsed(unsigned char *originalUrl){
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

unsigned char *urlEncodeReturnStr=(unsigned char*)malloc(1);
unsigned char *urlEncode(unsigned char *originalUrl) {
	unsigned char urlSpecialChars[23]="-._~:/?#[]@!$&'()*+,;=";
	unsigned char specialCharsCount=0;
	during(stringCounter(originalUrl),(argLoop index){
		specialCharsCount+=included(originalUrl[index],urlSpecialChars);
	});
	free(urlEncodeReturnStr);
	specialCharsCount=stringCounter(originalUrl)+(specialCharsCount*2)+1;
	urlEncodeReturnStr=(unsigned char*)calloc(specialCharsCount,sizeof(unsigned char));
	// _CS(urlEncodeReturnStr,$(specialCharsCount));
	unsigned char charToStr[2]="";
	during(stringCounter(originalUrl),(argLoop index){
		charToStr[0]=originalUrl[index];
		_CS(urlEncodeReturnStr,included(originalUrl[index],urlSpecialChars)?($("%",(intToHexaDecimal(originalUrl[index])+2))):(charToStr));
		// console.log(" >>> ",$("%",(intToHexaDecimal(originalUrl[index])+2)));
	});
	return urlEncodeReturnStr;
}

struct httpLink{
	unsigned char *domain=(unsigned char*)calloc(40,sizeof(unsigned char));
	unsigned short port;
	unsigned char secure;
	unsigned char *userName=(unsigned char*)calloc(30,sizeof(unsigned char));
	unsigned char *password=(unsigned char*)calloc(30,sizeof(unsigned char));
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

	// console.log(" >> ",responseBuffer,"\n---------------\n");

	if(urlParameters.secure){
		WiFiClientSecure client;
		client.setInsecure();
		if(!client.connect((char*)urlParameters.domain,urlParameters.port)){
			console.log("couldn't connect !!");
			return UNDEFINED;
		}
		client.println((char*)responseBuffer);
		CLR(responseBuffer);
		while(!client.available());
		unsigned char *makeStr=responseBuffer;
		unsigned long memoryLimter=0;
		if(!responeseHeaders){
			unsigned char headersPreset[5]="\r\n\r\n";
			unsigned char passedHeadersPreset=0;
			while(passedHeadersPreset!=4)
				passedHeadersPreset*=(client.read()==headersPreset[passedHeadersPreset++]);
		}
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
			console.log("couldn't connect !!");
			return UNDEFINED;
		}
		client.write((char*)responseBuffer);
		CLR(responseBuffer);
		while(!client.available());
		unsigned char *makeStr=responseBuffer;
		unsigned long memoryLimter=0;
		if(!responeseHeaders){
			unsigned char headersPreset[5]="\r\n\r\n";
			unsigned char passedHeadersPreset=0;
			while(passedHeadersPreset!=4)
				passedHeadersPreset*=(client.read()==headersPreset[passedHeadersPreset++]);
		}
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
	// if(equalStrings((unsigned char*)"\r\n\r\n",returnedBuffer+(stringCounter(returnedBuffer)-4)))
	// 	CLR(returnedBuffer+(stringCounter(returnedBuffer)-4));

	return returnedBuffer;
}

unsigned char *fetchDynamicBuffer=(unsigned char*)malloc(1);
template <typename httpRequest_t,typename requestBody_t>
unsigned char *fetch(httpRequest_t httpRequest,requestBody_t requestBody){
	free(fetchDynamicBuffer);
	fetchDynamicBuffer=(unsigned char*)calloc(fetchMemoryLimiter+1,sizeof(unsigned char));
	return fetch((unsigned char*)httpRequest,(unsigned char*)requestBody,fetchDynamicBuffer);
}

template <typename httpRequest_t>
unsigned char *fetch(httpRequest_t httpRequest){
	free(fetchDynamicBuffer);
	fetchDynamicBuffer=(unsigned char*)calloc(fetchMemoryLimiter+1,sizeof(unsigned char));
	return fetch((unsigned char*)httpRequest,UNDEFINED,fetchDynamicBuffer);
}






//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////    LEGACY-CODE    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



//^ 					//////////////////////////////////////////////////////////////////////////////////////////////
//^ 					/////////////////////////////////    SERVICE-EXECUTABLE    ///////////////////////////////////
//^ 					//////////////////////////////////////////////////////////////////////////////////////////////



#define POINT_BUFFER 0
#define DELETE_BUFFER 1

#define VIRTUAL_MEMORY_SIZE 4096
unsigned char *virtualControllerMemory=(unsigned char*)calloc(VIRTUAL_MEMORY_SIZE,sizeof(unsigned char));
unsigned char *virtualControllerMemoryIndex=(unsigned char*)calloc((size_t)(VIRTUAL_MEMORY_SIZE/2),sizeof(unsigned char));


void initializeVirtualControllerMemory(void){
	static unsigned char setupMemory;
	if(setupMemory)
		return;
	unsigned char *memoryJson=(unsigned char *)"{\"memory\":[]}";
	// virtualControllerMemory=(unsigned char*)calloc(stringCounter(memoryJson)+1,sizeof(unsigned char));
	_CS(virtualControllerMemory,memoryJson);
	// virtualControllerMemoryIndex=(unsigned char*)calloc(stringCounter(memoryJson)+1,sizeof(unsigned char));
	_CS(virtualControllerMemoryIndex,memoryJson);
	setupMemory=1;
}

unsigned char *highLevelMemory(unsigned long virtualMemoryAddress,unsigned char *savedData){
	constJsonReset();
	// constJsonValidate(savedData);
	if((stringCounter(savedData)+stringCounter(virtualControllerMemory)+5)>VIRTUAL_MEMORY_SIZE)			// the alogorithm depends on prealocated memory cause realloc didnt work, so we're just making sure we're not running out of memory
		return virtualControllerMemory;
	initializeVirtualControllerMemory();
	static unsigned short lastAddedElement=(unsigned short)-1;
	if(virtualMemoryAddress>lastAddedElement||(lastAddedElement==(unsigned short)-1)){		//new elemnt has been added
		lastAddedElement=virtualMemoryAddress;
		unsigned long orginalMemorySize=stringCounter(virtualControllerMemory);
		CLR(virtualControllerMemory+(orginalMemorySize-2));
		_CS(virtualControllerMemory,(*(virtualControllerMemory+(orginalMemorySize-3))!='[')?((unsigned char*)","):((unsigned char*)""));
		_CS(virtualControllerMemory,savedData);
		_CS(virtualControllerMemory,(unsigned char*)"]}");
	}
	else{
		unsigned char *getValueFromJson=(unsigned char*)calloc((stringCounter((unsigned char*)"memory[")+stringCounter(inttostring(virtualMemoryAddress))+stringCounter((unsigned char*)"]")+1),sizeof(unsigned char));
		_CS(getValueFromJson,(unsigned char*)"memory[");_CS(getValueFromJson,inttostring(virtualMemoryAddress));_CS(getValueFromJson,(unsigned char*)"]");		// i can not use $ cause it has a shared buffer for every instance
		// constJsonReset();
		JSON_LOW_MEMORY_USAGE(getValueFromJson,virtualControllerMemory);free(getValueFromJson);
		// console.log("virtualControllerMemory >> ",JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND);
		// constJsonValidate(JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND);
		JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR--;
		signed long differenceSize=(stringCounter(savedData)-1)-(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR-JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND);
		unsigned long finalMemorySize=stringCounter(virtualControllerMemory)+differenceSize+1;
		unsigned long memoryAllocationCounter=stringCounter(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR)*(differenceSize>0);
		within(stringCounter(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR)*(differenceSize!=0),{
			JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR[memoryAllocationCounter+differenceSize]=JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR[memoryAllocationCounter];
			memoryAllocationCounter-=(differenceSize>0);
			memoryAllocationCounter+=(differenceSize<0);
		});
		CLR(virtualControllerMemory+stringCounter(virtualControllerMemory)+differenceSize);
		during((JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR+differenceSize)-JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND+1,(argLoop index){
			JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND[index]=savedData[index];
		});
	}
	return virtualControllerMemory;
}


unsigned char *highLevelMemory(unsigned long virtualMemoryAddress){
	initializeVirtualControllerMemory();
	unsigned char *getValueFromJson=(unsigned char*)calloc((stringCounter((unsigned char*)"memory[")+stringCounter(inttostring(virtualMemoryAddress))+stringCounter((unsigned char*)"]")+1),sizeof(unsigned char));
	_CS(getValueFromJson,(unsigned char*)"memory[");_CS(getValueFromJson,inttostring(virtualMemoryAddress));_CS(getValueFromJson,(unsigned char*)"]");
	constJsonReset(); // this should solve the shared poniter problem
	unsigned char *savedData=constJson(getValueFromJson,virtualControllerMemory);
	// unsigned char *savedData=json(getValueFromJson,virtualControllerMemory);
	free(getValueFromJson);
	return savedData;
}



// unsigned char *highLevelMemoryIndex(unsigned long virtualMemoryAddress,unsigned char *savedData){
// 	if((stringCounter(savedData)+stringCounter(virtualControllerMemoryIndex)+5)>(VIRTUAL_MEMORY_SIZE/2))			
// 		return virtualControllerMemoryIndex;
// 	initializeVirtualControllerMemory();
// 	static unsigned short lastAddedElement=(unsigned short)-1;
// 	if(virtualMemoryAddress>lastAddedElement||(lastAddedElement==(unsigned short)-1)){		
// 		lastAddedElement=virtualMemoryAddress;
// 		unsigned long orginalMemorySize=stringCounter(virtualControllerMemoryIndex);
// 		CLR(virtualControllerMemoryIndex+(orginalMemorySize-2));
// 		_CS(virtualControllerMemoryIndex,(*(virtualControllerMemoryIndex+(orginalMemorySize-3))!='[')?((unsigned char*)","):((unsigned char*)""));
// 		_CS(virtualControllerMemoryIndex,savedData);
// 		_CS(virtualControllerMemoryIndex,(unsigned char*)"]}");
// 	}
// 	else{
// 		unsigned char *getValueFromJson=(unsigned char*)calloc((stringCounter((unsigned char*)"memory[")+stringCounter(inttostring(virtualMemoryAddress))+stringCounter((unsigned char*)"]")+1),sizeof(unsigned char));
// 		_CS(getValueFromJson,(unsigned char*)"memory[");_CS(getValueFromJson,inttostring(virtualMemoryAddress));_CS(getValueFromJson,(unsigned char*)"]");
// 		JSON_LOW_MEMORY_USAGE(getValueFromJson,virtualControllerMemoryIndex);free(getValueFromJson);
// 		JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR--;
// 		signed long differenceSize=(stringCounter(savedData)-1)-(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR-JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND);
// 		unsigned long finalMemorySize=stringCounter(virtualControllerMemoryIndex)+differenceSize+1;
// 		unsigned long memoryAllocationCounter=stringCounter(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR)*(differenceSize>0);
// 		within(stringCounter(JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR)*(differenceSize!=0),{
// 			JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR[memoryAllocationCounter+differenceSize]=JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR[memoryAllocationCounter];
// 			memoryAllocationCounter-=(differenceSize>0);
// 			memoryAllocationCounter+=(differenceSize<0);
// 		});
// 		CLR(virtualControllerMemoryIndex+stringCounter(virtualControllerMemoryIndex)+differenceSize);
// 		during((JSON_LOW_MEMORY_USAGE_DEAD_END_OF_STR+differenceSize)-JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND+1,(argLoop index){
// 			JSON_LOW_MEMORY_USAGE_JSON_OBJECT_FOUND[index]=savedData[index];
// 		});
// 	}
// 	return virtualControllerMemoryIndex;
// }

// unsigned char *highLevelMemoryIndex(unsigned long virtualMemoryAddress){
// 	unsigned char *getValueFromJson=(unsigned char*)calloc((stringCounter((unsigned char*)"memory[")+stringCounter(inttostring(virtualMemoryAddress))+stringCounter((unsigned char*)"]")+1),sizeof(unsigned char));
// 	_CS(getValueFromJson,(unsigned char*)"memory[");_CS(getValueFromJson,inttostring(virtualMemoryAddress));_CS(getValueFromJson,(unsigned char*)"]");
// 	unsigned char *savedData=json(getValueFromJson,virtualControllerMemoryIndex);
// 	free(getValueFromJson);
// 	return savedData;
// }


unsigned long smartPointer(unsigned long userAddress,unsigned char operation=POINT_BUFFER){
	initializeVirtualControllerMemory();
	#define virtualMemorySize 151
	static unsigned long registeredAddress[virtualMemorySize]={};
	//we have to init here before we go
	#define nullValue -1UL
	during(virtualMemorySize*(registeredAddress[virtualMemorySize-1]!=nullValue),(argLoop index){
		registeredAddress[index]=nullValue;
	});
		
	static unsigned long lastAddedElement;
	unsigned char registeredAddressCounter=0;
	while((registeredAddress[registeredAddressCounter++]!=userAddress)&&(registeredAddress[registeredAddressCounter-1]!=nullValue));
	registeredAddress[registeredAddressCounter-1]=userAddress;

	if(operation==DELETE_BUFFER){
		registeredAddress[registeredAddressCounter-1]=nullValue;
		highLevelMemory(registeredAddressCounter-1,(unsigned char*)"\"null\"");			// free the actual memory used
	}

	return registeredAddressCounter-1;
	
}


// unsigned long pointerAllocator(unsigned long bufferIdentifier,unsigned char operation){
// 	initializeVirtualControllerMemory();
// 	unsigned short scanForIndex=0;
// 	unsigned char *scanForPointerIndex;
// 	// constJsonReset();
// 	// console.log(highLevelMemoryIndex(0));
// 	// console.log(highLevelMemoryIndex(1));
// 	// while(((scanForPointerIndex=highLevelMemoryIndex(scanForIndex++))!=UNDEFINED)){	
// 	// 	// checking if the memory was not allocated or been cleared before
// 	// 	if(getInt32_t(scanForPointerIndex)==bufferIdentifier){
// 	// 		bufferIdentifier=scanForIndex-1;
// 	// 		goto realIndexIsNowSet;
// 	// 	}
// 	// }
	
// 	console.log(highLevelMemoryIndex(0));_delay_ms(200);
// 	console.log(highLevelMemoryIndex(0));_delay_ms(200);
// 	console.log(highLevelMemoryIndex(0));_delay_ms(200);

// 	while(((scanForPointerIndex=highLevelMemoryIndex(scanForIndex))!=UNDEFINED)&&(getInt32_t(scanForPointerIndex)!=bufferIdentifier)){scanForIndex++;console.log("loop");}
// 	if(getInt32_t(scanForPointerIndex)!=bufferIdentifier)	
// 		goto realIndexIsNowSet;
// 	constJsonReset();
// 	highLevelMemoryIndex(scanForIndex,inttostring(bufferIdentifier));
// 	highLevelMemory(scanForIndex,(unsigned char *)"");
// 	realIndexIsNowSet:
// 	if(operation==DELETE_BUFFER)
// 		highLevelMemoryIndex(scanForIndex,UNDEFINED);
// 	return scanForIndex;
// }


unsigned long virtualControllerOutputRegisterLow(unsigned long portData,unsigned long outputPortWidth,unsigned char outputStartBit){

	return portData;
}

unsigned long virtualControllerOutputRegisterHigh(unsigned long portData,unsigned long outputPortWidth,unsigned char outputStartBit){

	return portData;
}

unsigned long virtualControllerOutput(unsigned long portData,unsigned long outputPortWidth,unsigned char outputStartBit){
	console.log("portData >> ",portData);
	console.log("outputPortWidth >> ",outputPortWidth);
	console.log("outputStartBit >> ",outputStartBit);
	return portData;
}


unsigned long virtualControllerInputRegisterLow(void){
	unsigned long inputValue=0;

	return inputValue;
}

unsigned long virtualControllerInputRegisterHigh(void){
	unsigned long inputValue=0;

	return inputValue;
}

unsigned long virtualControllerInput(void){
	unsigned long inputValue=micros()/1500;

	// return inputValue;
	return 0x55555555;
}



std::vector<std::function<unsigned char*(unsigned char*)>>READ_CALLBACK_LIST;		// read from a real time connection
std::vector<std::function<unsigned char*(unsigned char*)>>WRITE_CALLBACK_LIST;		// write to a real time connection

void realTimeConnectionSet(unsigned char *dataToList){								// setting the data that we just got from the real time connection
	unsigned long readCallbackListCount=READ_CALLBACK_LIST.size();
	unsigned long readCallbackListCounter=0;
	while(readCallbackListCount--)
		READ_CALLBACK_LIST[readCallbackListCounter++](dataToList);					// passing the arg to every call back function in the list
	return;
}

void realTimeConnectionSend(unsigned char *dataToList){								// setting the data that we just got from the real time connection
	unsigned long writeCallbackListCount=WRITE_CALLBACK_LIST.size();
	unsigned long writeCallbackListCounter=0;
	while(writeCallbackListCount--)
		WRITE_CALLBACK_LIST[writeCallbackListCounter++](dataToList);					// passing the arg to every call back function in the list
	return;
}



#define realTimeConnectionSetAdd(SET_CALLBACK_ADD)		READ_CALLBACK_LIST.push_back([&]SET_CALLBACK_ADD)	//^ both vector and functional should be included
#define realTimeConnectionSendAdd(SEND_CALLBACK_ADD)	WRITE_CALLBACK_LIST.push_back([&]SEND_CALLBACK_ADD)	//^ both vector and functional should be included

unsigned long VIRTUAL_CONTROLLER_POLLING_RATE=20;
#define VIRTUAL_CONTROLLER_MAX_EVENTS 100
unsigned long VIRTUAL_CONTROLLER_EVENT_ADDRESS[VIRTUAL_CONTROLLER_MAX_EVENTS]={};


typedef const char* JSON_ATTRIBUTE;


JSON_ATTRIBUTE JSON_OPERATOR="OP";				// generic operator


// digital output & input
JSON_ATTRIBUTE OUTPUT_STREAM="OS";				// digital output stream
JSON_ATTRIBUTE PORT_ADDRESS="PA";				// digital output port address
JSON_ATTRIBUTE PORT_VALUE="PV";
JSON_ATTRIBUTE OUTPUT_INDEX="I";				// output object serialized

// digital input
// JSON_ATTRIBUTE PORT_ADDRESS="PA";				// digital input port address


// delay operator
JSON_ATTRIBUTE DELAY_MICRO_SEC="US";			// micro second delay
JSON_ATTRIBUTE DELAY_MILLI_SEC="MS";			// milli second delay

// loop operator
JSON_ATTRIBUTE LOOP_COUNTER="LC";				// loop counter
JSON_ATTRIBUTE LOOP_BODY="LB";					// loop body

// virtual controller memory
JSON_ATTRIBUTE BUFFER_IDENTIFIER="BI";
JSON_ATTRIBUTE BUFFER_DATA="BD";

// console logger
JSON_ATTRIBUTE CONSOLE_DATA="CD";

// fetch operator
JSON_ATTRIBUTE WEB_HOST="WH";
JSON_ATTRIBUTE POST_BODY="PB";
JSON_ATTRIBUTE REQUEST_PARAM="RP";

// $tring operator
JSON_ATTRIBUTE TOTAL_LENGTH="TL";
JSON_ATTRIBUTE SUB_STRING="SS";

// add event listener operator
JSON_ATTRIBUTE EVENT_ADDRESS="EA";
JSON_ATTRIBUTE EVENT_EXECUTABLE="EE";
JSON_ATTRIBUTE HANDLER_EXECUTABLE="HE";
JSON_ATTRIBUTE ONCHANGE_ADDRESS="CA";
JSON_ATTRIBUTE POLLING_RATE="PR";


// operator json
JSON_ATTRIBUTE OBJECT_KEY="OK";
JSON_ATTRIBUTE EXECUTABLE_JSON="EJ";


// function operator create & call
JSON_ATTRIBUTE FUNCTION_ADDRESS="FA";
JSON_ATTRIBUTE FUNCTION_OBJECT="FO";
JSON_ATTRIBUTE PARAMETER_ADDRESS="PA";
JSON_ATTRIBUTE STACK_EXECUTABLE="SE";
JSON_ATTRIBUTE RETURN_EXECUTABLE="RE";
JSON_ATTRIBUTE PARAMETER_OBJECT="PO";


// ececutable Stack operator
JSON_ATTRIBUTE EXECUTABLE_COUNTER="EC";
JSON_ATTRIBUTE EXECUTABLE_STACK="ES";


// ALU operator
JSON_ATTRIBUTE ALU_OPERATION="AO";
JSON_ATTRIBUTE FIRST_OPERAND="FO";
JSON_ATTRIBUTE SECOND_OPERAND="SO";


// server send operator
JSON_ATTRIBUTE PACKAGE_IDENTIFIER="PI";
JSON_ATTRIBUTE SERVER_DATA="SD";


unsigned char* virtualController(unsigned char* executableObject){
	const std::function<unsigned char*(unsigned char*)>jsonOperator[]={				// functional should be included so we can use lambda expression while passing variabels by ref
		[&](unsigned char *subExecutable){											//& digtal output operator
			const std::function<unsigned long(unsigned long,unsigned long,unsigned char)>outputPortList[]={
				[&](unsigned long portData,unsigned long outputPortWidth,unsigned char outputStartBit){											//^ register 0 output
					return virtualControllerOutputRegisterLow(portData,outputPortWidth,outputStartBit);
				},
				[&](unsigned long portData,unsigned long outputPortWidth,unsigned char outputStartBit){											//^ register 1 output
					return virtualControllerOutputRegisterHigh(portData,outputPortWidth,outputStartBit);
				},
				[&](unsigned long portData,unsigned long outputPortWidth,unsigned char outputStartBit){											//^ shift-register output
					return virtualControllerOutput(portData,outputPortWidth,outputStartBit);
				}
			};

			unsigned long portAddress=getInt32_t(virtualController(constJson(PORT_ADDRESS,subExecutable)));
			unsigned short portSelector=portAddress&0xFFFF;									//~ bits from 0->15
			unsigned char startBit=(portAddress>>16)&((1<<6)-1);							//~ bits from 16->21
			unsigned long portWidth=((portAddress>>22)&((1<<6)-1))+1;						//~ bits from 22->27
			portWidth=((portWidth==32)?((const unsigned long)-1):((1<<portWidth)-1));		//! this needs to be changed asap

			#define finalPortValue(userValue) ((userValue&portWidth)<<startBit)			//^ getting the final value that would be represented on the output port
			std::vector<unsigned long>portOutputStream;
			unsigned char streamObjectBuffer[18]={};
			unsigned short streamObjectBufferCounter=0;
			unsigned char *outputIndex=NULL;
			while((outputIndex=virtualController(constJson(_CS(CLR(streamObjectBuffer),$(OUTPUT_INDEX,streamObjectBufferCounter++)),subExecutable)))!=UNDEFINED)
				portOutputStream.push_back(finalPortValue(getInt32_t(outputIndex)));		// according to google vectors should dynamicly deallocate

			streamObjectBufferCounter=0;
			within(portOutputStream.size(),{
				outputPortList[portSelector](portOutputStream[streamObjectBufferCounter++],portWidth,startBit);
			});
			

			return subExecutable;
		},
		[&](unsigned char *	subExecutable){											//& digtal input operator
			const std::function<unsigned long(void)>inputPortList[]={
				[&](void){																//^ register 0 input
					return virtualControllerInputRegisterLow();
				},
				[&](void){																//^ register 1 input
					return virtualControllerInputRegisterHigh();
				},
				[&](void){																//^ shift-register input
					return virtualControllerInput();
				}
			};
			
			unsigned long portAddress=getInt32_t(virtualController(constJson(PORT_ADDRESS,subExecutable)));
			unsigned short portSelector=portAddress&0xFFFF;									//~ bits from 0->15
			unsigned char startBit=(portAddress>>16)&((1<<6)-1);							//~ bits from 16->21
			unsigned long portWidth=((portAddress>>22)&((1<<6)-1))+1;						//~ bits from 22->27
			portWidth=((portWidth==32)?((const unsigned long)-1):((1<<portWidth)-1));		//! this needs to be changed asap
			
			#define finalPortValueRead(userValue) ((userValue>>startBit)&portWidth)

			static unsigned char *digitalInputPortRaed=NULL;
			if(digitalInputPortRaed!=NULL)
				free(digitalInputPortRaed);
			digitalInputPortRaed=inttostring(finalPortValueRead(inputPortList[portSelector]()));
			return CACHE_BYTES(digitalInputPortRaed);
		},
		[&](unsigned char *subExecutable){											//& delay operator
			_delay_ms(getInt32_t(constJson(DELAY_MILLI_SEC,subExecutable)));
			return subExecutable; 
		},
		[&](unsigned char *subExecutable){											//& loop operator
			unsigned char *loopCounter;
			if((loopCounter=constJson(LOOP_COUNTER,subExecutable))!=UNDEFINED){
				within(getInt32_t(loopCounter),{
					unsigned char *finalExecutableObject;
					unsigned char jsonArrayCounter=0;
					while((finalExecutableObject=constJson($(LOOP_BODY,"[",jsonArrayCounter++,"]"),subExecutable))!=UNDEFINED){
						virtualController(finalExecutableObject);
					}
				});
			}
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//& console logger operator
			console.log(" -> ",virtualController(constJson(CONSOLE_DATA,subExecutable))," <- ");
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//& hardware id
			return (unsigned char*)"{\"Type\":\"ESP32-BASED-virtual-controller\"}";
		},
		[&](unsigned char *subExecutable){											//& virtual controller memory wrtie
			unsigned long finalMemorryAddress=smartPointer(getInt32_t(constJson(BUFFER_IDENTIFIER,subExecutable)),POINT_BUFFER);
			unsigned char *savedData=virtualController(constJson(BUFFER_DATA,subExecutable));
			unsigned char *finalSavedData=(unsigned char*)calloc(stringCounter(savedData)+1,sizeof(unsigned char));
			highLevelMemory(
				finalMemorryAddress,		// address is now defined 
				_CS(finalSavedData,savedData)
			);
			free(finalSavedData);
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//& virtual controller memory read
			static unsigned char *readVirtualMemoryAllocator=NULL;
			if(readVirtualMemoryAllocator!=NULL)
				free(readVirtualMemoryAllocator);
			unsigned char *finalDataFromVirtualMemory=highLevelMemory(smartPointer(getInt32_t(constJson(BUFFER_IDENTIFIER,subExecutable)),POINT_BUFFER));
			readVirtualMemoryAllocator=(unsigned char *)calloc(stringCounter(finalDataFromVirtualMemory)+1,sizeof(unsigned char));
			return _CS(readVirtualMemoryAllocator,finalDataFromVirtualMemory);
		},
		[&](unsigned char *subExecutable){											//& virtual controller memory delete
			smartPointer(getInt32_t(constJson(BUFFER_IDENTIFIER,subExecutable)),DELETE_BUFFER);
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//& fetch operator
			unsigned char *webHostUrlBuffer=(unsigned char*)calloc(256,sizeof(unsigned char));		//creating a buffer for the url as the object will change as the value gets used
			// unsigned char *postBodyBuffer=(unsigned char *)calloc(1024,sizeof(unsigned  char));


			unsigned char *mainPostBodyBuffer=(unsigned char *)calloc(1024,sizeof(unsigned  char));

			unsigned char *postBodyBuffer=mainPostBodyBuffer;

			unsigned char *postRequestParams;
			if((postRequestParams=virtualController(constJson(REQUEST_PARAM,subExecutable)))!=UNDEFINED){
				_CS(mainPostBodyBuffer,(unsigned char*)"{\"");
				_CS(mainPostBodyBuffer,(unsigned char*)REQUEST_PARAM);
				_CS(mainPostBodyBuffer,(unsigned char*)"\":");
				_CS(mainPostBodyBuffer,(unsigned char*)postRequestParams);
				_CS(mainPostBodyBuffer,(unsigned char*)",\"");
				_CS(mainPostBodyBuffer,(unsigned char*)POST_BODY);
				_CS(mainPostBodyBuffer,(unsigned char*)"\":");
				postBodyBuffer+=stringCounter(mainPostBodyBuffer);
			}



			unsigned char *finalWebHostUrl=_CS(webHostUrlBuffer,constJson(WEB_HOST,subExecutable));
			unsigned char *finalPostBody=virtualController(_CS(postBodyBuffer,constJson(POST_BODY,subExecutable)));		// some how i need to cache it in the same place
			// console.log(" ===> ",finalWebHostUrl);
			constJsonReset();
			// console.log(" ---> ",finalPostBody);
			unsigned long forceCachingCounter=0;
			within(stringCounter(finalPostBody),{
				postBodyBuffer[forceCachingCounter]=finalPostBody[forceCachingCounter++];
			});
			CLR(postBodyBuffer+forceCachingCounter);
			unsigned char *dataFromFetch;


			if(postRequestParams!=UNDEFINED){
				_CS(mainPostBodyBuffer+stringCounter(mainPostBodyBuffer),(unsigned char*)"}");
			}

			// console.log(" ---> ",mainPostBodyBuffer);

			if(!equalStrings(finalPostBody,(unsigned char*)"undefined"))
				dataFromFetch=fetch(finalWebHostUrl,mainPostBodyBuffer);
			else
				dataFromFetch=fetch(finalWebHostUrl);
			free(webHostUrlBuffer);
			free(mainPostBodyBuffer);
			return dataFromFetch;
		},
		[&](unsigned char *subExecutable){											//& add event listener operator
			unsigned long eventListenerAddress=getInt32_t(constJson(EVENT_ADDRESS,subExecutable));
			unsigned char eventAllocatorCounter=0;
			while(VIRTUAL_CONTROLLER_EVENT_ADDRESS[eventAllocatorCounter++]);	// this should be initialized with 0s
			VIRTUAL_CONTROLLER_EVENT_ADDRESS[eventAllocatorCounter-1]=eventListenerAddress*(eventAllocatorCounter!=VIRTUAL_CONTROLLER_MAX_EVENTS);
			// highLevelMemory(
			// 	smartPointer(eventListenerAddress,POINT_BUFFER),		
			// 	$(constJson(EVENT_EXECUTABLE,subExecutable))
			// );

			unsigned long finalMemorryAddress=smartPointer(eventListenerAddress,POINT_BUFFER);
			unsigned char *savedData=constJson(EVENT_EXECUTABLE,subExecutable);
			unsigned char *finalSavedData=(unsigned char*)calloc(stringCounter(savedData)+1,sizeof(unsigned char));
			highLevelMemory(
				finalMemorryAddress,		// address is now defined 
				_CS(finalSavedData,savedData)
			);
			free(finalSavedData);

			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//&  remove event listener operator
			unsigned long eventListenerAddress=getInt32_t(constJson(EVENT_ADDRESS,subExecutable));
			unsigned char eventAllocatorCounter=0;
			while(VIRTUAL_CONTROLLER_EVENT_ADDRESS[eventAllocatorCounter++]!=eventListenerAddress);	// the very last address should always be zero
			VIRTUAL_CONTROLLER_EVENT_ADDRESS[--eventAllocatorCounter]*=(VIRTUAL_CONTROLLER_EVENT_ADDRESS[eventAllocatorCounter]!=eventListenerAddress);
			smartPointer(eventListenerAddress,DELETE_BUFFER);
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//&  change event polling rate operator
			VIRTUAL_CONTROLLER_POLLING_RATE=getInt32_t(constJson(POLLING_RATE,subExecutable));
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//&  operator json
			unsigned char *objectKey=virtualController(constJson(OBJECT_KEY,subExecutable));
			CACHE_BYTES(objectKey);
			unsigned char *executableJson=virtualController(constJson(EXECUTABLE_JSON,subExecutable));
			CACHE_BYTES(executableJson);
			static unsigned char *operatorJsonReturn=NULL;
			if(operatorJsonReturn!=NULL)
				free(operatorJsonReturn);
			operatorJsonReturn=virtualController(constJson(objectKey,executableJson));
			free(objectKey);
			free(executableJson);
			return CACHE_BYTES(operatorJsonReturn);
		},
		[&](unsigned char *subExecutable){											//& create new function
			unsigned long functionAddress=getInt32_t(constJson(FUNCTION_ADDRESS,subExecutable));		// in this case the address of the function is a const
			unsigned char *functionObject=constJson(FUNCTION_OBJECT,subExecutable);
			highLevelMemory(
				smartPointer(functionAddress),
				CACHE_BYTES(functionObject)
			);
			free(functionObject);
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//& call function
			unsigned char *parameterObject=constJson(PARAMETER_OBJECT,subExecutable);
			CACHE_BYTES(parameterObject);

			unsigned char *functionObject=highLevelMemory(smartPointer(getInt32_t(constJson(FUNCTION_ADDRESS,subExecutable))));
			CACHE_BYTES(functionObject);

			unsigned long parameterAddress=getInt32_t(constJson(PARAMETER_ADDRESS,functionObject));
			highLevelMemory(
				smartPointer(parameterAddress),
				parameterObject
			);

			unsigned char *stackExecutable=constJson(STACK_EXECUTABLE,functionObject);
			virtualController(CACHE_BYTES(stackExecutable));
			unsigned char *returnExecutable=constJson(RETURN_EXECUTABLE,functionObject);

			static unsigned char *functionReturn=NULL;
			if(functionReturn!=NULL)
				free(functionReturn);
			functionReturn=virtualController(CACHE_BYTES(returnExecutable));

			free(functionObject);
			free(parameterObject);
			free(stackExecutable);
			free(returnExecutable);
			return CACHE_BYTES(functionReturn);
		},
		[&](unsigned char *subExecutable){											//& EXECUTABLE STACK
			unsigned long executableCounter=getInt32_t(virtualController(constJson(EXECUTABLE_COUNTER,subExecutable)));
			unsigned long executableStackCounter=0;
			unsigned char executableStackArrayElement[18]={};	// a super empty array so it would be quicker than dynamic memory allocation

			const std::function<unsigned char*(unsigned long)>jsonIndex=[&](unsigned long index){
				CLR(executableStackArrayElement);
				_CS(executableStackArrayElement,(unsigned char*)EXECUTABLE_STACK);
				_CS(executableStackArrayElement,(unsigned char*)"[");
				_CS(executableStackArrayElement,inttostring(index));
				_CS(executableStackArrayElement,(unsigned char*)"]");
				return executableStackArrayElement;
			};

			unsigned char *executableStackElement=NULL;
			while(executableCounter--){
				// _delay_ms(200);console.log(" >> ",executableCounter);
				executableStackCounter=0;
				// while((executableStackElement=constJson(jsonIndex(executableStackCounter++),subExecutable))!=UNDEFINED){
				// while((executableStackElement=constJson(_CS(CLR(executableStackArrayElement),$(EXECUTABLE_STACK,"[",executableStackCounter++,"]")),subExecutable))!=UNDEFINED){
				// 	virtualController($(executableStackElement));
				// 	// _delay_ms(200);console.log(" >> ",executableStackElement);
				// 	// free(executableStackElement);
				// }
				while(virtualController(constJson($(EXECUTABLE_STACK,"[",executableStackCounter++,"]"),subExecutable))!=UNDEFINED);
			}
			return subExecutable;
		},
		[&](unsigned char *subExecutable){											//& TURING COMPLETE
			const std::function<unsigned long(unsigned long,unsigned long)>ALUOperation[]={			
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ ADD 				-ARITHMATIC
					return firstOperand+secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ SUB 				-ARITHMATIC
					return firstOperand-secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ MUL 				-ARITHMATIC
					return firstOperand*secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ DIV 				-ARITHMATIC
					return firstOperand/secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ MOD 				-ARITHMATIC
					return firstOperand%secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ 2's COMP 			-ARITHMATIC
					return (~firstOperand)+1;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ XOR 				-BITWISE
					return firstOperand^secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ OR 					-BITWISE
					return firstOperand|secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ AND 				-BITWISE
					return firstOperand&secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ COMP 				-BITWISE
					return ~firstOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ SHIFT RIGHT 		-BITWISE
					return firstOperand>>secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ SHIFT LEFT 			-BITWISE
					return firstOperand<<secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ EQUAL 				-LOGIC
					return (firstOperand==secondOperand);
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ NOT EQUAL 			-LOGIC
					return (firstOperand!=secondOperand);
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ AND 				-LOGIC
					return firstOperand&&secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ OR 					-LOGIC
					return firstOperand||secondOperand;
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ NOT 				-LOGIC
					return (!firstOperand);
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ SMALLER 			-LOGIC
					return (firstOperand<secondOperand);
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ GREATER 			-LOGIC
					return (firstOperand>secondOperand);
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ SMALLER OR EQUAL	-LOGIC
					return (firstOperand<=secondOperand);
				},
				[&](unsigned long firstOperand,unsigned long secondOperand){	//^ GREATER OR EQUAL	-LOGIC
					return (firstOperand>=secondOperand);
				}
				// all the basic mathematical operations like Power, logarithmic , exponential , root , trignometric should be added further
			};
			// console.log(" ptr >> ",accumulator);
			unsigned long operationIndex=getInt32_t(virtualController(constJson(ALU_OPERATION,subExecutable)));
			unsigned long firstOperand=getInt32_t(virtualController(constJson(FIRST_OPERAND,subExecutable)));
			unsigned long secondOperand=getInt32_t(virtualController(constJson(SECOND_OPERAND,subExecutable)));
			static unsigned char *accumulator=NULL;
			if(accumulator!=NULL)
				free(accumulator);
			accumulator=inttostring(ALUOperation[operationIndex](firstOperand,secondOperand));
			return CACHE_BYTES(accumulator); 
		},
		[&](unsigned char *subExecutable){											//& SERVER SEND
			unsigned char *packageIndentifier=virtualController(constJson(PACKAGE_IDENTIFIER,subExecutable));
			CACHE_BYTES(packageIndentifier);
			unsigned char *serverData=virtualController(constJson(SERVER_DATA,subExecutable));
			CACHE_BYTES(serverData);
			unsigned char *dataToServer=makeJsonObject(JSON_KEYS(PACKAGE_IDENTIFIER,SERVER_DATA),JSON_VALUES(packageIndentifier,serverData));
			free(packageIndentifier);
			free(serverData);
			realTimeConnectionSend(CACHE_BYTES(dataToServer));
			free(dataToServer);
			return subExecutable;
		}


	};

	const unsigned short operatorsCount=sizeof(jsonOperator)/sizeof(jsonOperator[0]);

	unsigned char *jsonOperatorExist;
	if(((jsonOperatorExist=constJson(JSON_OPERATOR,executableObject))!=UNDEFINED)&&(getInt32_t(jsonOperatorExist)<operatorsCount))		// checking if it was an operator and a valid operator aka predefined
		return jsonOperator[getInt32_t(constJson(JSON_OPERATOR,executableObject))](executableObject);
	return executableObject;
}

void virtualControllerEventListener(void *params){
	unsigned long eventCheckerCounter=0;
	while(1){
		during(VIRTUAL_CONTROLLER_MAX_EVENTS,(unsigned long index){
			if(VIRTUAL_CONTROLLER_EVENT_ADDRESS[index]){				// all the values are null unless event do exist
				// console.log("__EVENT__",VIRTUAL_CONTROLLER_EVENT_ADDRESS[index]);_delay_ms(200);				
				unsigned char *eventExecutable=highLevelMemory(smartPointer(VIRTUAL_CONTROLLER_EVENT_ADDRESS[index]));
				unsigned long onchangeAddress=getInt32_t(constJson(ONCHANGE_ADDRESS,eventExecutable));


				
				unsigned char *eventChecker=virtualController(constJson(EVENT_EXECUTABLE,eventExecutable));
				eventChecker=_CS(((unsigned char*)calloc(stringCounter(eventChecker)+1,sizeof(unsigned char))),eventChecker);

				unsigned char *unchangedEventValue=highLevelMemory(smartPointer(onchangeAddress));
				unchangedEventValue=_CS(((unsigned char*)calloc(stringCounter(unchangedEventValue)+1,sizeof(unsigned char))),unchangedEventValue);
				
				unsigned char *handlerExecutable=constJson(HANDLER_EXECUTABLE,eventExecutable);
				handlerExecutable=_CS(((unsigned char*)calloc(stringCounter(handlerExecutable)+1,sizeof(unsigned char))),handlerExecutable);

				// console.log(" >> ",unchangedEventValue);_delay_ms(200);
				// console.log(" >> ",eventChecker);_delay_ms(200);


				if(!equalStrings(eventChecker,unchangedEventValue)){		// check if the value have changed then update it
					highLevelMemory(smartPointer(onchangeAddress),eventChecker);		// store the new value
					virtualController(constJson(HANDLER_EXECUTABLE,eventExecutable));
					// console.log("__EVENT_TRIG___");_delay_ms(200);
				}

				
				free(eventChecker);
				free(unchangedEventValue);
				free(handlerExecutable);

			}
		});
		_delay_ms(VIRTUAL_CONTROLLER_POLLING_RATE);
	}
	endTask();
}





//! //////////////////////////////////////////////////////////////////
//! ////////////////////	testing stuff goes here		//////////////
//! //////////////////////////////////////////////////////////////////


// JSON_PARSER jsonObject0;





void testingFuction(void * uselessParam){

	
	
	vTaskDelete(NULL);
}



unsigned short lastNetworkStat;



#define realTimeConnectionRetryInterval 100
#define realTimeConnectionUserCredentials() "\"anNvbiBkaXJlY3RpdmVzIHRlc3Qg\""		// this could be any unique string for each user
// unsigned char *realTimeConnectionBuffer=(unsigned char*)calloc(0x1FFF,sizeof(unsigned char));



void realTimeConnection(void *arg){
	_delay_ms(2000);
	while(lastNetworkStat!=WL_CONNECTED)
		_delay_ms(realTimeConnectionRetryInterval);

	_delay_ms(500);

	unsigned char *hostServer=NULL;
	hostServerDisconnected:
	while((hostServer=json("xtensa",fetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json")))==UNDEFINED)	// getting the data of host server while checking for connection
		_delay_ms(realTimeConnectionRetryInterval);

	struct httpLink hostServerAddress=urlBreakDown($("http://",hostServer));			// host:port expected


	WiFiClient tcpConnection;
	static unsigned char realTimeConnectionBuffer[0x1fff]={};

	static unsigned char runOnlyOnce;
	if(runOnlyOnce=1)
	WRITE_CALLBACK_LIST.push_back([&](unsigned char *tcpConnectionSend){		//^ adding call back function 
		base64(tcpConnectionSend,CLR(realTimeConnectionBuffer));
		if(tcpConnection.connected())
			tcpConnection.write((char*)realTimeConnectionBuffer);
		return tcpConnectionSend;
	});
	
	while(!tcpConnection.connect((char*)hostServerAddress.domain,hostServerAddress.port))
		_delay_ms(realTimeConnectionRetryInterval);
	console.log(" > RT server connected");


	tcpConnection.write((char*)makeJsonObject(JSON_KEYS("auth"),JSON_VALUES(realTimeConnectionUserCredentials())));

	while(1){
		if(tcpConnection.available()){
			realTimeConnectionSet(base64Decode(tcpGetString(tcpConnection,CLR(realTimeConnectionBuffer))));
		}
		else if(!tcpConnection.connected()){
			console.log("RT server Disconnected");
			goto hostServerDisconnected;
		}		
		_delay_ms(5);
	}

	

	vTaskDelete(NULL);
	return;
}





//////////////////////////////////////////////////////////////////
////////////////////	testing stuff goes here		//////////////
//////////////////////////////////////////////////////////////////



#define REMOTE_HOST "192.168.1.130"
#define REMOTE_PORT 80U
#define GENERAL_RESPONSE "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: *\r\n\r\nSk9ITiBLSEFMSUw=\r\n"
#define CORS_HEADERS (char *)"HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: *\r\n\r\n"
#define CLIENT_ACK "REQUEST_ACK\r\n"

#define EXPORTED_DATA_MAX_SIZE 10240UL
unsigned char EXPORTED_DATA[EXPORTED_DATA_MAX_SIZE]="";




void serviceExecutable(void*param){

	

    _delay_ms(500);



	eepromInit();
	
	// static unsigned short lastNetworkStat;			//! legacy code modified

	// unsigned char fileSystemMounted=1;
	// if(!fileSystem.begin()){
	// 	fileSystemMounted=0;
	// 	console.log("cannot mount FileSystem\r\n");
	// }

	
// 
// 
	// console.log("test >> ", 5012);

	

	console.log("serial port");


	const char *ssid="RISC-V";
	const char *password="threadripper";
	ssidSave((unsigned char*)ssid);
	wifiPasswordSave((unsigned char*)password);

	linkerSave(UNDEFINED);		//! host server is disabled to re-enable it just remove or comment this very line

	console.log(userSSID());
	console.log("serial port");
	console.log(userPassword());
	console.log("serial port");
	

	lastNetworkStat=networkConnect();
	console.log(inttostring(lastNetworkStat==WL_CONNECTED)," connected");
 	

	unsigned char networkWasConnect=(lastNetworkStat==WL_CONNECTED);

	WiFiServer server(80);
	server.begin();


	WiFiClient socket;
	//socket.connect(REMOTE_HOST,REMOTE_PORT);
	socket.setTimeout(5000);
	// linkerSave((unsigned char*)"{\"D\":\"xtensa32plus.ddns.net\",\"P\":50,\"K\":\"0123456789123456\",\"U\":{\"auth\":\"test0\"}}");
	unsigned char serverConnected=serverConnect(socket);
	// unsigned char serverConnected=0;
	uint64_t lastServerTime=0;


	WiFiClient client;
	client.setTimeout(5000);

	#define enc(_RAWTEXT) tcps((unsigned char*)_RAWTEXT,USER_KEY,EXPORTED_DATA)
	unsigned char *_serverData;// not to call a pointer returnning func twice
	#define SERVER_SEND(serverData) _serverData=_$Str(serverData); if(serverConnected){socket.write((char*)enc(_serverData));dataMask((unsigned char*)_serverData,USER_KEY);CLR(EXPORTED_DATA);}

	#define remoteUserData 0x00010000UL
	#define remoteUserDisconnected 0x00020000UL
	#define portListener 0x00040000UL
	#define clientDisconnected 0x00080000UL

	unsigned long eventListener=0;
	unsigned long staticListener=0;
	unsigned char clientJustConnected=0; //this method might lead to lots of synchronization problems but in a very few conditions
	unsigned char socketConnection=0;
	
	console.log("start the loop");
	// MDNS.begin("xtensa-lx6");
	// MDNS.addService("http", "tcp", 80);

	READ_CALLBACK_LIST.push_back([&](unsigned char *tcpConnectionRead){		//^ adding call back function 
		unsigned char realTimeConnectionBuffer[0x1FFF]={};
		virtualController(_CS(CLR(realTimeConnectionBuffer),tcpConnectionRead));
		// virtualController(tcpConnectionRead);
		realTimeConnectionSend((unsigned char*)"MAIN-THREAD-LOAD");
		return tcpConnectionRead;
	});


	xTaskCreate(
        testingFuction,    // Function that should be called
        "interuptSimulator",   // Name of the task (for debugging)
        30000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );

	xTaskCreate(
        virtualControllerEventListener,    // Function that should be called
        "virtualControllerEventListener",   // Name of the task (for debugging)
        30000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );



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
			if(!(--reconnectTimer%50)){
				unsigned char *remoteHostObject=linkerData();
				static unsigned char remoteConnectionResetTimer;
				if(!(--remoteConnectionResetTimer)&&serverConnected)
					socket.write("Sk9ITiBLSEFMSUw=");
				if((json("D",remoteHostObject)!=UNDEFINED)&&(json("P",remoteHostObject)!=UNDEFINED)&&(json("K",remoteHostObject)!=UNDEFINED)&&(json("U",remoteHostObject)!=UNDEFINED)){
					if(!serverConnected)
						serverConnected=serverConnect(socket);
					else if(!(serverConnected=socket.connected()))
						console.log("server disconnected");
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
			// console.log("\n \\/  \\/  \\/  \\/  \\/  NEW-REQUEST \\/  \\/  \\/  \\/  \\/ \n");
			// console.log(tcpText);
			



			unsigned char eventType=eventIdentifier(tcpText);


			if((eventType&WEB_SOCKET)==WEB_SOCKET){// never wanna mess with this line
				console.log("\n\nuser data >> ",eventData(EVENT_DATA,tcpText,(unsigned char*)"\r\n"));
				

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
			// 	console.log("filePath >> ");
			// 	console.log(filePath);
			// 	console.log(_endl);
				
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

				#define VIRTUAL_CONTROLLER 0x00000200UL

			
				console.log("\n\nuser data >> ",eventData(EVENT_DATA,tcpText,(unsigned char*)"<@>"));
				

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

					if((userInstruction&SERVER_DATA)&&(!socketConnection)){
						unsigned char *serverConfig;
						client.write(CORS_HEADERS);
						if((serverConfig=json("serverData",tcpText))!=UNDEFINED)
							linkerSave(serverConfig);
						client.write((char*)linkerData());
						client.flush();
						client.stop();
					}

					if(userInstruction&VIRTUAL_CONTROLLER){
						client.write(CORS_HEADERS);
						client.write(CLIENT_ACK);
						client.flush();
						client.stop();
						unsigned char *virtualControllerStack=(unsigned char*)calloc(fetchMemoryLimiter+1,sizeof(unsigned char*));
						virtualController(_CS(virtualControllerStack,fetch("http://192.168.1.15:766")));
						constJsonReset();
						free(virtualControllerStack);
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
			console.log("DATA FROM SERVER >> ",tcpText);
			
			SERVER_SEND("this should work!!");
		
		}



		
	}
}







//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////    SERVICE-EXECUTABLE    ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////







void setup(){
	esp_task_wdt_init(-1,false);		//this is the most important line of this entire code esp_task_wdt_init(uint32_t timeInSec,bool panic);
    delayAutoCalibrate();
    consoleSetup();
    Serial.begin(115200);
	Serial.print("\n\n\n-----------------CODE STARTED-----------------\n\n\n");
	consoleFeedback.push_back([&](unsigned char*dataFromConsole,unsigned char autoNLCR){
		if(autoNLCR)
			Serial.println((char*)dataFromConsole);
		else	
			Serial.print((char*)dataFromConsole);
	});
    _PM(13,OUTPUT);

	

    // serviceExecutable();
    xTaskCreate(
        serviceExecutable,    			// Function that should be called
        "serviceExecutable",   			// Name of the task (for debugging)
        30000,            				// Stack size (bytes)
        NULL,          		  			// Parameter to pass
        1,               				// Task priority
        NULL             				// Task handle
    );

	
	xTaskCreate(
        realTimeConnection,    // Function that should be called
        "realTimeConnection",   // Name of the task (for debugging)
        30000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );


	// xTaskCreate(
    //     virtualControllerEventListener,    // Function that should be called
    //     "virtualControllerEventListener",   // Name of the task (for debugging)
    //     30000,            // Stack size (bytes)
    //     NULL,            // Parameter to pass
    //     1,               // Task priority
    //     NULL             // Task handle
    // );

    
	// _delay_ms(2000);


	// initializeVirtualControllerMemory();
	// console.log(virtualControllerMemory);
	// console.log(" >> ",highLevelMemoryIndex(0,(unsigned char*)"test0"));
	// console.log(" >> ",highLevelMemoryIndex(1,(unsigned char*)"hello world"));
	// console.log(" >> ",highLevelMemoryIndex(2,(unsigned char*)"last element"));
	// console.log(" >> ",highLevelMemoryIndex(1,(unsigned char*)"short str"));
	// console.log(" >> ",highLevelMemoryIndex(1,(unsigned char*)"very--long--string"));
	// console.log(" >> ",highLevelMemoryIndex(2,(unsigned char*)"changed"));
	// console.log(" >> ",highLevelMemoryIndex(0,(unsigned char*)"test0-=-=-="));



	

	// console.log($("hello ","world >> ",-35));

	// virtualController(fetch("http://192.168.1.15:766"));


	// during(10,(unsigned long index){
	// 	// console.log($("index","\t-\t",index));
	// 	console.log("\n >> ",fetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json"));
	// 	_delay_ms(500);
	// });

	// CLR(EXPORTED_DATA);
	// console.log("\n >> ",fetch((unsigned char*)"https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json",UNDEFINED,EXPORTED_DATA));
	// console.log("\n >> ",fetch("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json"));
	// console.log(" >> ",urlEncodeUpgraded((unsigned char*)("https://raw.githubusercontent.com/engkhalil/xtensa32plus/main/dnsSquared.json")));
	// CLR(EXPORTED_DATA);


	// during(20,(unsigned long index){
        
    //     console.log("ESP32 >> ",index);
    //     _delay_ms(100);
       
    // });

}


void loop(){

}
