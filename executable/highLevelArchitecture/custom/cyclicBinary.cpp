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


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

class cyclicBinary{
    public:

    std::function<void(uint32_t)>sysTick;
    std::vector<std::function<void(uint16_t)>>readCallbackList;

    uint16_t mask=-1;
    uint16_t previousValue=0;

    void sysTick32Bit(uint32_t ticks,uint16_t portValue){
        static uint32_t lastPortValue=-1UL;
        if(lastPortValue==portValue){
            ticks=ticks<<16;
        }
        lastPortValue=portValue;
        sysTick(ticks);
        return;
    }

    cyclicBinary &onData(const std::function<void(uint16_t)>onReadCallback){
        readCallbackList.push_back(onReadCallback);
        return (*this);
    }

    uint16_t updateMasked(uint16_t rawData){
        uint8_t maskBitCounter=0;
        uint8_t rawDataBitCounter=0;
        while(maskBitCounter<16){
            if(mask&(1<<maskBitCounter++))
                (rawData&(1<<rawDataBitCounter++))?previousValue|=(1<<(maskBitCounter-1)):previousValue&=~(1<<(maskBitCounter-1));
        }

        return previousValue;
    }

    cyclicBinary &fastDecode(uint32_t *binaryCompressed,uint16_t length){
        while(length--){
            for(auto &readCallback:readCallbackList)
                readCallback(updateMasked((*binaryCompressed)&((uint16_t)-1)));
            sysTick32Bit((*binaryCompressed)>>16,(*binaryCompressed)&((uint16_t)-1));
            binaryCompressed++;
        }
        return (*this);
    }

    cyclicBinary &fastDecode(uint8_t *binaryCompressed,uint32_t loopCounter=1){
        union{
			uint8_t* base64;
			uint32_t* rawData;
		}compressionBuffer;

        uint32_t length=(stringCounter(binaryCompressed)*0.75)/4;
		compressionBuffer.base64=base64Decode(binaryCompressed);

        while(loopCounter--)
            fastDecode(compressionBuffer.rawData,length);

        return (*this);
    }


    cyclicBinary &decode(uint32_t *binaryCompressed,uint16_t length){
        static uint16_t exitCounter;
        if(exitCounter>=length){
            exitCounter=0;
            return (*this);
        }

        // if(!((*binaryCompressed)&((1<<16)-1))){
        //     mask=((*binaryCompressed)>>16);
        //     return decode((uint32_t*)(binaryCompressed+(++exitCounter)),length);
        // }

        uint32_t loopCounter=((*binaryCompressed)>>16)|((((*binaryCompressed)&((uint16_t)-1))==1)*((*(binaryCompressed+1))&0xffff0000));
        uint16_t loopBody=((*(binaryCompressed+(((*binaryCompressed)&((uint16_t)-1))==1)))&((uint16_t)-1))-1;
        uint32_t *startAddress=(binaryCompressed+(((*binaryCompressed)&((uint16_t)-1))==1))+1;
        exitCounter+=(uint16_t)(startAddress-binaryCompressed)+loopBody;
        while(loopCounter--){
            uint16_t loopBodyCounter=0;
            while(loopBodyCounter<loopBody){
                for(auto &readCallback:readCallbackList)
                    readCallback(startAddress[loopBodyCounter]&((uint16_t)-1));
                sysTick((startAddress[loopBodyCounter]>>16)|(((startAddress[loopBodyCounter]&((uint16_t)-1))==(startAddress[loopBodyCounter+1]&((uint16_t)-1)))*(startAddress[loopBodyCounter+1]&0xffff0000)));
                loopBodyCounter+=((startAddress[loopBodyCounter]&((uint16_t)-1))==(startAddress[loopBodyCounter+1]&((uint16_t)-1)))+1;
            }
        }
        return decode((uint32_t*)(binaryCompressed+exitCounter),length);
    }

    cyclicBinary &decode(uint8_t *binaryCompressed){
        union{
			uint8_t* base64;
			uint32_t* rawData;
		}compressionBuffer;

        uint32_t length=(stringCounter(binaryCompressed)*0.75)/4;
		compressionBuffer.base64=base64Decode(binaryCompressed);

        for(uint32_t itr=0;itr<=length;itr++)
            Serial.println(compressionBuffer.rawData[itr]);
        return (*this);

        
        return decode(compressionBuffer.rawData,length);
    }

    cyclicBinary &decode(char *binaryCompressed){
        return decode((uint8_t*)binaryCompressed);
    }

    cyclicBinary(const std::function<void(uint32_t)>_sysTick,uint16_t _previousValue=0){
        sysTick=_sysTick;
        previousValue=_previousValue;
    }

    ~cyclicBinary(){

    }

};