#pragma once


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

class stepper{

	int64_t refPoint=0;
	volatile int64_t currentPoint=0;

	std::function<void(void)> checkPosition=nullptr;

	float stepsPerUnit=1;

	uint8_t dir;
	uint8_t step;
	uint8_t en;
	volatile uint32_t *outputRegister=nullptr;
	uint32_t portValue=0;
	std::function<void(uint32_t)>externalPort=nullptr;

	void outputPort(uint32_t value){
		if(externalPort!=nullptr){
			externalPort(value);
			return;
		}
		uint32_t registerValue=*outputRegister;
		registerValue&=~((1<<dir)|(1<<step)|(1<<en));
		registerValue|=value;
		*outputRegister=registerValue;
	}

	#define OUTPUT_PORT(value) do { \
		if (externalPort != nullptr) { \
			externalPort(value); \
			return; \
		} \
		uint32_t registerValue = *outputRegister; \
		registerValue &= ~((1 << dir) | (1 << step) | (1 << en)); \
		registerValue |= (value); \
		*outputRegister = registerValue; \
	} while(0)


	void move(void){
		portValue|=(1<<en);
		if(checkPosition==nullptr){
			while(refPoint!=currentPoint){
				if(refPoint>currentPoint){
					portValue|=(1<<dir);
					outputPort(portValue^=(1<<step));
					currentPoint++;
				}
				else if(refPoint<currentPoint){
					portValue&=~(1<<dir);
					outputPort(portValue^=(1<<step));
					currentPoint--;
				}
			}
		}
		else{
			while(refPoint!=currentPoint){
				if(refPoint>currentPoint){
					portValue|=(1<<dir);
					outputPort(portValue^=(1<<step));
					checkPosition();
					currentPoint++;
				}
				else if(refPoint<currentPoint){
					portValue&=~(1<<dir);
					outputPort(portValue^=(1<<step));
					checkPosition();
					currentPoint--;
				}
			}
		}
	}



	stepper(){

	}
	~stepper(){

	}
};