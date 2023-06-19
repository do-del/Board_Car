/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

/*************  ����˵��    **************

�����̻���STC32G����ת�Ӱ壨�����������б�д���ԡ�

ʹ��USB�����Ӻ��İ�USB�ӿ������;

MCUͨ��USB CDC(Communication Device Class)Э��ʶ��Ϊ�����豸;

ʹ�ô������ִ�CDC���⴮�ڣ�Ȼ����MCU�������ݣ�MCU���ؽ��յ������ݵ���������;

���ݳ���������63�ֽ����ڡ�

���������ʾ���ָ�λ����USB����ģʽ�ķ�����
1. ͨ��ÿ1����ִ��һ�Ρ�KeyResetScan��������ʵ�ֳ���P3.2�ڰ�������MCU��λ������USB����ģʽ��
   (�����ϣ����λ����USB����ģʽ�Ļ������ڸ�λ�����ｫ IAP_CONTR ��bit6��0��ѡ��λ���û�������)
2. ͨ�����ء�stc_usb_cdc_32g.lib���⺯����ʵ��ʹ��STC-ISP�������ָ���MCU��λ������USB����ģʽ���Զ����ء�
   (ע�⣺ʹ��CDC�ӿڴ���MCU��λ���Զ����ع��ܣ���Ҫ��ѡ�˿����ã��´�ǿ��ʹ�á�STC USB Writer (HID)������ISP����)

����ʱ, ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��)��

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

#define MAIN_Fosc       24000000L   //������ʱ��

char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";                      //�����Զ���λ��ISP�����û��ӿ�����

//P3.2�ڰ�����λ�������
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
			RF24_Read_Buf(RD_RX_PLOAD,rx,11);     //��ȡ����
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
	sys_init();  //ϵͳ��ʼ��
	usb_init();  //USB CDC �ӿ�����
	EA = 1;
	
	while(DeviceState != DEVSTATE_CONFIGURED); //�ȴ�USB�������
	
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
	
	//PWM��ʼ��
	pwm_init();
	pwma_setcycle(1000);
	pwm1_setduty(300);
	pwm4_setduty(300);
	pwm1p_enable();
	pwm4p_enable();
	pwm1n_enable();
	pwm4n_enable();
	
	//RF24��ʼ��������RXģʽ
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
	RF24_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	rx_flag = 1;
	
	P20 = 1;
	
	while (1)
	{
		delay_ms(1);
    KeyResetScan();   //����P3.2�ڰ������������λ������USB����ģʽ������Ҫ�˹��ܿ�ɾ�����д���
		
		if(DeviceState != DEVSTATE_CONFIGURED)  //�ȴ�USB�������
			continue;

		if (bUsbOutReady)
		{
			usb_OUT_done();    //����Ӧ�𣨹̶���ʽ��
			
			printf("OutNumber=0x%X\r\n",OutNumber);  //ʹ�� printf ������ӡ�������ݳ���

			memcpy(UsbInBuffer, UsbOutBuffer, OutNumber);  //����������(UsbOutBuffer)�����Ƶ����ͻ�����(UsbInBuffer)
			usb_IN(OutNumber);      //ԭ·����, ���ڲ���
		}
	}
}

void sys_init()
{
	WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
	CKCON = 0; //��߷���XRAM�ٶ�

	P0M1 = 0x00;   P0M0 = 0x00;   //����Ϊ׼˫���
	P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
	P2M1 = 0x00;   P2M0 = 0x00;   //����Ϊ׼˫���
	P3M1 = 0x00;   P3M0 = 0x00;   //����Ϊ׼˫���
	P4M1 = 0x00;   P4M0 = 0x00;   //����Ϊ׼˫���
	P5M1 = 0x00;   P5M0 = 0x00;   //����Ϊ׼˫���
	P6M1 = 0x00;   P6M0 = 0x00;   //����Ϊ׼˫���
	P7M1 = 0x00;   P7M0 = 0x00;   //����Ϊ׼˫���
	
	Debug_Init();	//���Կڳ�ʼ��
	
	I2C_Init();
	
	timer0_init(1000, update_1ms);
}

//========================================================================
// ����: void delay_ms(u8 ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2021-3-9
// ��ע: 
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
// ����: void KeyResetScan(void)
// ����: P3.2�ڰ�������1�봥�������λ������USB����ģʽ��
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2022-6-11
// ��ע: 
//========================================================================
void KeyResetScan(void)
{
    if(!P32)
    {
        if(!Key_Flag)
        {
            Key_cnt++;
            if(Key_cnt >= 1000)		//����1000ms��Ч�������
            {
                Key_Flag = 1;		//���ð���״̬����ֹ�ظ�����

                USBCON = 0x00;      //���USB����
                USBCLK = 0x00;
                IRC48MCR = 0x00;
                
                delay_ms(10);
                IAP_CONTR = 0x60;   //���������λ����ISP��ʼִ��
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