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

		struct memoryElement{
			uint32_t address=-1;
			uint16_t stringAddress=-1;

			uint16_t memoryAddress=-1;
			uint16_t length=-1;


		};

		std::vector<memoryElement>allocationTable;

		uint8_t *memoryStringAddress;
		uint32_t stringAddressSize=0;
		
		uint32_t getStringAddress(uint8_t *inputAddress){
			uint32_t nextStringIndex=0;
			while(1){
				if(equalStrings(inputAddress,&(memoryStringAddress[nextStringIndex+1]))){ //found / already exist
					return nextStringIndex+1;
				}
				while((nextStringIndex<stringAddressSize)&&memoryStringAddress[++nextStringIndex]);
				if((nextStringIndex+stringCounter(inputAddress)+1)>=stringAddressSize){
					return -1UL;
				}
				if(memoryStringAddress[nextStringIndex+1]){ //  look for the next lable in the memory space
					continue;
				}
				else{// not found and still in memory bound -> create new
					_CS(&memoryStringAddress[nextStringIndex+1],inputAddress);
					return nextStringIndex+1;
				}
			}

		}

		uint8_t *dataMemory=nullptr;
		uint32_t dataMemorySize=0;

		uint16_t write(uint8_t *storedData){
			uint16_t storedDataSize=stringCounter(storedData);

			if((dataMemory==nullptr)||!storedDataSize)
				return -1;


			uint32_t storedDataIndex=0; 
			while(dataMemory[storedDataIndex]||dataMemory[++storedDataIndex]);
			
			uint32_t emptySpace=storedDataIndex;
			while((emptySpace<dataMemorySize)&&(!dataMemory[emptySpace++]));
			if(emptySpace<(storedDataSize+1))
				return -1;

			_CS(&(dataMemory[storedDataIndex]),storedData);
			return storedDataIndex;
		}

		void remove(uint16_t memoryAddress){
			uint8_t *dataAddress=&(dataMemory[memoryAddress]);
			uint16_t addressOffset=stringCounter(dataAddress)+1;
			CLR(dataAddress);

			// offset the data from memory
			uint8_t *offsetAddress=dataAddress+addressOffset;
			while(*offsetAddress)
				*(dataAddress++)=(*(offsetAddress++));

			// offset the allocation table
			for(auto &allocationTableElement:allocationTable)
				if(allocationTableElement.memoryAddress>memoryAddress)
					allocationTableElement.memoryAddress-=addressOffset;

			return;
		}





		

		memory(uint8_t *mainMemory,uint32_t mainMemorySize,uint8_t *addrSpace,uint32_t addrSpaceSize){
			memoryStringAddress=addrSpace;
			stringAddressSize=addrSpaceSize;
			CLR(memoryStringAddress);

		}
};