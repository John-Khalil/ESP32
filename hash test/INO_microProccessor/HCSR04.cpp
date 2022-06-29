#include <stdint.h>
class hcsr04{
    void (*triggerPin)(unsigned char);
    unsigned char (*echoPin)(void);
    void (*delay_us)(unsigned long);
    double soundSpeed=0.034/2;

    public:
    double calibrationFactor=1.0;
    void setup(void (*_triggerPin)(unsigned char),unsigned char (*_echoPin)(void),void (*_Delay_us)(unsigned long),double userCalibration){
        triggerPin=_triggerPin;
        echoPin=_echoPin;
        delay_us=_Delay_us;
        calibrationFactor=userCalibration;
        soundSpeed*=calibrationFactor;
    }
    void setup(void (*_triggerPin)(unsigned char),unsigned char (*_echoPin)(void),void (*_Delay_us)(unsigned long)){
        triggerPin=_triggerPin;
        echoPin=_echoPin;
        delay_us=_Delay_us;
        soundSpeed*=calibrationFactor;
    }

    unsigned short getDistance(void){
        triggerPin(1);
        delay_us(10);
        triggerPin(0);
        while(!echoPin());
        register unsigned short soundSpeedCounter=0;
        while(echoPin()&&(++soundSpeedCounter));
        return soundSpeed*soundSpeedCounter;
    }
};