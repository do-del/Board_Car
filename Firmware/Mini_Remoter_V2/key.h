#ifndef KEY_H
#define KEY_H

#include <Arduino.h>
#include <OneButton.h>

#define BUTTON_LEFT 5
#define BUTTON_RIGHT 7

typedef enum
{
  event_none = 0,
  event_click,
  event_double,
  event_long
}btn_state;

extern btn_state btn_state_left;
extern btn_state btn_state_right;
extern OneButton btn_left;
extern OneButton btn_right;

void key_init();

#endif
