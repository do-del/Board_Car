#ifndef I2C_H
#define I2C_H

#include "stc32g.h"
#include "intrins.h"

#define SDA P14
#define SCL P15

void I2C_Wait(void);
void I2C_Start(void);
void I2C_SendData(char dat);
unsigned char I2C_RecvACK(void);
unsigned char I2C_RecvData(void);
void I2C_SendACK(void);
void I2C_SendNAK(void);
void I2C_Stop(void);
void I2C_Init(void);
unsigned char I2C_SendByte(unsigned char dat);
unsigned char I2C_RecvByte(void);
void I2C_Write(unsigned char dev_addr, unsigned char reg_addr, unsigned char *p,unsigned char len);
void I2C_Read(unsigned char dev_addr,unsigned char reg_addr, unsigned char *buf, unsigned char len);
//void I2C_Scan(void);

#endif
