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
	int64_t currentPoint=0;

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
			outputPort(value);
			return;
		}
		uint32_t registerValue=*outputRegister;
		registerValue&=~((1<<dir)|(1<<step)|(1<<en));
		registerValue|=value;
		*outputRegister=registerValue;
	}

	



	stepper(){

	}
	~stepper(){

	}
};