#ifndef TIMER_H
#define TIMER_H

#include "stc32g.h"
#include "intrins.h"

#define SYSCLK 24000000UL

void timer0_init(unsigned int _us, void (*p)(void));

#endif
