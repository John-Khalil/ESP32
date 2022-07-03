#include "consoleLogger.h"
// float consoleLogger::_clkSpeed=0;


unsigned char * consoleLogger::CLR(unsigned char *deletedString){
    unsigned char *returnedString=deletedString;
    while(*deletedString||(consoleLogger::CLR_LENGTH-=(consoleLogger::CLR_LENGTH!=0))){
        *deletedString=0;
        deletedString++;	
    }
    return returnedString;
}


 unsigned char consoleLogger::inttostr(unsigned long num, unsigned char *str) {
    static unsigned char ucoun;
    for (unsigned char clrCoun = 0; clrCoun < ucoun; clrCoun++) {
        str[clrCoun] = 0;
    }
    ucoun = 0;
    if (num) {
        unsigned long sum = 0;
        unsigned long r = 10000000;
        for (unsigned char i = 0; i < 8; i++) {
            unsigned char flg = 0;
            for (unsigned char j = 9; j > 0; j--) {
                if ((num - sum) >= r * j) {
                    sum += r * j;
                    *str = (j)+0x30;
                    ucoun++;
                    str++;
                    flg = 1;
                    break;
                }
            }
            if (flg == 0 && sum != 0) {
                *str = 0x30;
                ucoun++;
                str++;
            }
            r /= 10;
        }
    }
    else {
        *str = 0x30;
        ucoun = 1;
    }
    return ucoun;
}

unsigned char* consoleLogger::inttostring(unsigned long num) {
    consoleLogger::CLR(consoleLogger::_globalStringNameThatYouWillNeverUse);
    consoleLogger::inttostr(num, consoleLogger::_globalStringNameThatYouWillNeverUse);
    return consoleLogger::_globalStringNameThatYouWillNeverUse;
}

unsigned char *consoleLogger::_spiConsoleLog(unsigned char *consoleData){
    unsigned char *loggedData=consoleData;
    while(!consoleLogger::_syncPin());
    consoleLogger::_dataPin(0);
    consoleLogger::_Delay_us(60);										//to sync with the receiver
    nlcrStartOver:
    while(*consoleData){
        unsigned char spiBitCounter=8;
        while(spiBitCounter--){
            consoleLogger::_dataPin((*consoleData&(1<<spiBitCounter))!=0);
            consoleLogger::_Delay_us(consoleLogger::_clkSpeed*0.4);
            consoleLogger::_clkPin(1);
            consoleLogger::_Delay_us(consoleLogger::_clkSpeed);
            consoleLogger::_clkPin(0);
            consoleLogger::_Delay_us(consoleLogger::_clkSpeed*0.6);
        }
        consoleData++;
    }
    if(consoleLogger::autoNLCR){
        unsigned char *nlcrStr=(unsigned char*)"\r\n";
        if((consoleData-2)!=nlcrStr){
            consoleData=nlcrStr;
            goto nlcrStartOver;
        }
    }
    consoleLogger::_dataPin(1);
    return loggedData;
}

void consoleLogger::setup(void (*clkPin)(unsigned char),void (*dataPin)(unsigned char),unsigned char (*syncPin)(void),void (*delayMicroSec)(unsigned long),unsigned long clkSpeed){
    consoleLogger::_clkPin=clkPin;
    consoleLogger::_dataPin=dataPin;
    consoleLogger::_Delay_us=delayMicroSec;
    consoleLogger::_syncPin=syncPin;
    consoleLogger::_clkSpeed=(1e6f/clkSpeed)/2;
}


void consoleLogger::disableNL(void){
    autoNLCR=0;
}
void consoleLogger::enableNL(void){
    autoNLCR=1;
}

unsigned char *consoleLogger::log(unsigned char *consoleData){
    return _spiConsoleLog(consoleData);
}
unsigned char *consoleLogger::log(char *consoleData){
    return _spiConsoleLog((unsigned char*)consoleData);
}

unsigned char *consoleLogger::log(unsigned long consoleData){
    return _spiConsoleLog(inttostring(consoleData));
}

unsigned char *consoleLogger::log(double consoleData){
    #define extraDigits 5
    const float decimalPlace=1e5f;
    unsigned char *biggerNumber=longToString(consoleData*decimalPlace);
    unsigned char biggerNumberCharCount=0;
    while(biggerNumber[biggerNumberCharCount++]);
    // biggerNumberCharCount--;                                                             //not sure why this is cancelled
    unsigned char decimalPointIndex=(--biggerNumberCharCount)-extraDigits;
    unsigned char endsWithZero=1;
    while((biggerNumberCharCount--)-decimalPointIndex){
        biggerNumber[biggerNumberCharCount+1]=biggerNumber[biggerNumberCharCount];
        if(endsWithZero){
            endsWithZero=!(biggerNumber[biggerNumberCharCount]-0x30);
            biggerNumber[biggerNumberCharCount+1]*=(biggerNumber[biggerNumberCharCount+1]!=0x30);
        }
    }    
    biggerNumber[decimalPointIndex]=0x2E;                                                   //finally adding the decimal point
    if(!biggerNumber[decimalPointIndex+1])
        biggerNumber[decimalPointIndex+1]=0x30;
    _spiConsoleLog(biggerNumber);
}



void consoleLogger::log(unsigned char *consoleText,unsigned long consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}



void consoleLogger::log(unsigned char *consoleText,double consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}

void consoleLogger::log(unsigned char *consoleText,unsigned char *consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}
void consoleLogger::log(unsigned char *consoleText,char *consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}



void consoleLogger::log(char *consoleText,unsigned long consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}

void consoleLogger::log(char *consoleText,double consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}

void consoleLogger::log(char *consoleText,unsigned char *consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}
void consoleLogger::log(char *consoleText,char *consoleNum){
    disableNL();
    log(consoleText);
    enableNL();
    log(consoleNum);
}

// consoleLogger::consoleLogger(/* args */)
// {
// }

// consoleLogger::~consoleLogger()
// {
// }

consoleLogger console;