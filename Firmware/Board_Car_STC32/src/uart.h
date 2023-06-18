#ifndef UART_H
#define UART_H

#include "stc32g.h"
#include "intrins.h"
#include "config.h"

#define DEBUG_ENABLE 1

#define DEBUG_BAUDRATE (115200UL)
#define DEBUG_BRT (65536-(MAIN_FOSC/DEBUG_BAUDRATE+2)/4)

void DebugInit(void);
void DebugSend(char ch);
void DebugSendStr(char *p);

#endif
