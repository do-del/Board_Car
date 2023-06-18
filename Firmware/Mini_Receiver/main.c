#include "stc8h.h"
#include "intrins.h"
#include "RF24.h"
#include "uart.h"
#include "sys.h"
#include "eeprom.h"
#include "pwm.h"

#define CH9 P37
#define CH10 P36

#define KEY P33
#define LED P24

void delay(unsigned int dly) // N ms延时函数
{
	unsigned int j;
	do
	{
		j = FOSC / 10000;
		while(--j)	;
	}while(--dly);
}

unsigned char rx[11] = {0};
unsigned char tx[11] = {0};

void main()
{
	P_SW2 |= 0x80;
	
	P2M0 |= 0x10;//0b0001 0000
	P2M1 &= ~0x10; //LED配置为推挽输出
	
	P0M0 |= 0x0f; //0b0000 1111
	P0M1 &= ~0x0f;
	
	P2M0 &= ~0x80; //0b1000 0000
	P2M1 |= 0x80;
	
	P3M0 &= ~0x08; //0b0000 1000
	P3M1 |= 0x08; //KEY配置为高阻输入
	P3PU |= 0x08;	//KEY配置上拉电阻
	
	P3M0 |= 0xc0;
	P3M1 &= ~0xc0;
	
	LED = 1;
	
	Uart_Init();
	EA = 1;
	Uart_SendStr("Hello World\r\n");
	
	RF24_Init();
	RF24_Write_Reg(WRITE_REG + EN_AA,0x00);
	RF24_Write_Reg(WRITE_REG+EN_RXADDR,0x01);
	RF24_Write_Reg(WRITE_REG+SETUP_RETR,0x00);
	RF24_RX_Mode();
	//RF24_TX_Mode();
	RF24_Set_Channel(66);
	RF24_Set_Power(RF_PWR_0);
	RF24_Set_P0_Size(11);
	RF24_Set_Rate(RATE_1M_BPS);
	RF24_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
	delay(1000);
	PWMA_Init();
	PWMB_Init();
	while(1)
	{
		if(IRQ == 0)
		{
			//uint8_t i;
			LED = 0;
			RF24_Read_Buf(RD_RX_PLOAD,rx,11);     //读取数据
			CE = 0;
			RF24_Write_Reg(WRITE_REG+STATUS,0x40);
			CE = 1;
			
			/*
			for(i = 0;i<11;i++)
			{
				Uart_Send(rx[i]);
			}
			*/
			if(rx[0] == 'D')
			{
				PWM_Set_Duty(CH_0,(uint16_t)(rx[1]<<3)+500);
				PWM_Set_Duty(CH_1,(uint16_t)(rx[2]<<3)+500);
				PWM_Set_Duty(CH_2,(uint16_t)(rx[3]<<3)+500);
				PWM_Set_Duty(CH_3,(uint16_t)(rx[4]<<3)+500);
				CH9 = rx[5];
				CH10 = rx[6];
			}
			tx[0] = 'O';
			tx[1] = 'K';
			RF24_TX_Mode();
			delay(2);
			CE = 0;
			RF24_Write_Buf(WR_TX_PLOAD, tx, 11);
			CE = 1;
			delay(1);
			RF24_RX_Mode();
		}
		
		
		/*
		RF24_TX_Mode();
	  tx[0] = 0x44;
		tx[1] = 'e';
		tx[2] = 'l';
		tx[3] = 'd';
		tx[4] = 'e';
		tx[5] = 'l';
		tx[6] = '-';
		tx[7] = 'D';
		tx[8] = 'F';
		tx[9] = 'L';
		tx[10] = 'Y';
		CE = 0;
		RF24_Write_Buf(WR_TX_PLOAD, tx, 11);	    //写数据到TX FIFO,32个字节
		CE = 1;
		delay(1);
		RF24_RX_Mode();
		delay(1000);
		*/
	}
}