#include "motor.h"

#if (USE_IO == 1)
#define MOTOR_A_P P10
#define MOTOR_A_N P11
#define MOTOR_B_P P16
#define MOTOR_B_N P17
#endif

#if (USE_PWM == 1)
#define MOTOR_DEFAULT_CYCLE 127
#define MOTOR_SHRESHOLD 15
#endif

void motor_init(void)
{
#if (USE_PWM == 1)
	pwm_init();
	pwma_setcycle(MOTOR_DEFAULT_CYCLE);
	motor_run(127);
	motor_turn(127);
#endif
	
#if (USE_IO == 1)
	P1M0 &= ~0xc3; //0x11000011
	P1M0 |= 0xc3;
	P1M1 &= ~0xc3;
	P1PU |= 0xc3;
#endif
}

void motor_run(unsigned char vel)
{
#if (USE_PWM == 1)
	unsigned char tmp;
	//unsigned int duty;
	if(vel > 127)
	{
		tmp = vel - 127;
		pwm1p_enable();
	}
	else
	{
		tmp = 127 - vel;
		pwm1n_enable();
	}
	tmp = Limit(tmp, MOTOR_SHRESHOLD);
	//duty = Map(0, 127, 0, MOTOR_DEFAULT_CYCLE, tmp);
	pwm1_setduty(tmp);
#endif
	
#if (USE_IO == 1)
	if(vel > 132)
	{
		MOTOR_A_N = 0;
		delay_ms(1);
		MOTOR_A_P = 1;
	}
	else if (vel < 122)
	{
		MOTOR_A_P = 0;
		delay_ms(1);
		MOTOR_A_N = 1;
	}
	else
	{
		MOTOR_A_P = 1;
		MOTOR_A_N = 1;
	}
#endif
}

void motor_turn(unsigned char vel)
{
#if (USE_PWM == 1)
	unsigned char tmp;
	//unsigned int duty;
	if(vel > 127)
	{
		tmp = vel - 127;
		pwm4p_enable();
	}
	else
	{
		tmp = 127 - vel;
		pwm4n_enable();
	}
	tmp = Limit(tmp, MOTOR_SHRESHOLD);
	//duty = Map(0, 127, 0, MOTOR_DEFAULT_CYCLE, tmp);
	pwm4_setduty(tmp);
#endif
	
#if (USE_IO == 1)
	if(vel > 132)
	{
		MOTOR_B_N = 0;
		delay_ms(1);
		MOTOR_B_P = 1;
	}
	else if (vel < 122)
	{
		MOTOR_B_P = 0;
		delay_ms(1);
		MOTOR_B_N = 1;
	}
	else
	{
		MOTOR_B_P = 1;
		MOTOR_B_N = 1;
	}
#endif
}