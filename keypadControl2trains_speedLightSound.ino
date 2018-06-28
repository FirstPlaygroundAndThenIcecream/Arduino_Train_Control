#include <Keypad.h>
#include "generalVar.h"
#include "message.h"
#include "msgArray.h"
#include "dccMsg.h"
#include "myInterrupt.h"

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
  };

byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {8, 7, 6};
  
  unsigned char locoAddr1 = 36;
  unsigned char locoAddr2 = 40;
  unsigned char extraSpeed1 = 2;
  unsigned char extraSpeed2 = 2;
  
  bool light = false;
  bool bell = false;
  bool sound1 = false;
  bool sound2 = false;
  bool forceStop = false;
  unsigned char dataArray[4] = {0x67, 0x81, 0x80, 0x00};
  unsigned char *p = NULL;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  extern SetupTimer2();
  keypad.addEventListener(keypadEvent);
}

void loop() 
{  
  char key = keypad.getKey();
}

void keypadEvent(KeypadEvent key){
  switch(keypad.getState()){
    case PRESSED:
     switch(key){
      case '*':
        addr = locoAddr1;
        data = 0X62 + extraSpeed1;
        extraSpeed1 += extraSpeed1;
        printFunc(addr, " speed ", data);
        if(extraSpeed1 > 16){
          data = 0x46;
          extraSpeed1 = 2;
          Serial.println("  backwards");
        }        
        extern assemble_dcc_msg();
        break;
        
      case '#':
        addr = locoAddr2;
        data = 0X62 + extraSpeed2;
        extraSpeed2 += extraSpeed2;
        printFunc(addr, " speed ", data);
        if(extraSpeed2 > 16){
          data = 0x46;
          extraSpeed2 = 2;
          Serial.println("  backwards");
        }        
        extern assemble_dcc_msg();
        break;

      case '0':
        addr = locoAddr1;
        data = 0x00;
        extern assemble_dcc_msg();
        extraSpeed1 = 2;
        Serial.println("stop all");
      break;

      case '1':
        addr = locoAddr2;
        light = !light;
        if(light){
          data = 0X90;
          printFunc(addr, " light on");
          }else{
            data = 0X80;
            printFunc(addr, " light off");
            }
        extern assemble_dcc_msg();
      break;

      case '2':
        addr = locoAddr2;
        sound2 = !sound2;
        if(sound2){
          data = 0X81;
          printFunc(addr, " sound on");
          }else{
            data = 0X80;
            printFunc(addr, " sound off");  
          }
        extern assemble_dcc_msg();
      break;

      case '3':
        addr = locoAddr1;
        bell = !bell;
        if(bell){
          data = 0X88;
          printFunc(addr, " bell on");
          }else{
            data = 0X80;
            printFunc(addr, " bell off");
          }
        extern assemble_dcc_msg();
      break;

      case '4':
        addr = locoAddr1;
        sound1 = !sound1;
        if(sound1){
          data = 0X81;
          printFunc(addr, " sound on");
          }else{
            data = 0X80;
            printFunc(addr, " sound off");
          }
        extern assemble_dcc_msg();
      break;

      case '5':
        addr = 0x9A;
        data = 0xF8;
        extern assemble_dcc_msg();
        Serial.println("101 turn");
      break;

      case '6':
        addr = 0x9A;
        data = 0xF9;
        extern assemble_dcc_msg();
        Serial.println("101 straight");
      break;

      case '7':
        addr = 0x9A;
        data = 0xFA;
        extern assemble_dcc_msg();
        Serial.println("102 turn");
      break;

      case '8':
        addr = 0x9A;
        data = 0xFB;
        extern assemble_dcc_msg();
        Serial.println("102 straight");
      break;

      case '9':
        addr = locoAddr2;
        forceStop = !forceStop;
        if(forceStop){
          Serial.println("drive automatically");
          p = &dataArray[0];
          for(char i = 0; i < 4; i++, p++){
            data = *p;
            extern assemble_dcc_msg();
            printFunc(addr, ", data: ", data );
            if(i==1){
              delay(6000);
              }else if(i != 3){
                delay(14000);
                }
            }          
          }else{
            data = 0x00;
            extern assemble_dcc_msg();
            Serial.println("stop drive automatically");
            }
      break;      
      }
    break;

    case RELEASED:
      switch(key){
        case '0':
          addr = locoAddr2;
          data = 0x00;
          extern assemble_dcc_msg();
          extraSpeed2 = 2;
        break;

        case '5':
          addr = 0x9A;
          data = 0xF0;
          extern assemble_dcc_msg();
        break;

        case '6':
          addr = 0x9A;
          data = 0xF1;
          extern assemble_dcc_msg();
        break;

        case '7':
          addr = 0x9A;
          data = 0xF2;
          extern assemble_dcc_msg();
        break;

        case '8':
          addr = 0x9A;
          data = 0xF3;
          extern assemble_dcc_msg();
        break;
        }
    break;
    
    case HOLD:
      switch(key){
        case '*':
         addr = locoAddr1; 
         data = 0x00;
         extraSpeed1 = 2;           
         printFunc(addr, " STOP");
         extern assemble_dcc_msg();
        break;

        case '#':
         addr = locoAddr2; 
         data = 0x00;
         extraSpeed2 = 2;
         printFunc(addr, " STOP");           
         extern assemble_dcc_msg();
        break;
        }
    break;
  }
}

void printFunc(unsigned char info, char text[]){
  Serial.print(info);
  Serial.println(text);
  }

void printFunc(unsigned char info_1, char text[], unsigned char info_2){
  Serial.print(info_1);
  Serial.print(text);
  Serial.println(info_2);
  }




