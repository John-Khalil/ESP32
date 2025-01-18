#pragma once

#include "platformUtils.cpp"

#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

#define HOME_FEED_RATE 100

class tBot{
    private:
        volatile uint32_t* OUTPUT_SET;
        volatile uint32_t* OUTPUT_RESET;
        uint32_t en;
        uint32_t dir1;
        uint32_t dir2;
        uint32_t clk;

        uint32_t homeSwitchD1;
        uint32_t homeSwitchD2;

    public:

        uint16_t stepsPerUnit=1;
        struct point{
            int32_t d1=0;
            int32_t d2=0;
        };
        point refPoint;
        point currentPoint;

        void followRef(uint32_t delayTime){
            *OUTPUT_RESET|=en;
            if(refPoint.d1!=currentPoint.d1){
                while(refPoint.d1!=currentPoint.d1){
                    *OUTPUT_SET|=(currentPoint.d1<refPoint.d1)?dir1:dir2;
                    *OUTPUT_RESET|=(currentPoint.d1>refPoint.d1)?dir1:dir2;

                    *OUTPUT_SET|=clk;
                    HW_DELAY(delayTime);
                    *OUTPUT_RESET|=clk;
                    HW_DELAY(delayTime);

                    (currentPoint.d1<refPoint.d1)?currentPoint.d1++:currentPoint.d1--;
                }
            }
            if(refPoint.d2!=currentPoint.d2){
                while(refPoint.d2!=currentPoint.d2){
                    (currentPoint.d2<refPoint.d2)?(*OUTPUT_RESET|=(dir1|dir2)):(*OUTPUT_SET|=(dir1|dir2));

                    *OUTPUT_SET|=clk;
                    HW_DELAY(delayTime);
                    *OUTPUT_RESET|=clk;
                    HW_DELAY(delayTime);
                    
                    (currentPoint.d2<refPoint.d2)?currentPoint.d2++:currentPoint.d2--;
                }
            }
            return;
        }

        void disable(void){
            *OUTPUT_SET|=en;
            return;
        }

        tBot& homeD1(void){
            refPoint.d1=0;
            *OUTPUT_SET|=(currentPoint.d1<refPoint.d1)?dir1:dir2;
            *OUTPUT_RESET|=(currentPoint.d1>refPoint.d1)?dir1:dir2;

            while(digitalRead(homeSwitchD1)){
                *OUTPUT_SET|=clk;
                HW_DELAY(HOME_FEED_RATE);
                *OUTPUT_RESET|=clk;
                HW_DELAY(HOME_FEED_RATE);
            }
            
            currentPoint.d1=0;
            return (*this);
        }

        tBot& homeD2(void){
            refPoint.d2=0;
            (currentPoint.d2<refPoint.d2)?(*OUTPUT_RESET|=(dir1|dir2)):(*OUTPUT_SET|=(dir1|dir2));

            while(digitalRead(homeSwitchD2)){
                *OUTPUT_SET|=clk;
                HW_DELAY(HOME_FEED_RATE);
                *OUTPUT_RESET|=clk;
                HW_DELAY(HOME_FEED_RATE);
            }
            
            currentPoint.d2=0;
            return (*this);
        }

        tBot& setStepsPerUnit(uint16_t _stepsPerUnit){
            stepsPerUnit=_stepsPerUnit;
            
            currentPoint.d1=refPoint.d1;
            currentPoint.d2=refPoint.d2;

            return (*this);
        }

        tBot& move(uint32_t d1,uint32_t d2,uint16_t feedRate){
            refPoint.d1=d1*stepsPerUnit;
            refPoint.d2=d2*stepsPerUnit;

            followRef(500000/(feedRate*stepsPerUnit));

            refPoint.d1/=stepsPerUnit;
            refPoint.d2/=stepsPerUnit;

            return (*this);
        }

        void setup(
            volatile uint32_t* _output_set,
            volatile uint32_t* _output_reset,
            uint32_t _en,
            uint32_t _dir1,
            uint32_t _dir2,
            uint32_t _clk1,
            uint32_t _clk2,
            uint16_t _stepsPerUnit,
            uint32_t _homeSwitchD1,
            uint32_t _homeSwitchD2
        ){
            OUTPUT_SET=_output_set;
            OUTPUT_RESET=_output_reset;
            en=(1<<_en);
            dir1=(1<<_dir1);
            dir2=(1<<_dir2);
            clk=(1<<_clk1)|(1<<_clk2);
            setStepsPerUnit(_stepsPerUnit);
            homeSwitchD1=_homeSwitchD1;
            homeSwitchD2=_homeSwitchD2;

        }



        tBot(
            volatile uint32_t* _output_set,
            volatile uint32_t* _output_reset,
            uint32_t _en,
            uint32_t _dir1,
            uint32_t _dir2,
            uint32_t _clk1,
            uint32_t _clk2,
            uint16_t _stepsPerUnit,
            uint32_t _homeSwitchD1,
            uint32_t _homeSwitchD2
        ){
            setup(
                _output_set,
                _output_reset,
                _en,
                _dir1,
                _dir2,
                _clk1,
                _clk2,
                _stepsPerUnit,
                _homeSwitchD1,
                _homeSwitchD2
            );
        }
        tBot(){

        }
        ~tBot(){
            
        }

};

tBot tbot; 