#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>
// #include "stringFunctions.cpp"

class highLevelMemory
{
private:

    unsigned short stringCounter(unsigned char *counted){
        unsigned short counter=0;
        while(*counted){
            counter++;
            counted++;
        }
        return counter;
    }
    
    unsigned short CLR_LENGTH=0;									//this value will be reseted to zero after clearing the string/uint_8 pointer
    unsigned char * CLR(unsigned char *deletedString,unsigned short _CLR_LENGTH=0){
        CLR_LENGTH=(CLR_LENGTH)?CLR_LENGTH:_CLR_LENGTH;
        unsigned char *returnedString=deletedString;
        while(*deletedString||(CLR_LENGTH-=(CLR_LENGTH!=0))){
            *deletedString=0;
            deletedString++;	
        }
        return returnedString;
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


    uint8_t *MAIN_MEMORY=nullptr;
    uint32_t MAIN_MEMORY_SIZE=0;
    struct highLevelMemoryElement{
        std::string variableName="";
        union{
            uint32_t virtualAddress=-1;
            uint16_t userDefinedAddress;
        }address;
        uint32_t size=0;
        uint32_t length=0;
        uint8_t *physicalAddress=nullptr;
        uint32_t bind=-1;
        std::vector<std::function<void(unsigned char*)>>onchangeEventListeners;
    };
    std::vector<highLevelMemoryElement>allocationTable;
    
public:
    const uint8_t* NO_DATA=(uint8_t*)"NO_DATA";
    const uint8_t* UNDEFINED=(uint8_t*)"undefined";

    uint32_t getVectorAddress(uint8_t *variableName){
        uint32_t loopCounter=allocationTable.size();
        while(loopCounter--){
            if(allocationTable[loopCounter].variableName==std::string((char*)variableName))
                return loopCounter;
        }
        return -1;
    }
    
    uint32_t getVectorAddress(uint32_t memoryAddress){
        if(memoryAddress>>16)
            return memoryAddress>>16;
        uint32_t loopCounter=allocationTable.size();
        while(loopCounter--){
            if(allocationTable[loopCounter].address.virtualAddress==memoryAddress)
                return loopCounter;
        }
        return -1;
    }

    uint32_t lastAvailabeAddress(void){
        uint32_t lastAddress=0;
        for(const auto &memoryElement : allocationTable)
            lastAddress+=memoryElement.length+1;    // +1 for the end string NULL
        return lastAddress;
    }

    highLevelMemory &write(uint8_t* key,uint8_t* data){
        for(const auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key)){
                if(stringCounter(data)==memoryElement.length){
                    _CS(CLR(memoryElement.physicalAddress,memoryElement.length+1),data);
                }
                else{
                    
                }
                return *this;                
            }
        
        if((stringCounter(data)+lastAvailabeAddress())<(MAIN_MEMORY_SIZE+1)){
            highLevelMemoryElement newElement;
            newElement.variableName=std::string((char*)key);
            newElement.length=stringCounter(data);
            newElement.address.virtualAddress=(allocationTable.size()<<16);
            newElement.physicalAddress=MAIN_MEMORY+lastAvailabeAddress();
            allocationTable.push_back(newElement);
            _CS(CLR(newElement.physicalAddress,newElement.length+1),data);
        }
        
        return (*this);
    }

    highLevelMemory(uint32_t memorySize){
        MAIN_MEMORY_SIZE=memorySize;
        MAIN_MEMORY=(uint8_t*)calloc(++memorySize,sizeof(uint8_t));
    }
    ~highLevelMemory(){
        free(MAIN_MEMORY);
    }
};

