#include <stdint.h>
#include <cstdlib>
#include <functional>
class spiConsole{

    // void (*_clkPin)(unsigned char);
    // void (*_dataPin)(unsigned char);
    // void (*_Delay_us)(unsigned long);
    // unsigned char (*_syncPin)(void);

    std::function<void(unsigned char)>_clkPin;
    std::function<void(unsigned char)>_dataPin;
    std::function<unsigned char(void)>_syncPin;
    std::function<void(unsigned long)>_Delay_us;


    float _clkSpeed=0;

    unsigned char autoNLCR=1;


    unsigned short CLR_LENGTH=0;									//this value will be reseted to zero after clearing the string/uint_8 pointer
    unsigned char * CLR(unsigned char *deletedString){
        unsigned char *returnedString=deletedString;
        while(*deletedString||(CLR_LENGTH-=(CLR_LENGTH!=0))){
            *deletedString=0;
            deletedString++;	
        }
        return returnedString;
    }
    
    
    unsigned short checkSum(unsigned char *packetInBytes){
        unsigned long packetCounter=0;
        unsigned short finalCheckSum=0;
        while(packetInBytes[packetCounter])
            finalCheckSum+=(packetInBytes[packetCounter++]=packetInBytes[packetCounter++]);
        return finalCheckSum;
    }

    unsigned char inttostr(unsigned long num, unsigned char *str) {
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



    unsigned char *_globalStringNameThatYouWillNeverUse=(unsigned char*)calloc(11,sizeof(unsigned char));
    unsigned char* inttostring(unsigned long num) {
        CLR(_globalStringNameThatYouWillNeverUse);
        inttostr(num, _globalStringNameThatYouWillNeverUse);
        return _globalStringNameThatYouWillNeverUse;
    }


    unsigned char* longToString(long num){
        unsigned char *signedStr=_globalStringNameThatYouWillNeverUse;
        if(num<0){
            *signedStr=0x2D;
            signedStr++;
            num*=(-1);
        }
        inttostr((unsigned long)num, signedStr);
        return _globalStringNameThatYouWillNeverUse;
    }


    unsigned char *_spiConsoleLog(unsigned char *consoleData){
        unsigned char *loggedData=consoleData;
        while(!_syncPin());
        _dataPin(0);
        _Delay_us(60);										//to sync with the receiver
        nlcrStartOver:
        while(*consoleData){
            unsigned char spiBitCounter=8;
            while(spiBitCounter--){
                _dataPin((*consoleData&(1<<spiBitCounter))!=0);
                _Delay_us(_clkSpeed*0.4);
                _clkPin(1);
                _Delay_us(_clkSpeed);
                _clkPin(0);
                _Delay_us(_clkSpeed*0.6);
            }
            consoleData++;
            // static unsigned char spiConsoleByteCounter;
            // if(!(--spiConsoleByteCounter))
            //     _Delay_us(10);
        }
        if(autoNLCR){
            unsigned char *nlcrStr=(unsigned char*)"\r\n";
            if((consoleData-2)!=nlcrStr){
                consoleData=nlcrStr;
                goto nlcrStartOver;
            }
        }
        // unsigned short finalPacketChe 
        // unsigned char checkSumAdd[3]={}

        _dataPin(1);
        return loggedData;
    }









    public:
        // void setup(void (*clkPin)(unsigned char),void (*dataPin)(unsigned char),unsigned char (*syncPin)(void),void (*delayMicroSec)(unsigned long),unsigned long clkSpeed){
        //     _clkPin=clkPin;
        //     _dataPin=dataPin;
        //     _Delay_us=delayMicroSec;
        //     _syncPin=syncPin;
        //     _clkSpeed=(1e6f/clkSpeed)/2;
        // }
        void setup(const std::function<void(unsigned char)>&clkPin,const std::function<void(unsigned char)>&dataPin,const std::function<unsigned char(void)>&syncPin,const std::function<void(unsigned long)>&delayMicroSec,unsigned long clkSpeed){
            _clkPin=clkPin;
            _dataPin=dataPin;
            _Delay_us=delayMicroSec;
            _syncPin=syncPin;
            _clkSpeed=(1e6f/clkSpeed)/2;
        }
        void disableNL(void){
            autoNLCR=0;
        }
        void enableNL(void){
            autoNLCR=1;
        }

        unsigned char *log(unsigned char *consoleData){
            return _spiConsoleLog(consoleData);
        }
        unsigned char *log(char *consoleData){
            return _spiConsoleLog((unsigned char*)consoleData);
        }
        unsigned char *log(const char *consoleData){
            return _spiConsoleLog((unsigned char*)consoleData);
        }
        // unsigned char *log(int32_t consoleData){
        //     return _spiConsoleLog(longToString(consoleData));
        // }
        // unsigned char *log(uint32_t consoleData){
        //     return _spiConsoleLog(inttostring(consoleData));
        // }
        unsigned char *log(unsigned long consoleData){
            return _spiConsoleLog(inttostring(consoleData));
        }

        unsigned char *log(unsigned short consoleData){
            return _spiConsoleLog(inttostring(consoleData));
        }

        unsigned char *log(unsigned char consoleData){
            return _spiConsoleLog(inttostring(consoleData));
        }

        unsigned char *log(long consoleData){
            return _spiConsoleLog(longToString(consoleData));
        }

        unsigned char *log(short consoleData){
            return _spiConsoleLog(longToString(consoleData));
        }

        unsigned char *log(char consoleData){
            return _spiConsoleLog(longToString(consoleData));
        }

        // unsigned char *log(int consoleData){
        //     return _spiConsoleLog(inttostring(consoleData));
        // }
        unsigned char *log(double consoleData){
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
            return _spiConsoleLog(biggerNumber);
             
        }

        // void log(unsigned char *consoleText,int32_t consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // void log(unsigned char *consoleText,uint32_t consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }



        void log(void){

        }

        template<typename T,typename... Types>
        void log(T arg1,Types... arg2){
            disableNL();
            log(arg1);
            autoNLCR=(sizeof...(Types)==1);
            log(arg2...);
        }














        // template<typename T1,typename T2>
        // void log(T1 consoleText,T2 consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // template<typename T1,typename T2,typename T3>
        // void log(T1 arg1,T2 arg2,T3 arg3){
        //     disableNL();
		// 	log(arg1);
        //     log(arg2);
        //     enableNL();
		// 	log(arg3);
		// }

        // template<typename T1,typename T2,typename T3,typename T4>
        // void log(T1 arg1,T2 arg2,T3 arg3,T4 arg4){
        //     disableNL();
		// 	log(arg1);
        //     log(arg2);
        //     log(arg3);
        //     enableNL();
		// 	log(arg4);
		// }

        // void log(unsigned char *consoleText,unsigned long consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // // void log(unsigned char *consoleText,int consoleNum){
        // //     disableNL();
		// // 	log(consoleText);
        // //     enableNL();
		// // 	log(consoleNum);
		// // }

        // void log(unsigned char *consoleText,double consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // void log(unsigned char *consoleText,unsigned char *consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }
        // void log(unsigned char *consoleText,char *consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // // void log(char *consoleText,int32_t consoleNum){
        // //     disableNL();
		// // 	log(consoleText);
        // //     enableNL();
		// // 	log(consoleNum);
		// // }

        // // void log(char *consoleText,uint32_t consoleNum){
        // //     disableNL();
		// // 	log(consoleText);
        // //     enableNL();
		// // 	log(consoleNum);
		// // }

        // void log(char *consoleText,unsigned long consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // // void log(char *consoleText,int consoleNum){
        // //     disableNL();
		// // 	log(consoleText);
        // //     enableNL();
		// // 	log(consoleNum);
		// // }

        // void log(char *consoleText,double consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }

        // void log(char *consoleText,unsigned char *consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }
        // void log(char *consoleText,char *consoleNum){
        //     disableNL();
		// 	log(consoleText);
        //     enableNL();
		// 	log(consoleNum);
		// }
		
};