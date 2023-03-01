#ifndef FETCH_H
#define FETCH_H


#include <functional>
#include "pointerTool.h"
#include "eepromBasicConfig.h"
#include "platform.h"

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include "consoleLogger.h"
// #include "consoleLogger.cpp"

#ifdef ESP32
    #include <WiFi.h>
#endif

#ifdef ESP8266
    #include <ESP8266WiFi.h>
#endif


unsigned char validHex(unsigned char *base16){
	return((((*base16)>0x2F)&&((*base16)<0x3A))||(((*base16)>0x60)&&((*base16)<0x67))||(((*base16)>0x40)&&((*base16)<0x47)));
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


#endif