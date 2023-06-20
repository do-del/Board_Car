#include "channel.h"

channel_value ch_value;

void channel_init()
{
  pinMode(CH_LEFT_KEY,INPUT_PULLUP);
  pinMode(CH_RIGHT_KEY,INPUT_PULLUP);
}

void channel_update()
{
  ch_value.left_key = (PIND & (1<<2)) ? 1 : 0;
  ch_value.right_key = (PIND & (1<<3)) ? 1 : 0;
  ch_value.left_hor = analogRead(CH_LEFT_HOR);
  ch_value.left_ver = analogRead(CH_LEFT_VER);
  ch_value.right_hor = analogRead(CH_RIGHT_HOR);
  ch_value.right_ver = analogRead(CH_RIGHT_VER);
}
