#include "generalVar.h"
#include "message.h"
#include "msgArray.h"

void assemble_dcc_msg(void) 
  {
    unsigned char xdata;
    noInterrupts();  // make sure that only "matching" parts of the message are used in ISR
    xdata = addr ^ data;
//    Serial.print("addr ");
//    Serial.print(addr);
//    Serial.print(" ");
//    Serial.print(data);
//    Serial.println();
    msg[1].data[0] = addr; 
    msg[1].data[1] = data;
    msg[1].data[2] = xdata;
    interrupts();
  }

