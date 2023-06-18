#ifndef CHANNEL_H
#define CHANNLE_H

#include <Arduino.h>

//LEFT_HOR，左边是1023，右边是0
//LEFT_VER，上是1023，下是0
//RIGHT_HOR,左是0，右边是1023
//RIGHT_VER,上是1023，下是0  

#define CH_LEFT_KEY 2
#define CH_RIGHT_KEY 3
#define CH_LEFT_HOR A3
#define CH_LEFT_VER A2
#define CH_RIGHT_HOR A1
#define CH_RIGHT_VER A0

typedef struct{
  uint16_t left_hor;
  uint16_t left_ver;
  uint16_t right_hor;
  uint16_t right_ver;
  uint8_t left_key;
  uint8_t right_key;
}channel_value;

extern channel_value ch_value;

void channel_init();
void channel_update();

#endif
