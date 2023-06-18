#include "pwm.h"

void PWMA_Init(void)
{
	PWM1 = 0;
	PWM2 = 0;
	PWM3 = 0;
	PWM4 = 0;
	
	P1M0 |= 0x55; //0b0101 0101
	P1M1 &= ~0x55;
	
	PWMA_PSCR = 11;
	PWMA_ARR = 19999;
	PWMA_CCER1 = 0;
	PWMA_CCER2 = 0;
	PWMA_SR1 = 0;
	PWMA_SR2 = 0;
	PWMA_ENO = 0;
	PWMA_PS = 0;
	PWMA_IER = 0;
	
	//PWM1
	PWMA_CCMR1 = 0x68;
	PWMA_CCR1 = 1499;
	PWMA_CCER1 |= 0x01;
	
	PWMA_CCMR2 = 0x68;
	PWMA_CCR2 = 1499;
	PWMA_CCER1 |= 0x10;
	
	PWMA_CCMR3 = 0x68;
	PWMA_CCR3 = 1499;
	PWMA_CCER2 |= 0x01;
	
	PWMA_CCMR4 = 0x68;
	PWMA_CCR4 = 1499;
	PWMA_CCER2 |= 0x10;
	
	PWMA_BKR = 0x80;
	PWMA_CR1 = 0x81;
	PWMA_EGR = 0x01;
	
	PWMA_ENO |= 0x55; //0b0101 0101
}

void PWMB_Init(void)
{
	PWM5 = 0;
	PWM6 = 0;
	PWM7 = 0;
	PWM8 = 0;
	
	P2M0 |= 0x0f; //0b0000 1111
	P2M1 &= ~0x0f;
	
	PWMB_PSCR = 11;
	PWMB_ARR = 19999;
	PWMB_CCER1 = 0;
	PWMB_CCER2 = 0;
	PWMB_SR1 = 0;
	PWMB_SR2 = 0;
	PWMB_ENO = 0;
	PWMB_PS = 0;
	PWMB_IER = 0;
	
	//PWM5
	PWMB_CCMR1 = 0x68;
	PWMB_CCR5 = 1499;
	PWMB_CCER1 |= 0x01;
	
	PWMB_CCMR2 = 0x68;
	PWMB_CCR6 = 1499;
	PWMB_CCER1 |= 0x10;
	
	PWMB_CCMR3 = 0x68;
	PWMB_CCR7 = 1499;
	PWMB_CCER2 |= 0x01;
	
	PWMB_CCMR4 = 0x68;
	PWMB_CCR8 = 1499;
	PWMB_CCER2 |= 0x10;
	
	PWMB_BKR = 0x80;
	PWMB_CR1 = 0x81;
	PWMB_EGR = 0x01;
	
	PWMB_ENO |= 0x55;	//0b0101 0101
}

void PWM_Set_Duty(unsigned char ch,uint16_t duty)
{
	switch(ch)
	{
		case CH_0:
			PWMB_CCR8 = duty;
			break;
		case CH_1:
			PWMB_CCR7 = duty;
			break;
		case CH_2:
			PWMB_CCR6 = duty;
			break;
		case CH_3:
			PWMB_CCR5 = duty;
			break;
		case CH_4:
			PWMA_CCR1 = duty;
			break;
		case CH_5:
			PWMA_CCR2 = duty;
			break;
		case CH_6:
			PWMA_CCR3 = duty;
			break;
		case CH_7:
			PWMA_CCR4 = duty;
			break;
		default:
			break;
	}
}
