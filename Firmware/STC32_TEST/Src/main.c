#include "stc32g.h"
#include "intrins.h"

#include "sys.h"
#include "motor.h"
#include "uart.h"
#include "rf24.h"
#include "timer.h"

bit rx_flag = 1;
bit txc_flag = 0;
unsigned char rx[11] = {0};
unsigned char tx[11] = {'O','K',0};
bit unconnect_flag = 1;
unsigned short unconnect_count = 0;
void update_1ms(void)
{
	unconnect_count++;
	if(unconnect_count > 3000)
	{
		unconnect_flag = 1;
	}
	if(txc_flag)
	{
		RF24_RX_Mode();
		txc_flag = 0;
	}
	if(rx_flag)
	{
		if(RF24_IRQ == 0)
		{
			RF24_Read_Buf(RD_RX_PLOAD,rx,11);     //读取数据
			CE_LOW();
			RF24_Write_Reg(WRITE_REG+STATUS,0x40);
			CE_HIGH();
			unconnect_flag = 0;
			unconnect_count = 0;
			if(rx[0] == 'D')
			{
				motor_run(rx[1]);
				motor_turn(rx[4]);
			}
			rx_flag = 0;
			RF24_TX_Mode();
		}
	}
	else
	{
		CE_LOW();
		RF24_Write_Buf(WR_TX_PLOAD, tx, 11);
		CE_HIGH();
		rx_flag = 1;
		txc_flag = 1;
	}
}

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
	
	P5M0 &= ~(0x01<<3);
	P5M0 |= (0x01<<3);
	
	P53 = 0;
	
	delay_ms(200);
	
	//uart_init();
	motor_init();
	
	//RF24初始化并进入RX模式
	RF24_Init();
	RF24_Write_Reg(WRITE_REG + EN_AA,0x00);	//无自动确认
	RF24_Write_Reg(WRITE_REG+EN_RXADDR,0x01);	//使能管道0
	RF24_Write_Reg(WRITE_REG+SETUP_RETR,0x00);	//关闭自动重发
	RF24_RX_Mode();
	//RF24_TX_Mode();
	RF24_Set_Channel(66);
	RF24_Set_Power(RF_PWR_0);
	RF24_Set_P0_Size(11);
	RF24_Set_Rate(RATE_1M_BPS);
	RF24_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
	rx_flag = 1;
	txc_flag = 0;
	
	timer0_init(1000, update_1ms);
	
	EA = 1;
	
	while(1)
	{
		if(unconnect_flag)
		{
			rx[1] = 127;
			rx[4] = 127;
			motor_run(rx[1]);
			motor_turn(rx[4]);
			//uart_sendStr("unconnect\r\n");
		}
		
		/*
		motor_turn(100);
		delay_ms(1000);
		
		motor_turn(80);
		delay_ms(1000);
		
		motor_turn(60);
		delay_ms(1000);
		
		motor_turn(40);
		delay_ms(1000);
		
		motor_turn(20);
		delay_ms(1000);
		
		motor_turn(0);
		delay_ms(1000);
		*/
	}
}