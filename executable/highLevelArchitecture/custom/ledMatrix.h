#include <stdint.h>
#include "matrixChars.h"
#include "platform.h"
#include "consoleLogger.h"

unsigned char nullText[5]="null";
unsigned char *displayText=nullText;
unsigned short  displayTextLength=4;

#define matCharAddress(asciiChar) (unsigned char)(asciiChar-(((0x30*((asciiChar>0x29)&&(asciiChar<0x3A))))|((0x37*((asciiChar>0x40)&&(asciiChar<0x5B))))|((0x3C*((asciiChar>0x60)&&(asciiChar<0X7B))))|(-4*(asciiChar==0x20))))

unsigned char sourceMappper(unsigned short address){
    if(matCharAddress(displayText[address%displayTextLength])>=matCharsLength)
        return 0;
    return matChars[matCharAddress(displayText[address%displayTextLength])][(address/displayTextLength)%8];         //never wanna mess with this line
}


// #define microSec(_timeInUs) //_delay_us(_timeInUs)

#define MARAWAN_ALGORITHM(BIT_INDEX,LENGTH) (((((BIT_INDEX%8)+(LENGTH%8)>8)))||(((BIT_INDEX%8)!=0)&&(!(LENGTH%8))))

uint64_t outOfMemory(unsigned long bitIndex,unsigned char returnLength){                                            //really not recommended for 64-bit values
    if(returnLength>64)
        returnLength=64;
    uint64_t finalResult=0;
    unsigned long byteIndex=bitIndex/8;
    // unsigned char shiftedLeft=(bitIndex%8)-1;
    unsigned char shiftedRight=((8-((bitIndex+returnLength)%8))*(((bitIndex+returnLength)%8)!=0));                  //((8-((bitIndex+returnLength)%8))*(((bitIndex+returnLength)%8)!=0))+1; bit index starts from zero
    
    unsigned char loopCounter=returnLength/8+((returnLength%8)!=0)+MARAWAN_ALGORITHM(bitIndex,returnLength);                                                 
    unsigned char upCounter=0;
    while(loopCounter--){
        finalResult|=(sourceMappper(byteIndex+loopCounter)<<(8*(upCounter++)))>>shiftedRight;
    }
    if(returnLength!=64)
        finalResult&=(1<<returnLength)-1;
    return finalResult;
}


#define SpiClk(_ClkPin)     _DW(12,(_ClkPin!=0))
#define SpiData(_DataPin)   _DW(27,(_DataPin!=0))
#define SpiLatch(_LatchPin) _DW(14,(_LatchPin!=0))

const float SpiClkSpeed=(1e6f/1000000)/2;

uint32_t shiftRegisterOutput(unsigned long externalPortValue){
    unsigned char shiftRegisterBitCounter=32;
    while(shiftRegisterBitCounter--){
        SpiData((externalPortValue>>shiftRegisterBitCounter)&0x01);
        SpiClk(1);
        microSec(SpiClkSpeed);
        SpiClk(0);
        microSec(SpiClkSpeed);
    }
    SpiData(0);
    SpiLatch(1);
    microSec(SpiClkSpeed*10);
    SpiLatch(0);
    microSec(SpiClkSpeed);
    return externalPortValue;
}

#define LED_MAT_PIN_1   13
#define LED_MAT_PIN_2   3
#define LED_MAT_PIN_3   4
#define LED_MAT_PIN_4   10
#define LED_MAT_PIN_5   6
#define LED_MAT_PIN_6   11
#define LED_MAT_PIN_7   15
#define LED_MAT_PIN_8   16

#define LED_MAT_PIN_9   9  
#define LED_MAT_PIN_10  14
#define LED_MAT_PIN_11  8
#define LED_MAT_PIN_12  12
#define LED_MAT_PIN_13  1
#define LED_MAT_PIN_14  7
#define LED_MAT_PIN_15  2
#define LED_MAT_PIN_16  5


unsigned short ledMatrixPinout(unsigned short alignedPins){
    if(!alignedPins)
        return 0;                                   //a little bit of optimization              
    alignedPins=0xFF00^alignedPins;
    unsigned short finalResult=0;
    unsigned short mat1088AS[16]={
        (const unsigned short)(1<<(LED_MAT_PIN_1-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_2-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_3-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_4-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_5-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_6-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_7-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_8-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_9-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_10-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_11-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_12-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_13-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_14-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_15-1)),
        (const unsigned short)(1<<(LED_MAT_PIN_16-1))
    };
    unsigned char pinsCounter=16;
    while(pinsCounter--)
        finalResult|=mat1088AS[pinsCounter]*((alignedPins>>pinsCounter)&0x01);
    return finalResult;
}


#define LedMatrixPort(_16BitValue) shiftRegisterOutput(ledMatrixPinout(_16BitValue))                 // mapping the pins is still missing

void ledMatrixRender(unsigned char *matrixBuffer,unsigned short threadingRatio){
    while(threadingRatio--){
        unsigned char matrixCounter=8;
        while(matrixCounter--){
            LedMatrixPort((1<<matrixCounter)|(matrixBuffer[matrixCounter]<<8));
            microSec(100);
            LedMatrixPort(0);
        }
    }
    return;
}

unsigned char *renderText(unsigned char *textToDisplay,unsigned short frameTime){                            // returns unsigned char * for the sack of chaining to other functions
    displayText=textToDisplay;
    displayTextLength=0;
    while(textToDisplay[displayTextLength++]);
    --displayTextLength;


    unsigned short textLengthBits=displayTextLength*8;
    unsigned short colomCounter=0;
    while(textLengthBits--){
        unsigned char displayBuffer[8]="";
        unsigned char rowCounter=8;
        while(rowCounter--){
            displayBuffer[rowCounter]=outOfMemory(((rowCounter*displayTextLength)*8)+colomCounter,8);
        }

        ledMatrixRender(displayBuffer,frameTime);

        colomCounter++;
    }

    return textToDisplay;
}