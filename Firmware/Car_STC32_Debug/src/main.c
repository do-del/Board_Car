/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

/*************  功能说明    **************

本例程基于STC32G核心转接板（屠龙刀）进行编写测试。

使用USB线连接核心板USB接口与电脑;

MCU通过USB CDC(Communication Device Class)协议识别为串口设备;

使用串口助手打开CDC虚拟串口，然后向MCU发送数据，MCU返回接收到的数据到串口助手;

数据长度限制在63字节以内。

此外程序演示两种复位进入USB下载模式的方法：
1. 通过每1毫秒执行一次“KeyResetScan”函数，实现长按P3.2口按键触发MCU复位，进入USB下载模式。
   (如果不希望复位进入USB下载模式的话，可在复位代码里将 IAP_CONTR 的bit6清0，选择复位进用户程序区)
2. 通过加载“stc_usb_cdc_32g.lib”库函数，实现使用STC-ISP软件发送指令触发MCU复位，进入USB下载模式并自动下载。
   (注意：使用CDC接口触发MCU复位并自动下载功能，需要勾选端口设置：下次强制使用”STC USB Writer (HID)”进行ISP下载)

下载时, 选择时钟 24MHZ (用户可自行修改频率)。

******************************************/

#include "stc.h"
#include "usb.h"
#include "debug.h"
#include "led.h"
#include "i2c.h"
#include "mpu6050.h"
#include "pwm.h"
#include "rf24.h"
#include "timer.h"

#define MAIN_Fosc       24000000L   //定义主时钟

char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";                      //设置自动复位到ISP区的用户接口命令

//P3.2口按键复位所需变量
bit Key_Flag;
u16 Key_cnt;

bit rx_flag = 1;
unsigned char rx[11] = {0};
unsigned char tx[11] = {'O','K',0};

mpu6050_struct mpu_dat;

void sys_init();
void delay_ms(u8 ms);
void KeyResetScan(void);

void I2C_Scan(void)
{
#if DEBUG_ENABLE
	unsigned char addr_test;
	unsigned char flag = 0;
	
	printf("i2c scan start...\r\n");
	
	for(addr_test = 0; addr_test < 200; addr_test++)
	{
		I2C_Start();
		if(!I2C_SendByte(addr_test<<1))	flag = 1;
		I2C_Stop();
		if(flag)
		{
			flag = 0;
			printf("Find addr: %X\r\n", addr_test);
		}
	}
	
	printf("i2c scan done\r\n");
	
#endif
}

void update_1ms(void)
{
	if(rx_flag)
	{
		if(RF24_IRQ == 0)
		{
			//uint8_t i;
			RF24_Read_Buf(RD_RX_PLOAD,rx,11);     //读取数据
			CE_LOW();
			RF24_Write_Reg(WRITE_REG+STATUS,0x40);
			CE_HIGH();
			
			/*
			for(i = 0;i<11;i++)
			{
				Uart_Send(rx[i]);
			}
			*/
			printf("rx:%c\r\n", rx[0]);
			if(rx[0] == 'D')
			{
				/*
				PWM_Set_Duty(CH_0,(uint16_t)(rx[1]<<3)+500);
				PWM_Set_Duty(CH_1,(uint16_t)(rx[2]<<3)+500);
				PWM_Set_Duty(CH_2,(uint16_t)(rx[3]<<3)+500);
				PWM_Set_Duty(CH_3,(uint16_t)(rx[4]<<3)+500);
				CH9 = rx[5];
				CH10 = rx[6];
				*/
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
		delay_ms(1);
		rx_flag = 1;
		RF24_RX_Mode();
	}
}

void main()
{
	sys_init();  //系统初始化
	usb_init();  //USB CDC 接口配置
	EA = 1;
	
	while(DeviceState != DEVSTATE_CONFIGURED); //等待USB完成配置
	
	P20 = 0;
	P21= 0;
	P3PU |= 0x08;
	delay_ms(200);
	delay_ms(200);
	delay_ms(200);
	delay_ms(200);
	delay_ms(200);
	P21 = 1;
	printf("---------STC32 Debug----------\r\n");
	//I2C_Scan();
	//mpu6050_init();
	
	//PWM初始化
	pwm_init();
	pwma_setcycle(1000);
	pwm1_setduty(300);
	pwm4_setduty(300);
	pwm1p_enable();
	pwm4p_enable();
	pwm1n_enable();
	pwm4n_enable();
	
	//RF24初始化并进入RX模式
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
	rx_flag = 1;
	
	P20 = 1;
	
	while (1)
	{
		delay_ms(1);
    KeyResetScan();   //长按P3.2口按键触发软件复位，进入USB下载模式，不需要此功能可删除本行代码
		
		if(DeviceState != DEVSTATE_CONFIGURED)  //等待USB完成配置
			continue;

		if (bUsbOutReady)
		{
			usb_OUT_done();    //接收应答（固定格式）
			
			printf("OutNumber=0x%X\r\n",OutNumber);  //使用 printf 函数打印接收数据长度

			memcpy(UsbInBuffer, UsbOutBuffer, OutNumber);  //将接收数据(UsbOutBuffer)，复制到发送缓冲区(UsbInBuffer)
			usb_IN(OutNumber);      //原路返回, 用于测试
		}
	}
}

void sys_init()
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
	
	Debug_Init();	//调试口初始化
	
	I2C_Init();
	
	timer0_init(1000, update_1ms);
}

//========================================================================
// 函数: void delay_ms(u8 ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2021-3-9
// 备注: 
//========================================================================
void delay_ms(u8 ms)
{
     u16 i;
     do{
          i = MAIN_Fosc / 6000;
          while(--i);   //6T per loop
     }while(--ms);
}

//========================================================================
// 函数: void KeyResetScan(void)
// 描述: P3.2口按键长按1秒触发软件复位，进入USB下载模式。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2022-6-11
// 备注: 
//========================================================================
void KeyResetScan(void)
{
    if(!P32)
    {
        if(!Key_Flag)
        {
            Key_cnt++;
            if(Key_cnt >= 1000)		//连续1000ms有效按键检测
            {
                Key_Flag = 1;		//设置按键状态，防止重复触发

                USBCON = 0x00;      //清除USB设置
                USBCLK = 0x00;
                IRC48MCR = 0x00;
                
                delay_ms(10);
                IAP_CONTR = 0x60;   //触发软件复位，从ISP开始执行
                while (1);
            }
        }
    }
    else
    {
        Key_cnt = 0;
        Key_Flag = 0;
    }
}