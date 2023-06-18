#include "stc32g.h"
#include "intrins.h"
#include "stc.h"
#include "usb.h"

#define LED_PIN P53

void sys_init(void);

void main(void)
{
	sys_init();
	usb_init();  //USB CDC 接口配置
  EA = 1;
	
	LED_PIN = 0;
	
	while(1)
	{
	}
}

void sys_init(void)
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

	//====== USB 初始化 ======
	P3M0 &= ~0x03;
	P3M1 |= 0x03;

	IRC48MCR = 0x80;
	while (!(IRC48MCR & 0x01));

	USBCLK = 0x00;
	USBCON = 0x90;
	//========================
}