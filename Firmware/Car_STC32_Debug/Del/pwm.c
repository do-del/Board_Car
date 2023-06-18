#include "pwm.h"

void pwm_init(void)
{
	PWMA_CCER1 = 0x00;
	PWMA_CCER2 = 0x00;
	PWMA_CCMR1 = 0x60;
	PWMA_CCMR4 = 0x60;
	
	PWMA_PSCRH = 0x00;
	PWMA_PSCRL = 0x17;
	
	PWMA_BKR = 0x80;
	PWMA_CR1 = 0x01;
}

void pwm1p_enable(void)
{
	pwm1n_disable();
	PWM1N_PIN = 0;
	PWMA_CCER1 = 0x01;
	PWMA_ENO |= 0x01;
}

void pwm1n_enable(void)
{
	pwm1p_disable();
	PWM1P_PIN = 0;
	PWMA_CCER1 = 0x01<<2;
	PWMA_ENO |= (0x01<<1);
}

void pwm4p_enable(void)
{
	pwm4n_disable();
	PWM4N_PIN = 0;
	PWMA_CCER2 = 0x01 << 4;
	PWMA_ENO |= (0x01<<6);
}

void pwm4n_enable(void)
{
	pwm4p_disable();
	PWM4P_PIN = 0;
	PWMA_CCER2 = 0x01 << 6;
	PWMA_ENO |= (0x01<<7);
}

void pwm1p_disable(void)
{
	PWMA_ENO &= ~(0x01);
	PWM1P_PIN = 0;
}

void pwm1n_disable(void)
{
	PWMA_ENO &= ~(0x01<<1);
	PWM1N_PIN = 0;
}

void pwm4p_disable(void)
{
	PWMA_ENO &= ~(0x01<<6);
	PWM4P_PIN = 0;
}

void pwm4n_disable(void)
{
	PWMA_ENO &= ~(0x01<<7);
	PWM4N_PIN = 0;
}

void pwma_setcycle(unsigned int cycle)
{
	PWMA_ARRH = (cycle>>8) & 0xff;
	PWMA_ARRL = cycle & 0xff;
}

void pwm1_setduty(unsigned int duty)
{
	PWMA_CCR1H = (duty>>8) & 0xff;
	PWMA_CCR1L = duty & 0xff;
}

void pwm4_setduty(unsigned int duty)
{
	PWMA_CCR4H = (duty>>8) & 0xff;
	PWMA_CCR4L = (duty>>8) & 0xff;
}

void pwm1p_config(void)
{
	PWMA_CCER1 = 0x00;	//дCCMRxǰ��Ҫ����CCERx
	PWMA_CCMR1 = 0x60;	//PWM1ģʽ
	
	PWMA_CCER1 = 0x01;	//OC1���ʹ��
	PWMA_CCR1H = 0x01;	//����/�ȽϼĴ���
	PWMA_CCR1L = 0x00;
	PWMA_ARRH = 0x02;	//�Զ���װ�ؼĴ���
	PWMA_ARRL = 0x00;
	PWMA_ENO = 0x01;	//���ʹ�ܼĴ���
	PWMA_BKR = 0x80;
	PWMA_CR1 = 0x01;	//���ƼĴ���������������
}

void pwm1n_config(void)
{
	PWMA_CCER1 = 0x00;
	PWMA_CCMR1 = 0x60;
	PWMA_CCER1 = (0x01 << 2);
	
	PWMA_CCR1H = 0x01;	//����/�ȽϼĴ���
	PWMA_CCR1L = 0x00;
	PWMA_ARRH = 0x02;	//�Զ���װ�ؼĴ���
	PWMA_ARRL = 0x00;
	PWMA_ENO = (0x01<<1);	//���ʹ�ܼĴ���
	PWMA_BKR = 0x80;
	PWMA_CR1 = 0x01;	//���ƼĴ���������������
}

void pwm4p_config(void)
{
	PWMA_CCER2 = 0x00;	//дCCMRxǰ��Ҫ����CCERx
	PWMA_CCMR4 = 0x60;	//PWM1ģʽ
	
	PWMA_CCER2= 0x01 << 4;	//OC4���ʹ��
	PWMA_CCR4H = 0x01;	//����/�ȽϼĴ���
	PWMA_CCR4L = 0x00;
	PWMA_ARRH = 0x02;	//�Զ���װ�ؼĴ���
	PWMA_ARRL = 0x00;
	PWMA_ENO = 0x01<<6;	//���ʹ�ܼĴ���
	PWMA_BKR = 0x80;
	PWMA_CR1 = 0x01;	//���ƼĴ���������������
}

void pwm4n_config(void)
{
	PWMA_CCER2 = 0x00;
	PWMA_CCMR4 = 0x60;
	PWMA_CCER2 = (0x01 << 6);
	
	PWMA_CCR4H = 0x01;	//����/�ȽϼĴ���
	PWMA_CCR4L = 0x00;
	PWMA_ARRH = 0x02;	//�Զ���װ�ؼĴ���
	PWMA_ARRL = 0x00;
	PWMA_ENO = (0x01<<7);	//���ʹ�ܼĴ���
	PWMA_BKR = 0x80;
	PWMA_CR1 = 0x01;	//���ƼĴ���������������
}