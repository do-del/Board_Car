#include "stc32g.h"
#include "intrins.h"
#include "pwm.h"

void main(void)
{
	WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXFR = 1; //扩展寄存器(XFR)访问使能
	CKCON = 0; //提高访问XRAM速度

	P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
	P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
	P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
	P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
	P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
	P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
	P6M1 = 0x00;   P6M0 = 0x00;   //设置为准双向口
	P7M1 = 0x00;   P7M0 = 0x00;   //设置为准双向口
	
	pwm_init();
	pwma_setcycle(1000);
	pwm1_setduty(300);
	pwm4_setduty(300);
	
	pwm1p_enable();
	pwm4p_enable();
	
	while(1)
	{
		
	}
}