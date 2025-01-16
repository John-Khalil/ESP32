#pragma once

#define OUTPUT_REGISTERS_BASE 0x100

#define DIGITAL_INPUT OUTPUT_REGISTERS_BASE+0x01
#define DIGITAL_INPUT_PULLUP OUTPUT_REGISTERS_BASE+0x02
#define DIGITAL_INPUT_PULLDOWN OUTPUT_REGISTERS_BASE+0x03
#define DIGITAL_OUTPUT OUTPUT_REGISTERS_BASE+0x04

#define DW1 OUTPUT_REGISTERS_BASE+0x05
#define DW0 OUTPUT_REGISTERS_BASE+0x06
#define DR OUTPUT_REGISTERS_BASE+0x07

#define TBOT_MOVE OUTPUT_REGISTERS_BASE+0x08
#define TBOT_STEPS_PER_UNINT OUTPUT_REGISTERS_BASE+0x09
#define TBOT_SETUP OUTPUT_REGISTERS_BASE+0x0A

#define CLOCK OUTPUT_REGISTERS_BASE+0x0B
#define ANALOG_READ OUTPUT_REGISTERS_BASE+0x0C

#define REGISTER_WRITE OUTPUT_REGISTERS_BASE+0x0D

#define TBOT_POS OUTPUT_REGISTERS_BASE+0x0E

#define PORTA 1
#define PORTB 2
#define PORTC 3



#define INPUT_REGISTERS_BASE 0x200

#define Pin_A0    192
#define Pin_A1    193
#define Pin_A2    194
#define Pin_A3    195
#define Pin_A4    196
#define Pin_A5    197
#define Pin_A6    198
#define Pin_A7    199
#define Pin_A8    12
#define Pin_A9    11
#define Pin_A10   10
#define Pin_A11   9
#define Pin_A12   8
#define Pin_A13   33
#define Pin_A14   34
#define Pin_A15   7
#define Pin_B0    200
#define Pin_B1    201
#define Pin_B2    32
#define Pin_B3    6
#define Pin_B4    5
#define Pin_B5    4
#define Pin_B6    3
#define Pin_B7    2
#define Pin_B8    1
#define Pin_B9    0
#define Pin_B10   30
#define Pin_B11   31
#define Pin_B12   16
#define Pin_B13   15
#define Pin_B14   14
#define Pin_B15   13
#define Pin_C13   17
#define Pin_C14   18
#define Pin_C15   19