#ifndef UART_H
#define UART_H

#include <stc8h.h>
#include <intrins.h>
#include "sys.h"

#define BRT (65536-(FOSC/115200+2)/4) //波特率转计数值
#define RX_LEN 0x10 //串口接收数据缓存长度
#define RX_LEN_MASK 0x0f 

void Uart_Init(void);
void Uart_Send(char dat);
void Uart_SendStr(char *p);

#endif
