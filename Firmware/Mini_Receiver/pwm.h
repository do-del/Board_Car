#ifndef PWM_H
#define PWM_H

#include <stc8h.h>
#include <intrins.h>
#include "sys.h"

#define PWM1 P10
#define PWM2 P12
#define PWM3 P14
#define PWM4 P16
#define PWM5 P20
#define PWM6 P21
#define PWM7 P22
#define PWM8 P23

#define CH_0 8
#define CH_1 7
#define CH_2 6
#define CH_3 5 
#define CH_4 1
#define CH_5 2
#define CH_6 3
#define CH_7 4

void PWMA_Init(void);
void PWMB_Init(void);
void PWM_Set_Duty(unsigned char ch,uint16_t duty);

#endif
