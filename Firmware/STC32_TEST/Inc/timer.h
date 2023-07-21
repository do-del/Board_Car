#ifndef TIMER_H
#define TIMER_H

#include "stc32g.h"
#include "intrins.h"
#include "sys.h"

#define SYSCLK MAIN_FOSC

void timer0_init(unsigned int _us, void (*p)(void));

#endif
