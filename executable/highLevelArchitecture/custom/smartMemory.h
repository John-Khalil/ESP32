#pragma once

#include "consoleLogger.h"
#include "consoleLogger.cpp"
#include "highLevelMemory.cpp"
#include "pointerTool.h"
#include "constDefinitions.h"
#include "eepromBasicConfig.h"
#include "networkUtils.h"
#include "platform.h"
#include "http.h"
#include "webService.h"
#include "fetch.h"

#include "jsonParser.h"
#include "localStorage.h"
#include "cyclicBinary.cpp"
#include "operaotors.h"


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

class memory{
	public:

		// std::vector<char[20]>memoryStringAddress;
		uint8_t *memoryStringAddress;
		uint32_t stringAddressSize=0;
		uint32_t getStringAddress(uint8_t *inputAddress){
			uint32_t nextStringIndex=0;
			while(1){
				if(equalStrings(inputAddress,&(memoryStringAddress[nextStringIndex]))){ //found / already exist
					return nextStringIndex;
				}
				while((nextStringIndex<stringAddressSize)&&memoryStringAddress[nextStringIndex++]);
				if((nextStringIndex+stringCounter(inputAddress)+1)>=stringAddressSize){
					return -1UL;
				}
				if(memoryStringAddress[nextStringIndex++]){ //  look for the next lable in the memory space
					continue;
				}
				else{// not found and still in memory bound -> create new
					_CS(&(memoryStringAddress[nextStringIndex]),inputAddress);
					// _CS(&(memoryStringAddress[nextStringIndex]),"\0");
				}

			}

		}

		struct memoryElement{
			uint32_t address=-1;
			uint16_t stringAddress=-1;      //^ to be used with vector lookup table
			
		};

		memory(uint8_t *mainMemory,uint32_t mainMemorySize,uint8_t *addrSpace,uint32_t addrSpaceSize){
			memoryStringAddress=addrSpace;
			stringAddressSize=addrSpaceSize;
			CLR(memoryStringAddress);

		}
};