#ifndef consoleLogger_h
#define consoleLogger_h

#include <stdint.h>


class consoleLogger
{
public:
    void (*_clkPin)(unsigned char);
    void (*_dataPin)(unsigned char);
    void (*_Delay_us)(unsigned long);
    unsigned char (*_syncPin)(void);
    float _clkSpeed=0;
    unsigned char autoNLCR=0;
    unsigned short CLR_LENGTH=0;
    unsigned char * CLR(unsigned char *deletedString);
    unsigned char inttostr(unsigned long num, unsigned char *str);
    unsigned char _globalStringNameThatYouWillNeverUse[11]="";
    unsigned char* inttostring(unsigned long num);
    unsigned char* longToString(long num);
    unsigned char *_spiConsoleLog(unsigned char *consoleData);


// public:
    // consoleLogger(/* args */);
    // ~consoleLogger();
    void setup(void (*clkPin)(unsigned char),void (*dataPin)(unsigned char),unsigned char (*syncPin)(void),void (*delayMicroSec)(unsigned long),unsigned long clkSpeed);
    void disableNL(void);
    void enableNL(void);
    unsigned char *log(unsigned char *consoleData);
    unsigned char *log(char *consoleData);
    unsigned char *log(unsigned long consoleData);
    unsigned char *log(double consoleData);
    void log(unsigned char *consoleText,unsigned long consoleNum);
    void log(unsigned char *consoleText,double consoleNum);
    void log(unsigned char *consoleText,unsigned char *consoleNum);
    void log(unsigned char *consoleText,char *consoleNum);
    void log(char *consoleText,unsigned long consoleNum);
    void log(char *consoleText,double consoleNum);
    void log(char *consoleText,unsigned char *consoleNum);
    void log(char *consoleText,char *consoleNum);    
}console;

// consoleLogger::consoleLogger(/* args */)
// {
// }

// consoleLogger::~consoleLogger()
// {
// }

#endif