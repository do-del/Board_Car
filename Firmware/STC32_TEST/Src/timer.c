#include "timer.h"

static void (*timer0_fun)(void);
void timer0_isr() interrupt 1
{
	timer0_fun();
}

void timer0_init(unsigned int _us, void (*p)(void))
{
	unsigned int tmp = 65536 - SYSCLK/12/_us;
	TMOD = 0x00;
	TH0= (tmp >> 8) & 0xff;
	TL0 = tmp & 0xff;
	TR0 = 1;
	ET0 = 1;
	timer0_fun = p;
}