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
        uint8_t validToken=0;
        std::vector<std::function<void(unsigned char*)>>onchangeEventListeners;
        std::vector<std::function<void(void)>>readEventListeners;
    };
    std::vector<highLevelMemoryElement>allocationTable;
    
public:
    uint8_t* NO_DATA=(uint8_t*)"NO_DATA";
    uint8_t* UNDEFINED=(uint8_t*)"undefined";

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


    highLevelMemory &shiftAddress(highLevelMemoryElement &memoryElement){
        uint32_t addressCounter=0;
        uint32_t loopCounter=(MAIN_MEMORY+lastAvailabeAddress())-(memoryElement.physicalAddress+memoryElement.length+1);
        while(loopCounter--)
            memoryElement.physicalAddress[addressCounter]=memoryElement.physicalAddress[(memoryElement.length+1)+(addressCounter++)];
        return *this;
    }

    highLevelMemory &shiftAllocationTable(highLevelMemoryElement &memoryElement){
        uint32_t addressCounter=(memoryElement.address.virtualAddress>>16);
        // uint32_t loopCounter=allocationTable.size()-addressCounter;
        while((++addressCounter)<allocationTable.size()){
            allocationTable[addressCounter].address.virtualAddress-=65536;  // instaed of (((x>>16)-1)<<16)|(x&65535)
            allocationTable[addressCounter].physicalAddress-=memoryElement.length+1;
        }
        return *this;
    }

    highLevelMemoryElement lastActiveElement;

    highLevelMemoryElement nullElement;

    highLevelMemory &get(uint8_t* key){
        for(auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key)){
                lastActiveElement=memoryElement;             
                return (*this); 
            }
        lastActiveElement=nullElement;
        return (*this);
    }

    highLevelMemory &get(uint32_t key){
        for(auto &memoryElement : allocationTable){
            memoryElement=(key>>16)?allocationTable[key>>16]:memoryElement;     // switch context for the full virtual address
            if(memoryElement.address.userDefinedAddress==(key&0xFFFF)){         // fully validate for the given address
                lastActiveElement=memoryElement; 
                return (*this);             
            }
        }   
        lastActiveElement=nullElement;
        return (*this);
    }

    highLevelMemory &onChange(const std::function<void(unsigned char *)>onchangeEventListener){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].onchangeEventListeners.push_back(onchangeEventListener);
        return (*this);
    }

    highLevelMemory &onRead(const std::function<void(void)>readEventListener){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners.push_back(readEventListener);
        return (*this);
    }

    highLevelMemory &bind(uint8_t* key){
        if(lastActiveElement.physicalAddress!=nullptr)
            for(auto &memoryElement : allocationTable)
                if(memoryElement.variableName==std::string((char*)key)){
                    allocationTable[lastActiveElement.address.virtualAddress>>16].bind=memoryElement.address.virtualAddress;              
                }
        return (*this);
    }

    highLevelMemory &bind(uint32_t key){
        if(lastActiveElement.physicalAddress!=nullptr)
            for(auto &memoryElement : allocationTable){
                memoryElement=(key>>16)?allocationTable[key>>16]:memoryElement;     
                if(memoryElement.address.userDefinedAddress==(key&0xFFFF)){         
                    allocationTable[lastActiveElement.address.virtualAddress>>16].bind=memoryElement.address.virtualAddress;              
                }
            }
        return (*this);
    }

    highLevelMemory &bind(void){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].bind=-1;
        return (*this);
    }

    highLevelMemory &unBind(void){
        allocationTable[lastActiveElement.address.virtualAddress>>16].bind=-1;
        return (*this);
    }

    // uint8_t validToken=0;

    highLevelMemory &write(uint32_t key,uint8_t* data){
        highLevelMemoryElement newElement;
        uint16_t bindIndex=-1;
        for(auto memoryElementNonRef : allocationTable)
            if((memoryElementNonRef=(key>>16)?allocationTable[key>>16]:memoryElementNonRef).address.userDefinedAddress==(key&0xFFFF)){

                bindIndex=(memoryElementNonRef.bind!=-1)?(memoryElementNonRef.address.virtualAddress>>16):bindIndex;                            // keep index
                memoryElementNonRef=(memoryElementNonRef.bind!=-1)?allocationTable[memoryElementNonRef.bind>>16]:memoryElementNonRef;           // switch context for memory binding

                auto &memoryElement=allocationTable[memoryElementNonRef.address.virtualAddress>>16];                                            //* get back to the org ref


                

                newElement=memoryElement;
                if(stringCounter(data)==memoryElement.length){
                    _CS(CLR(memoryElement.physicalAddress,memoryElement.length+1),data);
                }
                else{
                    shiftAddress(memoryElement).shiftAllocationTable(memoryElement).allocationTable.erase(allocationTable.begin() + (memoryElement.address.virtualAddress>>16));
                    break;      // adding it as a new element
                }
                goto functionReturn; //dry code                
            }
        
        if((stringCounter(data)+lastAvailabeAddress())<(MAIN_MEMORY_SIZE+1)){
            newElement.variableName=std::string((char*)key);
            newElement.length=stringCounter(data);
            newElement.address.virtualAddress=(allocationTable.size()<<16);
            newElement.physicalAddress=MAIN_MEMORY+lastAvailabeAddress();
            allocationTable.push_back(newElement);
            _CS(CLR(newElement.physicalAddress,newElement.length+1),data);
        }

        functionReturn:
        lastActiveElement=newElement;
        if(!newElement.validToken)
        for(auto &onchangeCallback:allocationTable[(bindIndex==(uint16_t)-1)?(lastActiveElement.address.virtualAddress>>16):bindIndex].onchangeEventListeners)
            onchangeCallback(lastActiveElement.physicalAddress);

        return (*this);
    }

    uint8_t *read(uint32_t key){
        for(auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key)){
                memoryElement=(memoryElement.bind!=-1)?allocationTable[memoryElement.bind>>16]:memoryElement;        // switch context for memory binding

                lastActiveElement=memoryElement;

                // static uint8_t validToken;
                if(!memoryElement.validToken){
                    memoryElement.validToken=1;
                    for(auto &readCallback:allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners)
                        readCallback();
                    uint8_t *updatedAddress=read(key); // the element may change if the read callback triggered a write for the same element
                    memoryElement.validToken=0;
                    return updatedAddress;
                }

                return memoryElement.physicalAddress;                
            }
    
        return UNDEFINED;
    }

    highLevelMemory &write(uint8_t* key,uint8_t* data){
        highLevelMemoryElement newElement;
        uint16_t bindIndex=-1;
        for(auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key)){
                bindIndex=(memoryElement.bind!=-1)?(memoryElement.address.virtualAddress>>16):bindIndex;             // keep index
                memoryElement=(memoryElement.bind!=-1)?allocationTable[memoryElement.bind>>16]:memoryElement;        // switch context for memory binding

                newElement=memoryElement;
                if(stringCounter(data)==memoryElement.length){
                    _CS(CLR(memoryElement.physicalAddress,memoryElement.length+1),data);
                }
                else{
                    shiftAddress(memoryElement).shiftAllocationTable(memoryElement).allocationTable.erase(allocationTable.begin() + (memoryElement.address.virtualAddress>>16));
                    break;      // adding it as a new element
                }
                goto functionReturn; //dry code                
            }
        
        if((stringCounter(data)+lastAvailabeAddress())<(MAIN_MEMORY_SIZE+1)){
            newElement.variableName=std::string((char*)key);
            newElement.length=stringCounter(data);
            newElement.address.virtualAddress=(allocationTable.size()<<16);
            newElement.physicalAddress=MAIN_MEMORY+lastAvailabeAddress();
            allocationTable.push_back(newElement);
            _CS(CLR(newElement.physicalAddress,newElement.length+1),data);
        }

        functionReturn:
        lastActiveElement=newElement;
        if(!newElement.validToken)
        for(auto &onchangeCallback:allocationTable[(bindIndex==(uint16_t)-1)?(lastActiveElement.address.virtualAddress>>16):bindIndex].onchangeEventListeners)
            onchangeCallback(lastActiveElement.physicalAddress);

        return (*this);
    }

    uint8_t *read(uint8_t* key){
        for(auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key)){
                memoryElement=(memoryElement.bind!=-1)?allocationTable[memoryElement.bind>>16]:memoryElement;        // switch context for memory binding

                lastActiveElement=memoryElement;

                // static uint8_t validToken;
                if(!memoryElement.validToken){
                    memoryElement.validToken=1;
                    for(auto &readCallback:allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners)
                        readCallback();
                    uint8_t *updatedAddress=read(key); // the element may change if the read callback triggered a write for the same element
                    memoryElement.validToken=0;
                    return updatedAddress;
                }

                return memoryElement.physicalAddress;                
            }
    
        return UNDEFINED;
    }

    highLevelMemory(uint32_t memorySize){
        MAIN_MEMORY_SIZE=memorySize;
        MAIN_MEMORY=(uint8_t*)calloc(++memorySize,sizeof(uint8_t));
    }
    ~highLevelMemory(){
        free(MAIN_MEMORY);
    }
};

