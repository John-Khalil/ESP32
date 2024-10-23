#pragma once

#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>
#include <Arduino.h>

#define DW digitalWrite
#define PM pinMode

#define GPIOA_BSRR ((volatile uint32_t *)0x40010810)
#define GPIOB_BSRR ((volatile uint32_t *)0x40010C10)
#define GPIOC_BSRR ((volatile uint32_t *)0x40011010)
#define GPIOA_BRR ((volatile uint32_t *)0x40010814)
#define GPIOB_BRR ((volatile uint32_t *)0x40010C14)
#define GPIOC_BRR ((volatile uint32_t *)0x40011014)
/*
    ^ blackpill -- no brr
    #define GPIOA_BSRR ((volatile uint32_t *)0x40020018)
    #define GPIOB_BSRR ((volatile uint32_t *)0x40020418)
    #define GPIOC_BSRR ((volatile uint32_t *)0x40020818)
*/

HardwareTimer *Timer1 = new HardwareTimer(TIM1);

volatile uint64_t TIMER_ACCUMULATOR=0;
#define GET_TIMER() ((uint64_t)(TIMER_ACCUMULATOR|Timer1->getCount()))

void onTimerInterrupt() {
    TIMER_ACCUMULATOR+=65536;   //adding 2^16
}

void setupTimer(void){
    Timer1->setPrescaleFactor(72);
    Timer1->setOverflow(65535);
    Timer1->attachInterrupt(onTimerInterrupt);
    Timer1->resume();
    return;
}

void inputInterrupt(
        uint32_t interruptPin,
        const std::function<void(void)>& interruptHandler,
        uint32_t interruptMode=CHANGE,
        uint32_t inputPinMode=INPUT_PULLUP
    ){

    pinMode(interruptPin, inputPinMode);
    attachInterrupt(digitalPinToInterrupt(interruptPin),interruptHandler, interruptMode);
    
    return;
}