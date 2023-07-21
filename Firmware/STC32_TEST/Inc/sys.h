#ifndef SYS_H
#define SYS_H

#include "stc32g.h"
#include "intrins.h"

#define MAIN_FOSC 12000000UL

#define SELECT_PWM 1
#if (SELECT_PWM == 1)
#define USE_PWM 1
#define USE_IO 0
#else
#define USE_PWM 0
#define USE_IO 1
#endif

void delay_ms(unsigned short ms);

#endif
