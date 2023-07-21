#include "uart.h"

void uart_init(void)
{
	SCON = 0x50;
	T2L = BRT;
	T2H = BRT >> 8;
	S1BRT = 1;
	T2x12 = 1;
	T2R = 1;
	ES = 1;
}

void uart_send(char ch)
{
	SBUF = ch;
	while(!TI);
}

void uart_sendStr(char *p)
{
	while(*p)
	{
		uart_send(*p++);
	}
}

void uart_sendNum(unsigned long num)
{
	unsigned char length = 0;
	unsigned char buf[32] = {0};
	while(num / 10)
	{
		buf[length++] = num % 10 + 48;
		num /= 10;
	}
	buf[length] = num + 48;
	
	while(length)
	{
		uart_send(buf[length--]);
	}
	uart_send(buf[length]);
}

void UartIsr() interrupt 4
{
	if(TI)
	{
		TI = 0;
	}
}