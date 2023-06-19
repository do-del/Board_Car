#ifndef RF24_H
#define RF24_h

#include "stc32g.h"
#include "intrins.h"
#include "spi.h"

#define RF24_CSN P26
#define RF24_IRQ P22
#define RF24_CE P27

#define MOSI_HIGH() (MOSI = 1)
#define MOSI_LOW() (MOSI = 0)
//#define MISO_HIGH() (PORTB |= 0x10)
//#define MISO_LOW() (PORTB &= ~0x10)
#define MISO_VALUE() (MISO)
#define SCK_HIGH() (SCK = 1)
#define SCK_LOW() (SCK = 0)
#define CSN_HIGH() (RF24_CSN = 1)
#define CSN_LOW() (RF24_CSN = 0)
#define CE_HIGH() (RF24_CE = 1) //CE高电平，芯片由Standby模式进入到Idle-TX或RX模式
#define CE_LOW() (RF24_CE = 0) //CE低电平，芯片返回Standby模式
#define IRQ_VALUE() (RF24_IRQ)

#define TX_ADR_WIDTH   5  				// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32  				// 数据通道有效数据宽度
extern unsigned char TX_ADDRESS[];

//********************************************************************************************************************//
// SPI(RF24) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(RF24) registers(addresses)
#define CONFIG          0x00  // 配置寄存器
#define EN_AA           0x01  // 使能数据管道自动确认
#define EN_RXADDR       0x02  // 使能接收数据管道地址
#define SETUP_AW        0x03  // 地址宽度配置
#define SETUP_RETR      0x04  // 自动重发配置
#define RF_CH           0x05  // 射频信道，设置通信的频段
#define RF_SETUP        0x06  // 射频配置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发射结果统计
#define RSSI            0x09  // 接收信号强度检测
#define RX_ADDR_P0      0x0A  // 数据管道0的接收地址
#define RX_ADDR_P1      0x0B  // 数据管道1的接收地址
#define RX_ADDR_P2      0x0C  // 数据管道2的接收地址
#define RX_ADDR_P3      0x0D  // 数据管道3的接收地址
#define RX_ADDR_P4      0x0E  // 数据管道4的接收地址
#define RX_ADDR_P5      0x0F  // 数据管道5的接收地址
#define TX_ADDR         0x10  // 发射端的发射地址
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // FIFO状态

//********************************************************************************************************************//
// 状态寄存器中的标志位
#define RX_DR						0x40  // RX FIFO有值标志位，写‘1’清除
#define TX_DS						0x20  //发射完成中断位，写‘1’清除
#define MAX_RT					0x10  //达到最大重发次数中断位

/********************************************************
函数功能：RF24引脚初始化                
入口参数：无
返回  值：无
*********************************************************/
void RF24_Init(void);

/********************************************************
函数功能：写寄存器的值（单字节）                
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
unsigned char RF24_Write_Reg(unsigned char reg, unsigned char value);
#define RF24_RxFlag_Clr() RF24_Write_Reg(WRITE_REG+STATUS,0x40)

/********************************************************
函数功能：写寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
unsigned char RF24_Write_Buf(unsigned char reg, const unsigned char *pBuf, unsigned char bytes);						  					   
#define RF24_FIFO_Write(p,i) RF24_Write_Buf(WR_TX_PLOAD, p, i)
#define RF24_Set_TxAddress(p,i) RF24_Write_Buf(WRITE_REG + TX_ADDR, p, i)
#define RF24_Set_RxAddress(p,i) RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, p, i)

/********************************************************
函数功能：读取寄存器的值（单字节）                  
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
unsigned char RF24_Read_Reg(unsigned char reg);

/********************************************************
函数功能：读取寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（READ_REG｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
unsigned char RF24_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
#define RF24_FIFO_Read(p,i) RF24_Read_Buf(RD_RX_PLOAD,p,i)

/********************************************************
函数功能：RF24接收模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void RF24_RX_Mode(void);			

/********************************************************
函数功能：RF24发送模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void RF24_TX_Mode(void);

/********************************************************
函数功能：读取接收数据                       
入口参数：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/
unsigned char RF24_RxPacket(unsigned char *rxbuf);

/********************************************************
函数功能：发送一个数据包                      
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败 
          0x20:发送成功            
          0xff:发送失败                  
*********************************************************/
unsigned char RF24_TxPacket(unsigned char *txbuf);

#define	RF_PWR_N12 0x00
#define	RF_PWR_N6 0x01
#define RF_PWR_N4 0x02
#define RF_PWR_0 0x03
#define RF_PWR_P1 0x04
#define RF_PWR_P3 0x05
#define RF_PWR_P4 0x06
#define RF_PWR_P7 0x07
void RF24_Set_Power(unsigned char p);

void RF24_Set_Channel(unsigned char ch);

#define RATE_1M_BPS 0x00
#define RATE_2M_BPS 0x01
#define RATE_250K_BPS 0x02
#define RATE_RESERVED 0x03
void RF24_Set_Rate(unsigned char rate);

void RF24_Set_P0_Size(unsigned char len);

#endif