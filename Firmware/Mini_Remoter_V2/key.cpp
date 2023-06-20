#include "key.h"

btn_state btn_state_left;
btn_state btn_state_right;

OneButton btn_left = OneButton(BUTTON_LEFT,true,true);
OneButton btn_right = OneButton(BUTTON_RIGHT,true,true);

static void btn_left_click_handle()
{
  btn_state_left = btn_state::event_click;
}

static void btn_left_double_handle()
{
  btn_state_left = btn_state::event_double;
}

static void btn_left_long_handle()
{
  btn_state_left = btn_state::event_long;
}

static void btn_right_clcik_handle()
{
  btn_state_right = btn_state::event_click;
}

static void btn_right_double_handle()
{
  btn_state_right = btn_state::event_double;
}

static void btn_right_long_handle()
{
  btn_state_right = btn_state::event_long;
}

void key_init()
{
  btn_left.attachClick(btn_left_click_handle);
  btn_left.attachDoubleClick(btn_left_double_handle);
  btn_left.attachLongPressStart(btn_left_long_handle);

  btn_right.attachClick(btn_right_clcik_handle);
  btn_right.attachDoubleClick(btn_right_double_handle);
  btn_right.attachLongPressStart(btn_right_long_handle);
}