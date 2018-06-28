#ifndef MYINTERRUPT_H
#define MYINTERRUPT_H

#define DCC_PIN  4 // DCC out

//Timer frequency is 2MHz for ( /8 prescale from 16MHz )
#define TIMER_SHORT 0x8D               // 58  usec pulse length 141 255-141 = 114
#define TIMER_LONG  0x1B               // 116 usec pulse length  27 255- 27 = 228
#define PREAMBLE  0                    // definitions for state machine
#define SEPERATOR 1                    // definitions for state machine
#define SENDBYTE  2                    // definitions for state machine

void SetupTimer2();

#endif
