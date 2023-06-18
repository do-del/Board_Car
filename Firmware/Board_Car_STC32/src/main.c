#include "stc32g.h"
#include "intrins.h"
#include "stc.h"
#include "usb.h"

#define LED_PIN P53

void sys_init(void);

void main(void)
{
	sys_init();
	usb_init();  //USB CDC �ӿ�����
  EA = 1;
	
	LED_PIN = 0;
	
	while(1)
	{
	}
}

void sys_init(void)
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

	//====== USB ��ʼ�� ======
	P3M0 &= ~0x03;
	P3M1 |= 0x03;

	IRC48MCR = 0x80;
	while (!(IRC48MCR & 0x01));

	USBCLK = 0x00;
	USBCON = 0x90;
	//========================
}