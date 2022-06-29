#include <stdint.h>
class extendedIO{
    

    void (*_delay_us)(unsigned long);
    // _delay_us=microSec;

  

    void (*clkPin)(unsigned char);
    void (*dataDirection)(unsigned char);           //0 input, 1 output
    void (*latchPin)(unsigned char);
    void (*latchPinRead)(unsigned char);
    void (*outputPin)(unsigned char);
    unsigned char (*inputPin)(void);

    // dataDirection=_dataDirection;

    public:
        float clkSpeed=1e6f/5e6f;
        unsigned char portWidth=15;
        #define clkDuration() _delay_us(clkSpeed/2)

        void setup(void (*userClk)(unsigned char),void (*userLatch)(unsigned char),void (*userLatchRead)(unsigned char),void (*userOutput)(unsigned char),unsigned char (*userInput)(void),void (*setDirection)(unsigned char),unsigned char _portWidth,void (*calibratedDelay)(unsigned long),double userDefinedClkSpeed){
            clkPin=userClk;
            latchPin=userLatch;
            latchPinRead=userLatchRead;
            outputPin=userOutput;
            inputPin=userInput;
            dataDirection=setDirection;
            portWidth=_portWidth;
            _delay_us=calibratedDelay;
            clkSpeed=1e6f/userDefinedClkSpeed;

            latchPin(0);
            latchPinRead(0);
            clkPin(0);
            outputPin(0);
            dataDirection(1);
        }

        
        // void setup(void (*userClk)(unsigned char),void (*userLatch)(unsigned char),void (*userLatchRead)(unsigned char),void (*userOutput)(unsigned char),unsigned char (*userInput)(void),void (*setDirection)(unsigned char)){
        //     clkPin=userClk;
        //     latchPin=userLatch;
        //     latchPinRead=userLatchRead;
        //     outputPin=userOutput;
        //     inputPin=userInput;
        //     dataDirection=setDirection;

        //     latchPin(0);
        //     latchPinRead(0);
        //     clkPin(0);
        //     outputPin(0);
        //     dataDirection(1);
        // }

        // void setup(void (*userClk)(unsigned char),void (*userLatch)(unsigned char),void (*userLatchRead)(unsigned char),void (*userOutput)(unsigned char),unsigned char (*userInput)(void)){
        //     clkPin=userClk;
        //     latchPin=userLatch;
        //     latchPinRead=userLatchRead;
        //     outputPin=userOutput;
        //     inputPin=userInput;

        //     latchPin(0);
        //     latchPinRead(0);
        //     clkPin(0);
        //     outputPin(0);
        //     dataDirection(1);
        // }

        unsigned long read(void){
            dataDirection(0);
            unsigned long portValue=0;
            latchPinRead(1);
            clkDuration();
            latchPinRead(0);
            clkDuration();
            latchPinRead(1);
            clkDuration();
            latchPinRead(0);
            unsigned char loopCounter=0;
            while(loopCounter<portWidth){
                clkPin(1);
                clkDuration();
                clkPin(0);
                clkDuration();
                portValue|=(inputPin()!=0)<<loopCounter++;
            }
            return portValue;
        }

        unsigned long write(unsigned long portValue){
            dataDirection(1);
            unsigned char loopCounter=portWidth;
            while(loopCounter--){
                outputPin((portValue&(1<<loopCounter))!=0);
                clkPin(1);
                clkDuration();
                clkPin(0);
                clkDuration();
            }
            latchPin(1);
            clkDuration();
            latchPin(0);
            return portValue;
        }

};