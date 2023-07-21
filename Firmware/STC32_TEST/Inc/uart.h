#ifndef UART_H
#define UART_H

#include "stc32g.h"
#include "intrins.h"
#include "sys.h"

#define BRT (65536 - (MAIN_FOSC / 115200 + 2) / 4)

void uart_init(void);
void uart_send(char ch);
void uart_sendStr(char *p);
void uart_sendNum(unsigned long num);

#endif
