#include "stc32g.h"
#include "intrins.h"
#include "pwm.h"

void main(void)
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