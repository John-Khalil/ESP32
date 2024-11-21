#pragma once

#include "pointerTool.h"

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

		uint8_t *read(uint32_t address,uint8_t disableCallbacks=0){
			for(auto &allocationTableElement:allocationTable)
				if((allocationTableElement.address==address)&&(allocationTableElement.memoryAddress!=((uint16_t)-1))){
					if(!disableCallbacks)
						elementReadEvent(allocationTableElement);
          return &(dataMemory[allocationTableElement.memoryAddress]);
				}
        return Memory::undefined;
		}

    uint8_t *read(uint8_t *address,uint8_t disableCallbacks=0){
			for(auto &allocationTableElement:allocationTable)
				if(equalStrings(&(memoryStringAddress[(allocationTableElement.address&((1<<31)-1))]),address)&&(allocationTableElement.memoryAddress!=((uint16_t)-1))){
					if(!disableCallbacks)
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
						return read(address,1);
					}
					remove(allocationTableElement.memoryAddress);
					uint16_t memoryAddress=write(data);
          allocationTableElement.memoryAddress=memoryAddress;
          allocationTableElement.length=stringCounter(data);
					elementWriteEvent(allocationTableElement,data);
          return (memoryAddress!=((uint16_t)-1))?read(address,1):Memory::undefined;
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
				return read(address,1);
			}
			return Memory::undefined;
		}

    uint8_t *write(uint8_t * address,uint8_t *data){
      return write((1<<31)|getStringAddress(address),data);
    }

    void onWrite(uint32_t address,const std::function<void(uint8_t*)>&writeEvent){
      for(auto &allocationTableElement:allocationTable)
				if(allocationTableElement.address==address){
					((allocationTableElement.writeEvents==nullptr)
						?(allocationTableElement.writeEvents=new std::vector<std::function<void(uint8_t*)>>)
						:allocationTableElement.writeEvents
					)->push_back(writeEvent);
					return;
        }
			memoryElement newMemoryElement;
			newMemoryElement.address=address;
			newMemoryElement.memoryAddress=((uint16_t)-1);
			newMemoryElement.length=0;
			allocationTable.push_back(newMemoryElement);
			allocationTable[allocationTable.size()-1].writeEvents=new std::vector<std::function<void(uint8_t*)>>;
			allocationTable[allocationTable.size()-1].writeEvents->push_back(writeEvent);
			return;
    }

		void onRead(uint32_t address,const std::function<void(void)>&readEvent){
      for(auto &allocationTableElement:allocationTable)
				if(allocationTableElement.address==address){
					((allocationTableElement.readEvents==nullptr)
						?(allocationTableElement.readEvents=new std::vector<std::function<void(void)>>)
						:allocationTableElement.readEvents
					)->push_back(readEvent);
					return;
        }
			memoryElement newMemoryElement;
			newMemoryElement.address=address;
			newMemoryElement.memoryAddress=((uint16_t)-1);
			newMemoryElement.length=0;
			allocationTable.push_back(newMemoryElement);
			allocationTable[allocationTable.size()-1].readEvents=new std::vector<std::function<void(void)>>;
			allocationTable[allocationTable.size()-1].readEvents->push_back(readEvent);
			return;
    }






		uint8_t dynamicMemory=0;

		Memory(uint8_t *mainMemory,uint32_t mainMemorySize,uint8_t *addrSpace,uint32_t addrSpaceSize){
			memoryStringAddress=addrSpace;
			stringAddressSize=addrSpaceSize;
			CLR(memoryStringAddress);

			dataMemory=mainMemory;
			dataMemorySize=mainMemorySize;
			CLR(mainMemory);
		}

		Memory(uint32_t mainMemorySize,uint32_t addrSpaceSize){
			dynamicMemory=1;
			uint8_t *mainMemory=(uint8_t*)calloc(mainMemorySize, sizeof(uint8_t));
			uint8_t *addrSpace=(uint8_t*)calloc(addrSpaceSize, sizeof(uint8_t));
			Memory(mainMemory,mainMemorySize,addrSpace,addrSpaceSize);
		}

		~Memory(){
			for(auto &allocationTableElement:allocationTable){
				if(allocationTableElement.writeEvents!=nullptr)
					delete allocationTableElement.writeEvents;
				if(allocationTableElement.readEvents!=nullptr)
					delete allocationTableElement.readEvents;
			}
			if(dynamicMemory){
				free(memoryStringAddress);
				free(dataMemory);
			}
		}
}; 
uint8_t* Memory::undefined=(uint8_t*)EMPTY_STRING;

uint8_t mainMemory[3000]={};
uint8_t addrSpace[500]={};
Memory appLinker(mainMemory,3000,addrSpace,500);