#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
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


    uint8_t *MAIN_MEMORY=nullptr;
    struct highLevelMemoryElement{
        std::string variableName="";
        union{
            uint32_t virtualAddress=-1;
            uint16_t userDefinedAddress;
        }address;
        uint32_t size=0;
        uint32_t length=0;
        uint8_t *physicalAddress=nullptr;
        uint32_t bind;
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
        for(const auto &memoryelement : allocationTable)
            lastAddress+=memoryelement.length+1;    // +1 for the end string NULL
        return lastAddress;
    }

    highLevelMemory &write(uint8_t *key,uint8_t *data){
        for(const auto &memoryelement : allocationTable)
            if(memoryelement.variableName==std::string((char*)key)){

                return *this;                
            }
        highLevelMemoryElement newElement;
        newElement.variableName=std::string((char*)key);

        allocationTable.push_back(newElement);
        return *this;
    }

    highLevelMemory(uint32_t memorySize){
        MAIN_MEMORY=(uint8_t*)calloc(++memorySize,sizeof(uint8_t));
    }
    ~highLevelMemory(){
        free(MAIN_MEMORY);
    }
};

