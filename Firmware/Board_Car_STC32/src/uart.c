#include "uart.h"

void DebugInit(void)
{
	SCON = 0x50;
	T2L = DEBUG_BRT;
	T2H = DEBUG_BRT>>8;
	S1BRT = 1;
	T2x12 = 1;
	T2R = 1;
}

void DebugSend(char ch)
{
	while(!TI);
	TI = 0;
	SBUF = ch;
}

void DebugSendStr(char *p)
{
	while(*p)
	{
		DebugSend(*p);
		p++;
	}
}

void UartIsr() interrupt 4
{
	if(RI)
	{
		DebugSend(SBUF);
	}
}