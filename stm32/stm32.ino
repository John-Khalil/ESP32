#include "custom/pointerTool.h"
#include "custom/smartMemory.cpp"


// #include <iostream>
#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

int main(void){
    Serial.begin(115200);
    // Serial.print 
    return 0;
}

// // Pin definitions
// const int interruptPin = PB12;  // Pin to attach interrupt to (can be any GPIO pin)

// // Variable to store the pin state
// volatile uint32_t pinState = 0;

// void setup() {
//   // Configure the pin as an input
//   pinMode(interruptPin, INPUT_PULLUP);
//   pinMode(PC13, OUTPUT);
//   digitalWrite(PC13,1);
//   // Attach the interrupt (trigger on RISING edge)
//   attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);

//   // Start serial communication for debugging
//   Serial.begin(9600);
// }

// void loop() {
//   // Just print the pin state
//   Serial.println(pinState);
// //   delay(250);
// //   digitalWrite(PC13,1);
// //   delay(250);
// //   digitalWrite(PC13,0);

//     *((volatile uint32_t *)0x40011010)|=(1<<13);
//     delay(250);
//     *((volatile uint32_t *)0x40011012)|=(1<<13);
//     delay(250);
    

// }

// // Interrupt service routine (ISR)
// void handleInterrupt() {
//   // Read the pin state
//   pinState++;
// //   digitalWrite(PC13,pinState);
// }
