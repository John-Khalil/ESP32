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

const char* EMPTY_STRING="";

class Memory{
	public:
		static uint8_t *undefined;

		struct memoryElement{
			uint32_t address=-1;
			uint16_t memoryAddress=-1;
			uint16_t length=-1;

      uint8_t nonRecursiveWrite=0;
      std::vector<std::function<void(uint8_t*)>>*writeEvents=nullptr;
      uint8_t nonRecursiveRead=0;
      std::vector<std::function<void(void)>>*readEvents=nullptr;
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
			while(dataMemory[++storedDataIndex]||dataMemory[(storedDataIndex-1)]);
			
			uint32_t emptySpace=storedDataIndex;
			while((emptySpace<dataMemorySize)&&(!dataMemory[emptySpace++]));
			if(emptySpace<(storedDataSize+1))
				return -1;

			_CS(&(dataMemory[storedDataIndex]),storedData);
			return storedDataIndex;
		}

		void remove(uint16_t memoryAddress){
      if(memoryAddress==((uint16_t)-1))
		    return;
			uint8_t *dataAddress=&(dataMemory[memoryAddress]);
			uint16_t addressOffset=stringCounter(dataAddress)+1;
			CLR(dataAddress);

			// offset the data from memory
			uint8_t *offsetAddress=dataAddress+addressOffset;
			while(*offsetAddress){
				*(dataAddress++)=*offsetAddress;
				(*(offsetAddress++))=0;
			}

			// offset the allocation table
			for(auto &allocationTableElement:allocationTable)
				if(allocationTableElement.memoryAddress>memoryAddress)
					allocationTableElement.memoryAddress-=addressOffset;

			return;
		}

		void elementReadEvent(memoryElement& memoryElement){
			if((memoryElement.readEvents==nullptr)||(memoryElement.nonRecursiveRead))
				return;
			memoryElement.nonRecursiveRead=1;
			for(auto &readEvent:(*memoryElement.readEvents))
				readEvent();
			memoryElement.nonRecursiveRead=0;
		}

		void elementWriteEvent(memoryElement& memoryElement,uint8_t *data){
			if((memoryElement.writeEvents==nullptr)||(memoryElement.nonRecursiveWrite))
				return;
			memoryElement.nonRecursiveWrite=1;
			for(auto &writeEvent:(*memoryElement.writeEvents))
				writeEvent(data);
			memoryElement.nonRecursiveWrite=0;
		}

		uint8_t *read(uint32_t address){
			for(auto &allocationTableElement:allocationTable)
				if((allocationTableElement.address==address)&&(allocationTableElement.memoryAddress!=((uint16_t)-1))){
					elementReadEvent(allocationTableElement);
          return &(dataMemory[allocationTableElement.memoryAddress]);
				}
        return Memory::undefined;
		}

    uint8_t *read(uint8_t *address){
			for(auto &allocationTableElement:allocationTable)
				if(equalStrings(&(memoryStringAddress[(allocationTableElement.address&((1<<31)-1))]),address)&&(allocationTableElement.memoryAddress!=((uint16_t)-1))){
          elementReadEvent(allocationTableElement);
					return &(dataMemory[allocationTableElement.memoryAddress]);
				}
        return Memory::undefined;
		}

    uint8_t *read(char *address){
      return read((uint8_t*)address);
    }

		uint8_t *write(uint32_t address,uint8_t *data){
			for(auto &allocationTableElement:allocationTable){
				if(allocationTableElement.address==address){
					if(allocationTableElement.length==stringCounter(data)){
						CLR(&(dataMemory[allocationTableElement.memoryAddress]));
						_CS(&(dataMemory[allocationTableElement.memoryAddress]),data);
						elementWriteEvent(allocationTableElement,data);
						return (allocationTableElement.memoryAddress!=((uint16_t)-1))?(&(dataMemory[allocationTableElement.memoryAddress])):Memory::undefined;
					}
					remove(allocationTableElement.memoryAddress);
					uint16_t memoryAddress=write(data);
          allocationTableElement.memoryAddress=memoryAddress;
          allocationTableElement.length=stringCounter(data);
					elementWriteEvent(allocationTableElement,data);
          return (allocationTableElement.memoryAddress!=((uint16_t)-1))?(&(dataMemory[allocationTableElement.memoryAddress])):Memory::undefined;
				}

			}
      // create new
			uint16_t memoryAddress=write(data);
			if(memoryAddress!=((uint16_t)-1)){
				memoryElement newMemoryElement;
				newMemoryElement.address=address;
				newMemoryElement.memoryAddress=memoryAddress;
				newMemoryElement.length=stringCounter(data);
				allocationTable.push_back(newMemoryElement);
				return (newMemoryElement.memoryAddress!=((uint16_t)-1))?(&(dataMemory[newMemoryElement.memoryAddress])):Memory::undefined;
			}
			return Memory::undefined;
		}

    uint8_t *write(uint8_t * address,uint8_t *data){
      return write((1<<31)|getStringAddress(address),data);
    }

    void onWrite(uint32_t address,const std::function<void(uint8_t*)>&writeEvent){
			if(read(address)==Memory::undefined){
				memoryElement newMemoryElement;
				newMemoryElement.address=address;
				newMemoryElement.memoryAddress=((uint16_t)-1);
				newMemoryElement.length=0;
				allocationTable.push_back(newMemoryElement);
				allocationTable[allocationTable.size()-1].writeEvents=new std::vector<std::function<void(uint8_t*)>>;
				allocationTable[allocationTable.size()-1].writeEvents->push_back(writeEvent);
				return;
			}
      for(auto &allocationTableElement:allocationTable)
				if(allocationTableElement.address==address){
					((allocationTableElement.writeEvents==nullptr)
						?(allocationTableElement.writeEvents=new std::vector<std::function<void(uint8_t*)>>)
						:allocationTableElement.writeEvents
					)->push_back(writeEvent);
					return;
        }
    }

		void onRead(uint32_t address,const std::function<void(void)>&readEvent){
			if(read(address)==Memory::undefined){
				memoryElement newMemoryElement;
				newMemoryElement.address=address;
				newMemoryElement.memoryAddress=((uint16_t)-1);
				newMemoryElement.length=0;
				allocationTable.push_back(newMemoryElement);
				allocationTable[allocationTable.size()-1].readEvents=new std::vector<std::function<void(void)>>;
				allocationTable[allocationTable.size()-1].readEvents->push_back(readEvent);
				return;
			}
      for(auto &allocationTableElement:allocationTable)
				if(allocationTableElement.address==address){
					((allocationTableElement.readEvents==nullptr)
						?(allocationTableElement.readEvents=new std::vector<std::function<void(void)>>)
						:allocationTableElement.readEvents
					)->push_back(readEvent);
					return;
        }
    }






		

		Memory(uint8_t *mainMemory,uint32_t mainMemorySize,uint8_t *addrSpace,uint32_t addrSpaceSize){
			memoryStringAddress=addrSpace;
			stringAddressSize=addrSpaceSize;
			CLR(memoryStringAddress);

      Memory::undefined=(uint8_t*)EMPTY_STRING;

		}
		~Memory(){
			for(auto &allocationTableElement:allocationTable){
				if(allocationTableElement.writeEvents!=nullptr)
					delete allocationTableElement.writeEvents;
				if(allocationTableElement.readEvents!=nullptr)
					delete allocationTableElement.readEvents;
			}
		}
};