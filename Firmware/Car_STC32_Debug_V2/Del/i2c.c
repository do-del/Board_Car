#include "i2c.h"
#include "debug.h"
#include <stdio.h>

void I2C_Wait(void)
{
	while(!(I2CMSST & 0x40));
	I2CMSST &= ~0x40;
}

void I2C_Start(void)
{
	I2CMSCR = 0x01;
	I2C_Wait();
}

void I2C_Stop(void)
{
	I2CMSCR = 0x06;
	I2C_Wait();
}

void I2C_SendData(char dat)
{
	I2CTXD = dat;
	I2CMSCR = 0x02;
	I2C_Wait();
}

unsigned char I2C_RecvACK(void)
{
	I2CMSCR = 0x03;
	I2C_Wait();
	return (I2CMSST & 0x02);
}

unsigned char I2C_RecvData(void)
{
	I2CMSCR = 0x04;
	I2C_Wait();
	return I2CRXD;
}

void I2C_SendACK(void)
{
	I2CMSST = 0x00;
	I2CMSCR = 0x05;
	I2C_Wait();
}

void I2C_SendNAK(void)
{
	I2CMSST = 0x01;
	I2CMSCR = 0x05;
	I2C_Wait();
}

void I2C_Init(void)
{
	P1PU |= 0x30; //0x00110000
	I2CCFG = 0xdb;
	I2CMSST = 0x00;
}

unsigned char I2C_SendByte(unsigned char dat)
{
	I2C_SendData(dat);
	return I2C_RecvACK();
}

unsigned char I2C_RecvByte(void)
{
	unsigned char dat;
	dat = I2C_RecvData();
	I2C_SendACK();
	return dat;
}

void I2C_Write(unsigned char dev_addr, unsigned char reg_addr, unsigned char *p,unsigned char len)
{
	unsigned char i = 0;
	I2C_Start();
	I2C_SendByte(dev_addr<<1);
	I2C_SendByte(reg_addr);
	for(i = 0; i < len; i++)
	{
		I2C_SendByte(p[i]);
	}
	I2C_Stop();
}

void I2C_read(unsigned char dev_addr,unsigned char reg_addr, unsigned char *buf, unsigned char len)
{
	unsigned char i = 0;
	I2C_Start();
	I2C_SendByte(dev_addr<<1);
	I2C_SendByte(reg_addr);
	I2C_Start();
	I2C_SendByte((dev_addr<<1)+1);
	for(i = 0; i < len-1; i++)
	{
		buf[i] = I2C_RecvByte();
	}
	buf[i] = I2C_RecvData();
	I2C_SendNAK();
	I2C_Stop();
}