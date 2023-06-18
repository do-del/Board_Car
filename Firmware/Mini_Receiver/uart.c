#include "uart.h"

char rx_count;	//���ڽ������ݳ���
char buffer[RX_LEN]; //���ڽ������ݻ���

void UartIsr() interrupt 4
{
	if(RI)
	{
		RI = 0;
		buffer[rx_count++] = SBUF;
		if(SBUF == '\n')
			rx_count |= RX_LEN;
		buffer[rx_count] = 0;
	}
}

void Uart_Init(void)
{
	SCON = 0x50;
	T2L = BRT;
	T2H = BRT>>8;
	AUXR = 0x15;
	rx_count = 0;
	ES = 1;
}

void Uart_Send(char dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void Uart_SendStr(char *p)
{
	while(*p)
	{
		Uart_Send(*p++);
	}
}