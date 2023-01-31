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
    uint8_t *MAIN_MEMORY=nullptr;
    struct highLevelMemoryElement{
        std::string variable="";
        uint32_t address=-1;
    };
    
public:
    const uint8_t* NO_DATA=(uint8_t*)"NO_DATA";
    const uint8_t* UNDEFINED=(uint8_t*)"undefined";

    highLevelMemory(uint32_t memorySize){
        MAIN_MEMORY=(uint8_t*)calloc(++memorySize,sizeof(uint8_t));
    }
    ~highLevelMemory(){
        free(MAIN_MEMORY);
    }
};

