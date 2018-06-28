#include "avr/interrupt.h"
#include "generalVar.h"
#include "message.h"
#include "msgArray.h"
#include "myInterrupt.h"
#include <stdbool.h>
  
  unsigned char last_timer=TIMER_SHORT;  // store last timer value
  unsigned char flag=0;                  // used for short or long pulse
  bool second_isr = false;               // pulse up or down
  unsigned char state = PREAMBLE;
  unsigned char preamble_count = 16;
  unsigned char outbyte = 0;
  unsigned char cbit = 0x80;

  unsigned char preample1;   // global variabel for preample part 1
  unsigned char preample2;   // global variabel for preample part 2

  int msgIndex = 0;  
  int byteIndex = 0;
  
void SetupTimer2()
  {
    //Timer2 Settings: Timer Prescaler /8, mode 0
    //Timer clock = 16MHz/8 = 2MHz oder 0,5usec
    // 
    TCCR2A = 0; //page 203 - 206 ATmega328/P

    TCCR2B = 2; //Page 206
  
    /*     bit 2     bit 1     bit0
            0         0         0       Timer/Counter stopped 
            0         0         1       No Prescaling
            0         1         0       Prescaling by 8
            0         0         0       Prescaling by 32
            1         0         0       Prescaling by 64
            1         0         1       Prescaling by 128
            1         1         0       Prescaling by 256
            1         1         1       Prescaling by 1024
    */
    TIMSK2 = 1<<TOIE2;   //Timer2 Overflow Interrupt Enable - page 211 ATmega328/P   
    TCNT2 = TIMER_SHORT;   //load the timer for its first cycle
  }

ISR(TIMER2_OVF_vect) //Timer2 overflow interrupt vector handler
  {
    //Capture the current timer value TCTN2. This is how much error we have
    //due to interrupt latency and the work in this function
    //Reload the timer and correct for latency.  
    unsigned char latency;
  
    if (second_isr) 
    {  // for every second interupt just toggle signal
      digitalWrite(DCC_PIN,1);    //1 is high, 0 is low
      second_isr = false;    
      latency = TCNT2;    // set timer to last value
      TCNT2 = latency+last_timer; 
    }
    else  
    {  // != every second interrupt, advance bit or state
      digitalWrite(DCC_PIN,0);
      second_isr = true;
      switch(state)  
      {
        case PREAMBLE:
           flag=1; // short pulse
           preamble_count--;
           if (preamble_count == 0)  
           {  
              //Serial.print(" ");
              state = SEPERATOR; // advance to next state
              msgIndex++; // get next message
              if (msgIndex >= MAXMSG)  
              {  
                msgIndex = 0; 
              }  
              byteIndex = 0; //start msg with byte 0
           }
        break;
        case SEPERATOR:
//           Serial.print(" ");
           flag=0; // long pulse and then advance to next state
           state = SENDBYTE; // goto next byte ...
           outbyte = msg[msgIndex].data[byteIndex];
           cbit = 0x80;  // send this bit next time first
        break;
        case SENDBYTE:
           if ((outbyte & cbit)!=0)  
           { 
              flag = 1;  // send short pulse
           }
           else  
           {
              flag = 0;  // send long pulse
           }
           cbit = cbit >> 1;
           if (cbit == 0)  
           {  // last bit sent 
              byteIndex++;
              //Serial.print(" ");
              if (byteIndex >= msg[msgIndex].len) //   
              {  // this was already the XOR byte then advance to preamble
 //                Serial.println();
                 state = PREAMBLE;
                 preamble_count = 16;

              }
              else  
              {  // send separtor and advance to next byte
                 state = SEPERATOR ;
              }
           }
        break;
      } // end switch   
 
      if (flag)  
      {  // data = 1 short pulse
        latency=TCNT2;
        TCNT2=latency+TIMER_SHORT;
        last_timer=TIMER_SHORT;
//        Serial.print("1");
      }  
      else  
      {   // data = 0 long pulse
        latency=TCNT2;
        TCNT2=latency+TIMER_LONG; 
        last_timer=TIMER_LONG;
//        Serial.print("0");
      } 
    }
  }
