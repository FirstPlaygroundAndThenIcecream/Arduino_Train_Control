#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct Message                         // buffer for command
  {
    unsigned char data[7];
    unsigned char len;
  } Message;

#endif


